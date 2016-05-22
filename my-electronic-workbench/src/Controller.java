import java.util.Observable;

/**
 * Kontroler
 * - Kontroler przechwytuje zdarzenia modeli i wysyła je do widoków
 * - Kontroler jest interfejsem do modeli. Może nie wykorzystać wszystkich ich 
 * możliwości, wedle uznania
 * - Do kontrolera odwoływać się będzie widok poprzez jego publiczne metody
 * - Jest napisany pod konkretny schemat modelu (jednego modelu)
 * 
 */

public class Controller extends AbstractController {
	private final Model model ;
	private final ProcessQueue viewsQueue ;
	private final ProcessQueue modelQueue ;
	
	/**
	 * Konstruktor właściwy Kontrolera. Inicjuje dodatkowo kolejki procesów dla Modelu i dla Widoku
	 * Tworzy odpowiedni egzemplarz Modelu
	 * 
	 */
	public Controller() {
		this.model = new Model() ;
		model.initDefault();
		
		model.addObserver(this);
		viewsQueue = new ProcessQueue() ;
		modelQueue = new ProcessQueue() ;
	}
	
	/**
	 * @return Zwraca referencję do kolejki procesowej dla Widoków
	 */
	public ProcessQueue getViewsQueue() {
		return viewsQueue ;
	}
	
	/**
	 * Żądanie utworzenia bramki And. Zadaniem kontrolera jest wywołać odpowiednie metody Modelu tak, by
	 * utworzył nową bramkę And
	 */
	void createAndGateRequest() {
		EwbMVC.p("Zlecam wykonanie Model:createAnd");
		
		modelQueue.addProcess(new Runnable() {
			@Override public void run() {
				model.createAnd() ;
			}
		}) ; 
	}
	
	/**
	 * Żądanie utworzenia bramki Or. Zadaniem kontrolera jest wywołać odpowiednie metody Modelu tak, by 
	 * utworzył w sobie nową bramkę Or.
	 */
	void createOrGateRequest() {
		EwbMVC.p("Zlecam wykonanie Model:createOr");

		modelQueue.addProcess(new Runnable() {
			@Override public void run() {
				model.createOr() ;
			}
		}) ;
	}
	
	/**
	 * Żądanie utworzenia bramki NOT. Zadaniem kontrolera jest wywołać odpowiednie metody Modelu tak,
	 * by utworzł w sobie nową bramkę Not
	 */
	void createNotGateRequest() {
		EwbMVC.p("Zlecam wykonanie Model:createNot");
		
		modelQueue.addProcess(new Runnable() {
			@Override public void run() {
				model.createNot() ;
			}
		}); 
	}
	
}
