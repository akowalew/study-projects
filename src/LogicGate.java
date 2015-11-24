import java.util.Observable;

public abstract class LogicGate extends Observable {

	final int INPUTS_SIZE ; 
	private boolean inputPins[] ;
	String name ;
	
	protected abstract boolean getLogicState() ; 
	
	public LogicGate(int inputsSize, String name) {
		inputPins = new boolean[inputsSize] ;
		INPUTS_SIZE = inputsSize ;
		this.name = new String(name) ;
	}
	
	public void setInput(int whichInputPin, boolean logicState) throws ArrayIndexOutOfBoundsException {
		boolean prev = getOutput() ;
		
		inputPins[whichInputPin] = logicState ;
		
		if(prev != getOutput()) {
			setChanged(); 
			notifyObservers();
		}
	}
	
	public boolean getInput(int whichInputPin) throws ArrayIndexOutOfBoundsException {
		return inputPins[whichInputPin] ;
	}
	
	public boolean getOutput() {
		return getLogicState() ;
	}
	
	public int getInputsSize() {
		return INPUTS_SIZE ;
	}
	
	public String getName() {
		return name ;
	}
	
	public String toString() {
		return name ;
	}
	
}
