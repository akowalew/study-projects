
public class NotGate extends LogicGate {
	static int count = 0 ;
	
	public NotGate() {
		super(1, "NOT" + count++) ;
	}
	
	@Override
	protected boolean getLogicState() {
		// TODO Auto-generated method stub
		return !getInput(0) ;
	}

}
