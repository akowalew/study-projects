
public class OrGate extends LogicGate {
	static int count = 0 ;
	public OrGate() {
		super(2, "OR Gate" + count) ;
	}
	
	@Override
	protected boolean getLogicState() {
		// TODO Auto-generated method stub
		return getInput(0) || getInput(1) ;
	}

}
