import java.awt.Container;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowEvent;
import java.util.Observable;
import java.util.Observer;
import java.util.Vector;

import javax.swing.*;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;

import java.awt.* ;

public class View  {
 
	private JFrame frame ;
	
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
	
	Controller controller ;
	
	private void initComponents() {
		frame = new JFrame("View") ;
		Container pane = frame.getContentPane() ;
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		
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
			@Override
			public void actionPerformed(ActionEvent e) {
				createAndGateButtonPressed(e);				
			}
		});
		
		addOrGateAction.addActionListener(new ActionListener() {			
			@Override
			public void actionPerformed(ActionEvent e) {
				createOrGateButtonPressed(e);
			}
		});

		addNotGateAction.addActionListener(new ActionListener() {			
			@Override
			public void actionPerformed(ActionEvent e) {
				createNotGateButtonPressed(e);
			}
		});
		
		exitAction.addActionListener(new ActionListener() {			
			@Override
			public void actionPerformed(ActionEvent e) {
				frame.dispatchEvent(new WindowEvent(frame, WindowEvent.WINDOW_CLOSING));
			}
		});
		

		listData = new Vector<LogicGate>() ;
		listBox = new JList<LogicGate>(listData) ;
		//listBox.setPreferredSize(new Dimension(200, 200));
		listBox.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
		listBox.setLayoutOrientation(JList.VERTICAL);
		JScrollPane listScroller = new JScrollPane(listBox) ;
		listScroller.setPreferredSize(new Dimension(100, 300));
		listBox.addListSelectionListener(new ListSelectionListener() {
			@Override public void valueChanged(ListSelectionEvent e) {
				listItemPressed(e) ;
			}
		});
 		
		lbl1 = new JLabel("                       ") ;
		lbl2 = new JLabel("                       ") ;
		
		pane.setLayout(new FlowLayout());
		
		
		pane.add(listScroller) ;
		pane.add(lbl1) ;
		pane.add(lbl2) ;
		
		frame.pack(); 
		
	}
	
	public View(Controller controller) {
		this.controller = controller ;
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
		controller.createAndGateRequest();
	}
	
	private void createOrGateButtonPressed(ActionEvent e) {
		controller.createOrGateRequest();
	}
	
	private void createNotGateButtonPressed(ActionEvent e) {
		controller.createNotGateRequest();
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

	public void addLogicGate(LogicGate gate) {
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
}
