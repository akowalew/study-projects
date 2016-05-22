import java.util.ArrayList;

public class Model extends AbstractModel {
	ArrayList<LogicGate> logicGates ;
	ArrayList<AndGate>	andGates ;
	
	public Model() {
		logicGates = new ArrayList<LogicGate>() ;
		andGates = new ArrayList<AndGate>() ;
	}
	
	public void initDefault() {
		
	}
	
	public AndGate createAnd() {
		EwbMVC.p("Model: Dostałem polecenie createAnd");
		AndGate gate = new AndGate() ;
		logicGates.add(gate) ;
		andGates.add(gate) ;
		
		this.setChanged();
		notifyObservers(new ModelMessage(MessageType.CREATED_OBJECT, gate));
		return gate ;
	}
	
	public OrGate createOr() {
		EwbMVC.p("Model: Dostałem polecenie createOr");
		OrGate gate = new OrGate() ;
		logicGates.add(gate) ;
		
		this.setChanged();
		notifyObservers(new ModelMessage(MessageType.CREATED_OBJECT, gate));
		return gate ;
	}
	
	public NotGate createNot() {
		EwbMVC.p("Model: Dostałem polecenie createNot");
		NotGate gate = new NotGate() ;
		logicGates.add(gate ) ;
		
		this.setChanged();
		notifyObservers(new ModelMessage(MessageType.CREATED_OBJECT, gate));
		return gate ;
	}
	
	
	public enum MessageType {
		CREATED_OBJECT,
		removedObject,
	}
	
	public class ModelMessage {
		private MessageType msg ;
		private Object obj ;
		
		public ModelMessage(MessageType msg, Object obj) {
			this.msg = msg ;
			this.obj = obj ;
		}
		
		public Object getObject() {
			return obj ;
		}
		
		public MessageType getMessage() {
			return msg ;
		}
	}
}
