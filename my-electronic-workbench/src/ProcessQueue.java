import java.util.LinkedList;

public class ProcessQueue {
	
	volatile private LinkedList<Runnable> fifo ; // pierwszy wszedł, pierwszy wyjdzie, kolejka
	
	private Thread thread ;
	
	public ProcessQueue() {
		fifo = new LinkedList<Runnable>() ;
		thread = new Thread(new Runnable() {
			@Override public void run() {
				runProcesses();
			}
		}) ;
		thread.start();
	}
	
	public void addProcess(Runnable process) {
		// dodaje proces i jeśli jest pierwszym procesem na liście 
		// to uruchamia osobny wątek, który wywoła procesy i opróżni listę
		fifo.add(process) ;
	}
	
	private void runProcesses() {
		// uruchamia wszystkie procesy na liście i opróżnia ją
		while(true) {
			while(!fifo.isEmpty())
				fifo.removeFirst().run();
		}
	}
}
