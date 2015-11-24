import java.util.Observable;

public class AndGate extends LogicGate {
	private static int count = 0 ;
	
	public AndGate() {
		super(2, "AND GATE" + count) ;
	}
	
	@Override
	protected boolean getLogicState() {
		return getInput(0) && getInput(1) ;
	}

}
