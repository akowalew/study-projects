import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowEvent;
import java.util.Observable;
import java.util.Observer;
import java.util.Vector;

import javax.swing.*;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;
import javax.swing.table.TableColumn;

import java.awt.* ;

public class View extends AbstractView  {
 
	private JFrame frame ;
	private Container framePane ;
	
	private JButton btn1, btn2, btn3 ;
	
	private JLabel lbl1, lbl2 ;
	
	private JMenuBar menuBar ;
	
	private JMenu programMenu, workspaceMenu ;
	
	private JMenuItem dialogAction ;
	private JMenuItem exitAction ;
	private JMenuItem addAndGateAction ;
	private JMenuItem addOrGateAction ;
	private JMenuItem addNotGateAction ;
	
	private JList<LogicGate> listBox ;
	private Vector<LogicGate> listData ;
	
	private final Controller controller ;
	private final ProcessQueue queue ;
	
	private void initMenuComponent() {
		menuBar = new JMenuBar() ;
		frame.setJMenuBar(menuBar);
		
		programMenu = new JMenu("Program") ;
		workspaceMenu = new JMenu("Workspace") ;
		menuBar.add(programMenu) ;
		menuBar.add(workspaceMenu) ;
		
		dialogAction = new JMenuItem("Dialog") ;
		exitAction = new JMenuItem("Exit");
		programMenu.add(dialogAction) ;
		programMenu.addSeparator(); 
		programMenu.add(exitAction) ;
		
		addAndGateAction = new JMenuItem("Add an AND Gate");
		addOrGateAction = new JMenuItem("Add an OR Gate");
		addNotGateAction = new JMenuItem("Add a NOT Gate");
		workspaceMenu.add(addAndGateAction) ;
		workspaceMenu.add(addOrGateAction) ;
		workspaceMenu.add(addNotGateAction) ;
		
		addAndGateAction.addActionListener(new ActionListener() {			
			@Override public void actionPerformed(ActionEvent e) {
				createAndGateButtonPressed(e);				
			}
		});
		
		addOrGateAction.addActionListener(new ActionListener() {			
			@Override public void actionPerformed(ActionEvent e) {
				createOrGateButtonPressed(e);
			}
		});

		addNotGateAction.addActionListener(new ActionListener() {			
			@Override public void actionPerformed(ActionEvent e) {
				createNotGateButtonPressed(e);
			}
		});
		
		exitAction.addActionListener(new ActionListener() {			
			@Override public void actionPerformed(ActionEvent e) {
				frame.setVisible(false);
				frame.dispose(); 
				System.exit(0);
			}
		});
	}

