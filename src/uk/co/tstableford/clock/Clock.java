package uk.co.tstableford.clock;

import gnu.io.CommPort;
import gnu.io.CommPortIdentifier;
import gnu.io.NoSuchPortException;
import gnu.io.PortInUseException;
import gnu.io.SerialPort;
import gnu.io.SerialPortEvent;
import gnu.io.SerialPortEventListener;
import gnu.io.UnsupportedCommOperationException;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.lang.reflect.Field;
import java.util.Calendar;
import java.util.Collections;
import java.util.LinkedList;
import java.util.List;
import java.util.Scanner;
import java.util.TooManyListenersException;

public class Clock {
	private OutputStream out;
	private InputStream in;
	private SerialWriter writer;
	public static void main(String[] args) throws IOException, PortInUseException, 
	NoSuchPortException, UnsupportedCommOperationException, 
	TooManyListenersException, InterruptedException, NoSuchFieldException, SecurityException, IllegalArgumentException, IllegalAccessException {
		System.out.println(System.getProperty("user.dir")+System.getProperty("file.separator")+"lib");
		System.setProperty("java.library.path", System.getProperty("user.dir")+System.getProperty("file.separator")+"lib");
		Field fieldSysPath = ClassLoader.class.getDeclaredField( "sys_paths" );
		fieldSysPath.setAccessible( true );
		fieldSysPath.set( null, null );
		new Clock(args[0]);
	}
	public Clock(String portName) throws IOException, PortInUseException, NoSuchPortException, UnsupportedCommOperationException, TooManyListenersException, InterruptedException{
		CommPortIdentifier portIdentifier = CommPortIdentifier.getPortIdentifier(portName);
		if (portIdentifier.isCurrentlyOwned()){
			System.out.println("Error: Port is currently in use");
		}else{
			System.out.println("Opening serial port "+portName);
			CommPort commPort = portIdentifier.open(this.getClass().getName(),2000);

			if (commPort instanceof SerialPort){
				SerialPort serialPort = (SerialPort) commPort;
				serialPort.setSerialPortParams(9600,SerialPort.DATABITS_8,SerialPort.STOPBITS_1,SerialPort.PARITY_NONE);
				in = serialPort.getInputStream();
				out = serialPort.getOutputStream();
				writer = new SerialWriter();

				serialPort.addEventListener(new SerialReader());
				serialPort.notifyOnDataAvailable(true);

			}else{
				System.out.println("Error: Only serial ports are handled by this example.");
			}
		}
		(new Thread(writer)).start();
		menu();
	}
	private void printMenu(){
		System.out.println("(Q) Quit");
		System.out.println("(P) Print Time");
		System.out.println("(S) Sync Time");
		System.out.println("(E) Enter Time");
		System.out.println("(Y) Print Stored");
		System.out.println("(A) Set Stored As Alarm");
		System.out.println("(W) Print Alarm Time");
		System.out.println("(C) Print Compared Time");
	}
	private void menu(){
		Time time = new Time();
		char option;
		Scanner s = new Scanner(System.in);
		do{
			printMenu();
			option = s.nextLine().charAt(0);
			switch(option){
			case 'P':
				byte[] t = {'b', 'g', 'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'};
				writer.write(t);
				break;
			case 'S':
				byte b[] = new byte[10];
				b[0] = 'b'; b[1] = 'g'; b[2] = 's';
				Calendar now = Calendar.getInstance();
				b[3] = (byte)(now.get(Calendar.YEAR)-2000);
				b[4] = (byte)(now.get(Calendar.MONTH)+1);
				b[5] = (byte)now.get(Calendar.DAY_OF_MONTH);
				int dof = now.get(Calendar.DAY_OF_WEEK)-1;
				if(dof==0){ dof = 7; }
				b[6] = (byte)dof;
				b[7] = (byte)now.get(Calendar.HOUR_OF_DAY);
				b[8] = (byte)now.get(Calendar.MINUTE);
				b[9] = (byte)now.get(Calendar.SECOND);
				writer.write(b);
				break;
			case 'E':
				time.scan(s);
				break;
			case 'Y':
				System.out.println(time.toString());
				break;
			case 'A':
				byte[] by = time.getByte();
				by[2] = 'a';
				writer.write(by);
				break;
			case 'W':
				byte[] tt = {'b', 'g', 'w', 'p', 'p', 'p', 'p', 'p', 'p', 'p'};
				writer.write(tt);
				break;
			case 'C':
				byte[] ty = {'b', 'g', 'c', 'p', 'p', 'p', 'p', 'p', 'p', 'p'};
				writer.write(ty);
				break;
			}
		}while(option!='Q');
		s.close();
	}
	class SerialWriter implements Runnable{
		private List<Byte> buffer;
		public SerialWriter(){
			buffer = Collections.synchronizedList(new LinkedList<Byte>());
		}
		public synchronized void write(byte[] data){
			for(int i=0; i<data.length; i++){
				write(data[i]);
			}
		}
		private synchronized void write(byte data){
			buffer.add(data);
		}
		@Override
		public void run() {
			while(true){
				while(buffer.size()>0){
					try {
						out.write(buffer.remove(0));
					} catch (IOException e1) {
						e1.printStackTrace();
					}
				}
				try {
					Thread.sleep(50);
				} catch (InterruptedException e) {
					e.printStackTrace();
				}
			}
		}

	}
	class SerialReader implements SerialPortEventListener{
		private byte[] buffer = new byte[1024];
		public SerialReader(){}
		public void serialEvent(SerialPortEvent arg0) {
			int data;
			int len = 0;
			try{
			while ( ( data = in.read()) >= 0 )
			{
				if ( data == '\n' ) {
					break;
				}
				buffer[len++] = (byte) data;
			}
			}catch(IOException e){}
			System.out.print(new String(buffer,0,len));             
		}

	}
}

