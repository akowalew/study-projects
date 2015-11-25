import java.util.ArrayList;
import java.util.Observable;
import java.util.Observer;

public abstract class AbstractController implements Observer {
	private ArrayList<AbstractView> views ;
	
	void addView(AbstractView view) {
		views.add(view) ;
	}
	
	void deleteView(AbstractView view) {
		views.remove(view) ;
	}
	
	public AbstractController() {
		views = new ArrayList<AbstractView>() ;
	}
	
	@Override
	public void update(Observable arg0, Object arg1) {
		EwbMVC.p("Controller: Model się zmienił");
		// TODO Auto-generated method stub
		for(AbstractView view : views) {
			view.modelUpdate(arg0, arg1);
		}
	}
}
