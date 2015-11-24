import java.util.Observable;
import java.util.Observer;

public class Controller implements Observer {

	View viewVar ;
	
	public Controller(View vieww) {
		viewVar = vieww ;
	}
	
	@Override
	public void update(Observable o, Object arg) {
		// TODO Auto-generated method stub

	}

}