	private void initWestBorderComponents() {
		JPanel panel = new JPanel(new FlowLayout()) ;
		panel.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10));
		panel.setPreferredSize(new Dimension(100, 450));
		
		JButton btn = new JButton("AND");
		btn.setPreferredSize(new Dimension(70, 70));
		
		panel.add(btn);
		panel.add(new JButton("OR"));
		panel.add(new JButton("NOT"));
		panel.add(new JButton("XOR"));
		framePane.add(panel, BorderLayout.WEST) ;
	}
	
	private void initEastBorderComponents() {
		JPanel panel = new JPanel(new FlowLayout()) ;
		panel.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10));
		final int prefferedWidth = 200 ;
		panel.setPreferredSize(new Dimension(prefferedWidth, 450));
		
		listData = new Vector<LogicGate>() ;
		listBox = new JList<LogicGate>(listData) ;
		//listBox.setPreferredSize(new Dimension(200, 200));
		listBox.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
		listBox.setLayoutOrientation(JList.VERTICAL);
		listBox.addListSelectionListener(new ListSelectionListener() {
			@Override public void valueChanged(ListSelectionEvent e) {
				listItemPressed(e) ;
			}
		});
		
		JLabel tmplbl = new JLabel("Components List") ;

		JScrollPane listScroller = new JScrollPane(listBox) ;
		listScroller.setPreferredSize(new Dimension(prefferedWidth-20, 200));
		
		JLabel tmplbl2 = new JLabel("Component info") ;
		
		String[] columnNames = { 	"Property",
									"Value" } ;
		Object[][] data = {
				{ "Name", "AND0" },
				{ "Category", "Logic Gate" },
				{ "Output0 state", new Boolean(true) },
				{ "Input0 state", new Boolean(true) },
				{ "Input1 state", new Boolean(true) }
		} ;
		
		JTable table = new JTable(data, columnNames) ;
		table.setPreferredScrollableViewportSize(new Dimension(prefferedWidth-20, 150));
		table.setFillsViewportHeight(true);
		
		TableColumn column = table.getColumnModel().getColumn(0) ;
		column.setPreferredWidth(100);

		panel.add(tmplbl) ;
		panel.add(listScroller) ;
		panel.add(tmplbl2);
		panel.add(new JScrollPane(table));
		framePane.add(panel, BorderLayout.EAST) ;
	}
	
	private void initComponents() {
		frame = new JFrame("View") ;
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		
		framePane = frame.getContentPane() ;
		framePane.setLayout(new BorderLayout(10, 10));
				
		initMenuComponent();
		initEastBorderComponents() ;
		initWestBorderComponents();

		lbl1 = new JLabel("   sdadasdasd          ") ;
		lbl2 = new JLabel("                       ") ;
		
		frame.pack(); 
	}
	
	public View(Controller controller) {
		this.controller = controller ;
		queue = controller.getViewsQueue() ;
		
		initComponents() ;
		
		SwingUtilities.invokeLater(new Runnable() {
			
			@Override
			public void run() {
				// TODO Auto-generated method stub
				frame.setVisible(true);
			}
		});
	}

	private void createAndGateButtonPressed(ActionEvent e) {
		queue.addProcess(new Runnable() {
			@Override public void run() {				
				controller.createAndGateRequest();
			}
		});
	}
	
	private void createOrGateButtonPressed(ActionEvent e) {
		queue.addProcess(new Runnable() {
			@Override public void run() {				
				controller.createOrGateRequest();
			}
		});
	}
	
	private void createNotGateButtonPressed(ActionEvent e) {
		queue.addProcess(new Runnable() {
			@Override public void run() {				
				controller.createNotGateRequest();
			}
		});
	}
	
	private void listItemPressed(ListSelectionEvent e) {
		JList<LogicGate> obj ;
		if(e.getValueIsAdjusting() == false) {
			if(e.getSource() == listBox) {
				
				obj = (JList<LogicGate>) e.getSource() ;
				if(obj.getSelectedIndex() >= 0) {
					labelSetText(lbl2, "" + obj.getSelectedIndex()) ;
				}
			}
		}
	}

	private void addLogicGate(LogicGate gate) {
		labelSetText(lbl1, gate.toString());
		listBoxAddElem(gate);
	}
	
	private void listBoxAddElem(final LogicGate gate) {
		listData.addElement(gate);
		SwingUtilities.invokeLater(new Runnable() {
			@Override public void run() {
				listBox.setListData(listData);
			}
		});
	}
	
	private void labelSetText(final JLabel label, final String text) {
		SwingUtilities.invokeLater(new Runnable() {
			@Override public void run() {
				label.setText(text);
			}
		}) ;
	}

	@Override
	public void modelUpdate(Observable arg0, Object arg1) {
		EwbMVC.p("View: Model się zmienił");
		// TODO Auto-generated method stub
		if(arg1 instanceof Model.ModelMessage) {
			Model.ModelMessage msg = (Model.ModelMessage) arg1 ;
			if(msg.getMessage() == Model.MessageType.CREATED_OBJECT) {
				addLogicGate((LogicGate)msg.getObject());
			}
 		}
	}
}
