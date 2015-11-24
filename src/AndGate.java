import java.util.Observable;

public class AndGate extends LogicGate {
	
	public AndGate() {
		super(2, "AND GATE") ;
	}
	
	@Override
	protected boolean getLogicState() {
		return getInput(0) && getInput(1) ;
	}

}
