
public class EwbMVC {

	static void p(Object o) {
		System.out.println(o) ;
	}
	
	public static void main(String[] args) {
		// TODO Auto-generated method stub
		
		Model model = new Model() ;
		Controller controller = new Controller(model) ;
		View view = new View(controller) ;
		
		controller.registerView(view);
		
	}
	
	

}
