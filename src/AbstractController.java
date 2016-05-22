import java.util.ArrayList;
import java.util.Observable;
import java.util.Observer;

/**
 * Klasa podstawowa dla naszego kontrolera. Implementuje interfejs Observer, 
 * przez co może być używany jako obserwator zdarzeń pochodzących od modeli
 * 
 * @see Observer
 * @author dicker
 *
 */

public abstract class AbstractController implements Observer {
	private ArrayList<AbstractView> views ;
	
	/**
	 * Podpina referencję do widoku do tablicy zawartej w kontrolerze,
	 * przez co Kontroler może później przekazywać mu zdarzenia modelu
	 * @param view Referencja do widoku, który ma zostać podpięty 
	 * @see AbstractView
	 */
	public void addView(AbstractView view) {
		views.add(view) ;
	}
	
	/**
	 * Odpina referencję do widoku z tablicy zawartej w kontrolerze,
	 * przez co Kontroler nie będzie już przekazywać mu zdarzeń modelu
	 * @param view Referencja do widoku, który ma zostać odpięty
	 * @see AbstractView
	 */
	public void deleteView(AbstractView view) {
		views.remove(view) ;
	}
	
	/**
	 * Konstruktor podstawowy Kontrolera, inicjuje tablicę referencji do widoków
	 */
	public AbstractController() {
		views = new ArrayList<AbstractView>() ;
	}
	
	/**
	 * Funckja wywoływana przez obiekty, które obserwuje Kontroler(przez Modele) po wywołaniu 
	 * przez nie metody notifyObservers
	 * @param arg0	Referencja do modelu, który zawołał zmianę
	 * @param arg1	Obiekt przesyłany przez model
	 */
	@Override public void update(Observable arg0, Object arg1) {
		EwbMVC.p("Controller: Model się zmienił");
		// TODO Auto-generated method stub
		for(AbstractView view : views) {
			view.modelUpdate(arg0, arg1);
		}
	}
}
