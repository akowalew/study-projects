import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowEvent;
import java.util.ArrayList;
import java.util.Observable;
import java.util.Observer;
import java.util.Vector;

import javax.swing.*;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;
import javax.swing.table.AbstractTableModel;
import javax.swing.table.DefaultTableColumnModel;
import javax.swing.table.TableColumn;
import javax.swing.table.TableColumnModel;
import javax.xml.crypto.Data;

import java.awt.* ;

public class View extends AbstractView  {
 
	private JFrame frame ;
	private Container framePane ;
	
	private final Controller controller ;
	private final ProcessQueue queue ;
	
	private MyListBox listbox ;
	private MyTable table ;
	
	private void initMenuComponent() {
		
		JMenuBar menuBar ;
		JMenu programMenu, workspaceMenu ;
		
		JMenuItem dialogAction ;
 		JMenuItem exitAction ;
 		JMenuItem addAndGateAction ;
 		JMenuItem addOrGateAction ;
 		JMenuItem addNotGateAction ;
		
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
		
		final int btnSize = 65 ;
		
		JPanel panel = new JPanel(new FlowLayout()) ;
		panel.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10));
		panel.setPreferredSize(new Dimension(100, 450));
		panel.setBackground(new Color(100, 100, 100));
		
		JButton btn ;
		
		btn = new JButton("AND");
		btn.setPreferredSize(new Dimension(btnSize, btnSize));
		btn.addActionListener(new ActionListener() {			
			@Override public void actionPerformed(ActionEvent e) {
				createAndGateButtonPressed(e);
			}
		});
		panel.add(btn);
		
		btn = new JButton("OR");
		btn.setPreferredSize(new Dimension(btnSize, btnSize));
		btn.addActionListener(new ActionListener() {			
			@Override public void actionPerformed(ActionEvent e) {
				createOrGateButtonPressed(e);
			}
		});
		panel.add(btn);
		
		btn = new JButton("NOT") ;
		btn.setPreferredSize(new Dimension(btnSize,btnSize));
		btn.addActionListener(new ActionListener() {			
			@Override public void actionPerformed(ActionEvent e) {
				createNotGateButtonPressed(e);
			}
		});
		panel.add( btn);
		

		framePane.add(panel, BorderLayout.WEST) ;
	}
	
	private void initEastBorderComponents() {
		JPanel panel = new JPanel(new FlowLayout()) ;
		panel.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10));
		panel.setBackground(new Color(150,150,150));
		
		final int prefferedWidth = 200 ;
		panel.setPreferredSize(new Dimension(prefferedWidth, 450));

		JLabel tmplbl = new JLabel("Components List") ;
		JLabel tmplbl2 = new JLabel("Component info") ;
		
		listbox = new MyListBox(prefferedWidth) ;
		table = new MyTable(prefferedWidth) ;
		
		panel.add(tmplbl) ;
		panel.add(listbox.getScrollPane()) ;
		panel.add(tmplbl2);
		panel.add(table.getScrollPane());
		
		framePane.add(panel, BorderLayout.EAST) ;
	}

	private void initCenterBorderComponents() {
		JPanel pane = new JPanel() ;
		pane.setBackground(new Color(50,50,50));
		pane.add(new JButton("Siema")) ;
		
		framePane.add(pane, BorderLayout.CENTER) ;
	}
	
	private void initComponents() {
		frame = new JFrame("View") ;
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		
		framePane = frame.getContentPane() ;
		framePane.setLayout(new BorderLayout(10, 10));
				
		initMenuComponent();
		initEastBorderComponents() ;
		initWestBorderComponents();
		initCenterBorderComponents();
		
		
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
	
	

	
	private void labelSetText(final JLabel label, final String text) {
		SwingUtilities.invokeLater(new Runnable() {
			@Override public void run() {
				label.setText(text);
			}
		}) ;
	}

	private void addLogicGate(LogicGate gate) {
		//labelSetText(lbl1, gate.toString());
		listbox.listBoxAddElem(gate); // dodanie elementu do listy
		table.showLogicGateInfo(gate);
	}
	
	@Override
	public void modelUpdate(Observable arg0, Object arg1) {
		EwbMVC.p("View: Model się zmienił");
		// TODO Auto-generated method stub
		if(arg1 instanceof Model.ModelMessage) {
			Model.ModelMessage msg = (Model.ModelMessage) arg1 ;
			if(msg.getMessage() == Model.MessageType.CREATED_OBJECT) {
				LogicGate gate = (LogicGate) msg.getObject() ;
				addLogicGate(gate) ;
			}
 		}
	}
	
	private class MyTable {
		
		final JTable table ;
		final JScrollPane scrollPane ;
		
		private MyTableModel myTableModel ;
		
		class MyTableModel extends AbstractTableModel {

			ArrayList<Object[]> data ;
			String[] columnNames ;
			
			MyTableModel(Object[][] obj, String[] header) {
				this.columnNames = header ;
				data = new ArrayList<Object[]>() ;
				
				if(obj != null)
					for(int i = 0 ; i < obj.length ; i++)
						data.add(obj[i]) ;
			}
			
			@Override public String getColumnName(int index) {
				return columnNames[index] ;
			}
			
			@Override public int getColumnCount() {
				return columnNames.length ;
			}

			@Override public int getRowCount() {
				return data.size() ;
			}

			@Override public Object getValueAt(int rowIndex, int columnIndex) {
				return data.get(rowIndex)[columnIndex];
			}
		
			public void addRow(Object[] e) {
				data.add(e) ;
				fireTableDataChanged();
			}
			
			public void deleteRows() {
				data.clear(); 
				fireTableDataChanged(); 
			}
		}
		
		public MyTable(int prefferedWidth) {
			
			String[] columnNames = { 	"Property",
										"Value" } ;
			
			myTableModel = new MyTableModel(null, columnNames) ;
			
			table = new JTable(myTableModel) ;
			table.setPreferredScrollableViewportSize(new Dimension(prefferedWidth-20, 150));
			table.setFillsViewportHeight(true);
			
			TableColumn column = table.getColumnModel().getColumn(0) ;
			column.setPreferredWidth(100);
			
			scrollPane = new JScrollPane(table) ;
		}
		
		public JScrollPane getScrollPane() {
			return scrollPane ;
		}
		
		public void showLogicGateInfo(LogicGate gate) {
			myTableModel.deleteRows();
			
			Object[] row ;
			
			row = new Object[2] ;
			row[0] = new String("Name") ;
			row[1] = new String(gate.getName()) ;
			
			myTableModel.addRow(row);
			
			row = new Object[2] ;
			row[0] = new String("Number of inputs") ;
			row[1] = new Integer(gate.getInputsSize()) ;
			
			myTableModel.addRow(row);
			
			for(int i = 0 ; i < gate.getInputsSize() ; i++) {
				row = new Object[2] ;
				row[0] = new String("Input " + i + " value") ;
				row[1] = (new Boolean(gate.getInput(i))).booleanValue() ;
				myTableModel.addRow(row);
 			}
		}
	}
	
	private class MyListBox {
		private JList<LogicGate> listBox ;
		private Vector<LogicGate> listData ;
		private JScrollPane listScroller ;
		
		public MyListBox(int prefferedWidth) {
			listData = new Vector<LogicGate>() ;
			listBox = new JList<LogicGate>(listData) ;
			
			listBox.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
			listBox.setLayoutOrientation(JList.VERTICAL);
			listBox.addListSelectionListener(new ListSelectionListener() {
				@Override public void valueChanged(ListSelectionEvent e) {
					listItemPressed(e) ;
				}
			});
			
			listScroller = new JScrollPane(listBox) ;
			listScroller.setPreferredSize(new Dimension(prefferedWidth-20, 200));
			
		}
		
		public JScrollPane getScrollPane() {
			return listScroller ;
		}
		
		private void listItemPressed(ListSelectionEvent e) {
			JList<LogicGate> obj ;
			if(e.getValueIsAdjusting() == false) {
				if(e.getSource() == listBox) {
					
					obj = (JList<LogicGate>) e.getSource() ;
					if(obj.getSelectedIndex() >= 0) {
						LogicGate gate = obj.getSelectedValue() ;
						table.showLogicGateInfo(gate);
						//labelSetText(lbl2, "" + obj.getSelectedIndex()) ;
					}
				}
			}
		}
		
		public void listBoxAddElem(final LogicGate gate) {
			listData.addElement(gate);
			SwingUtilities.invokeLater(new Runnable() {
				@Override public void run() {
					listBox.setListData(listData);
				}
			});
		}
		
	}
}
