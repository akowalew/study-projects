
/**
 * Klasa lącząca cały model MVC w jedność. Utuchamia aplikację
 * 
 * @author dicker
 *
 */
public class EwbMVC {

	/**
	 * Drukuje na ekranie. Zamiennik dla System.out.println(..)
	 * @param o Obiekt do wydrukowania na ekranie
	 */
	public static void p(Object o) {
		System.out.println(o) ;
	}
	
	
	/**
	 * Konstruktor, tworzy Kontroler(który tworzy w sobie egzemplarz Modelu) 
	 * oraz widok. Dodaje widok do listy widoków Kontrolera
	 */
	public EwbMVC() {
		Controller controller = new Controller() ;
		View view = new View(controller) ;
		
		controller.addView(view);
	}
	
	/**
	 * Metoda główna programu. Wywołuje konstruktor EwbMVC
	 * @see EwbMVC
	 * @param args
	 */
	public static void main(String[] args) {
		new EwbMVC() ;
	}
	
	

}
