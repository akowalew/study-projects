
public class NotGate extends LogicGate {

	public NotGate() {
		super(1, "NOT Gate") ;
	}
	
	@Override
	protected boolean getLogicState() {
		// TODO Auto-generated method stub
		return !getInput(0) ;
	}

}
