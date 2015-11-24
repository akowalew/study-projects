
public class Controller {
	private View view ;
	private Model model ;
	
	public Controller(Model model) {
		this.model = model ;
	}
	
	public void registerView(View view) {
		this.view = view ;
	}
	
	public void unregisterView() {
		this.view = null ;
	}
	
	void createAndGateRequest() {
		LogicGate gate ;
		EwbMVC.p("Zlecam wykonanie Model:createAnd");
		gate = model.createAnd() ;
		
		if(view != null)
			view.addLogicGate(gate);
	}
	
	void createOrGateRequest() {
		LogicGate gate ;
		EwbMVC.p("Zlecam wykonanie Model:createOr");
		gate = model.createOr() ;
	
		if(view != null)
			view.addLogicGate(gate);
	}
	
	void createNotGateRequest() {
		LogicGate gate ;
		EwbMVC.p("Zlecam wykonanie Model:createNot");
		gate = model.createNot() ;
		
		if(view != null)
			view.addLogicGate(gate);
	}
	
	
}
