import java.beans.PropertyChangeSupport;
import java.util.ArrayList;

public class Model {

	ArrayList<LogicGate> logicGates ;
	ArrayList<AndGate>	andGates ;
	
	
	public Model() {
		logicGates = new ArrayList<LogicGate>() ;
		andGates = new ArrayList<AndGate>() ;
	}
	
	AndGate createAnd() {
		AndGate gate = new AndGate() ;
		logicGates.add(gate) ;
		andGates.add(gate) ;
		return gate ;
	}
	
	OrGate createOr() {
		OrGate gate = new OrGate() ;
		logicGates.add(gate) ;
		return gate ;
	}
	
	NotGate createNot() {
		NotGate gate = new NotGate() ;
		logicGates.add(gate ) ;
		return gate ;
	}
}
