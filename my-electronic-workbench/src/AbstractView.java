import java.util.Observable;

/**
 * Klasa podstawowa dla Widoku. Gwarantuje, że właściwy widok
 * zaimplementuje metodę modelUpdate, która będzie wywoływana przez kontroler podczas
 * przechwytywania zdarzeń pochodzących od modeli
 * @author dicker
 *
 */
public abstract class AbstractView {
	
	/**
	 * Metoda wywoływana wtedy, gdy model zmieni swój stan i postanowi poinformować o tym kontroler
	 * @param arg0	Model, który informuje o zmianie
	 * @param arg1	Obiekt wysyłany przez zmieniający się model.
	 */
	public abstract void modelUpdate(Observable arg0, Object arg1) ;
}
