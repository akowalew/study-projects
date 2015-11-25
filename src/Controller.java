import java.util.Observable;

/*
 * Kontroler
 * - Kontroler przechwytuje zdarzenia modeli i wysyła je do widoków
 * - Kontroler jest interfejsem do modeli. Może nie wykorzystać wszystkich ich 
 * możliwości, wedle uznania
 * - Do kontrolera odwoływać się będzie widok poprzez jego publiczne metody
 * - Jest napisany pod konkretny schemat modelu (jednego modelu)
 */

public class Controller extends AbstractController {
	private final Model model ;
	private final ProcessQueue viewsQueue ;
	private final ProcessQueue modelQueue ;
	
	public Controller(Model model) {
		this.model = model ;
		model.addObserver(this);
		viewsQueue = new ProcessQueue() ;
		modelQueue = new ProcessQueue() ;
	}
	
	public ProcessQueue getViewsQueue() {
		return viewsQueue ;
	}
	
	void createAndGateRequest() {
		EwbMVC.p("Zlecam wykonanie Model:createAnd");
		
		modelQueue.addProcess(new Runnable() {
			@Override public void run() {
				model.createAnd() ;
			}
		}) ; 
	}
	
	void createOrGateRequest() {
		EwbMVC.p("Zlecam wykonanie Model:createOr");

		modelQueue.addProcess(new Runnable() {
			@Override public void run() {
				model.createOr() ;
			}
		}) ;
	}
	
	void createNotGateRequest() {
		EwbMVC.p("Zlecam wykonanie Model:createNot");
		
		modelQueue.addProcess(new Runnable() {
			@Override public void run() {
				model.createNot() ;
			}
		}); 
	}
	
}
