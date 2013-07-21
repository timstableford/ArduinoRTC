package uk.co.tstableford.clock;

import java.util.Scanner;

public class Time {
	private byte year;
	private byte month;
	private byte date;
	private byte day;
	private byte hour;
	private byte minute;
	private byte second;
	public Time(){}
	public void scan(Scanner s){
		System.out.println("Enter year (2000-2099): ");
		year = (byte)(s.nextInt()-2000);
		System.out.println("Enter month (1-12): ");
		month = (byte)s.nextInt();
		System.out.println("Enter date(1-31): ");
		date = (byte)s.nextInt();
		System.out.println("Enter day(1-7): ");
		day = (byte)s.nextInt();
		System.out.println("Enter hour(0-23): ");
		hour = (byte)s.nextInt();
		System.out.println("Enter minute(0-59): ");
		minute = (byte)s.nextInt();
		System.out.println("Enter second(0-59): ");
		second = (byte)s.nextInt();
		s.nextLine();
	}
	public String toString(){
		return String.format("Year:%d Month:%d Date:%d Day:%d Time %d:%d:%d", 
				year, month, date, day, hour, minute, second);
	}
	public byte[] getByte(){
		byte[] b = new byte[10];
		b[0] = 'b'; b[1] = 'g';
		b[3] = year;
		b[4] = month;
		b[5] = date;
		b[6] = day;
		b[7] = hour;
		b[8] = minute;
		b[9] = second;
		return b;
	}
}
