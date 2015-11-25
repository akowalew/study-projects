
public class EwbMVC {

	static void p(Object o) {
		System.out.println(o) ;
	}
	
	public EwbMVC() {
		Model model = new Model() ;
		Controller controller = new Controller(model) ;
		View view = new View(controller) ;
		
		controller.addView(view);
		
		model.initDefault();
	}
	
	public static void main(String[] args) {
		new EwbMVC() ;
	}
	
	

}
