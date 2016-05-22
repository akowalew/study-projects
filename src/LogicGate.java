import java.util.Observable;

/** 
 * Klasa podstawowa dla każdej bramki logicznej.
 * @author dicker
 *
 */
public abstract class LogicGate extends Observable {

	final int INPUTS_SIZE ; 
	private boolean inputPins[] ;
	final String name ;
	
	protected abstract boolean getLogicState() ; 
	
	/**
	 * Konstruktor podstawowy bramki logicznej
	 * @param inputsSize	Ile wejść ma dany układ logiczny
	 * @param name	Nazwa przypisana danemu egzemplarzowi układu
	 */
	public LogicGate(int inputsSize, String name) {
		inputPins = new boolean[inputsSize] ;
		INPUTS_SIZE = inputsSize ;
		this.name = new String(name) ;
	}
	
	/** 
	 * Ustawia stan dla pinu wejściowego danej bramki Logicznej
	 * @param whichInputPin	Który pin wejściowy ma być zmieniony(numeracja od 0)
	 * @param logicState	Stan pinu do ustawienia
	 * @throws ArrayIndexOutOfBoundsException	Jeśli podamy niepoprawny indeks wejścia
	 */
	public void setInput(int whichInputPin, boolean logicState) throws ArrayIndexOutOfBoundsException {
		boolean prev = getOutput() ;
		
		inputPins[whichInputPin] = logicState ;
		
		if(prev != getOutput()) {
			setChanged(); 
			notifyObservers();
		}
	}
	
	/**
	 * Pobiera stan logiczny pinu wejściowego
	 * @param whichInputPin Z którego pinu wejściowego pobieramy stan(numeracja od 0)
	 * @return	Stan logiczny pinu
	 * @throws ArrayIndexOutOfBoundsException Gdy podamy niepoprawny numer pinu
	 */
	public boolean getInput(int whichInputPin) throws ArrayIndexOutOfBoundsException {
		return inputPins[whichInputPin] ;
	}
	
	/**
	 * Pobieramy stan wyjścia. Mamy zawsze pojedyncze wyjście
	 * @return	Stan logiczny wyjścia
	 */
	public boolean getOutput() {
		return getLogicState() ;
	}
	
	/**
	 * @return Zwraca ilość pinów wejściowych danego układu
	 */
	public int getInputsSize() {
		return INPUTS_SIZE ;
	}
	
	/**
	 * 
	 * @return Zwraca nazwę danego układu wejściowego nadaną podczas jego konstrukcji
	 */
	public String getName() {
		return name ;
	}
	
	/**
	 * @see getName
	 */
	public String toString() {
		return name ;
	}
	
}
