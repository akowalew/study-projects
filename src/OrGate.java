
public class OrGate extends LogicGate {

	public OrGate() {
		super(2, "OR Gate") ;
	}
	
	@Override
	protected boolean getLogicState() {
		// TODO Auto-generated method stub
		return getInput(0) || getInput(1) ;
	}

}
