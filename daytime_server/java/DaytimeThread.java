/*
PA2

Claire Whittington
Peter Hilbert

*/

import java.io.*;
import java.net.Socket;
import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;

// objects of this class work on one request
class DaytimeThread implements Runnable {

    Socket client;
    DataInputStream fromClient;
    DataOutputStream toClient;
    int charFromClient;
    int state = 0;

    // output streams to destination web server and requesting client
    DaytimeThread(Socket client) {
        this.client = client;
    }

    @Override
    public void run() {
        
        // first get the streams
        try {
            this.toClient   = new DataOutputStream(client.getOutputStream());
        } catch (IOException e) {
            e.printStackTrace();
        }

        try {
	    String timeInfo = getTimeInfo();

	    for (int index = 0; index < timeInfo.length(); index++)
	        {
		 toClient.writeByte(timeInfo.charAt(index));
		}
        } catch (IOException e) {
            e.printStackTrace();
        }

        try {
            client.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private String getTimeInfo() {
         DateTimeFormatter dtf = DateTimeFormatter.ofPattern("MM/dd/yyyy hh:mm:ss a*");  
         LocalDateTime now = LocalDateTime.now();  
         return dtf.format(now); 
    }
}
