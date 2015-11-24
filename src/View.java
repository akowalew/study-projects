import java.awt.Container;
import java.awt.GridLayout;
import java.util.Observable;
import java.util.Observer;
import javax.swing.*;
import java.awt.* ;

public class View  {

	JFrame frame ;
	JButton btn1, btn2, btn3 ;
	JLabel lbl1, lbl2 ;
	
	private Controller cntrl ;
	
	private void initComponents() {
		frame = new JFrame("Widok") ;
		Container pane = frame.getContentPane() ;
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		
		btn1 = new JButton("Utwórz AND") ;
		btn2 = new JButton("Utwórz OR") ;
		btn3 = new JButton("Utwórz NOT") ;
		
		lbl1 = new JLabel("           ") ;
		lbl2 = new JLabel("           ") ;
		
		pane.setLayout(new GridLayout(0, 1));
		
		pane.add(btn1) ;
		pane.add(btn2) ;
		pane.add(btn3) ;
		pane.add(lbl1) ;
		pane.add(lbl2) ;
		
		frame.pack(); 
		
	}
	
	public View() {
		initComponents() ;
		
		SwingUtilities.invokeLater(new Runnable() {
			
			@Override
			public void run() {
				// TODO Auto-generated method stub
				frame.setVisible(true);
			}
		});
	}
	
	public void setController(Controller cntrl) {
		this.cntrl = cntrl ;
	}
	

}
