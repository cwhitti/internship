/*
PA2

Claire Whittington
Peter Hilbert

*/

import java.io.IOException;
import java.net.ServerSocket;
import java.util.logging.Level;
import java.util.logging.Logger;

public class DaytimeServer {

    private static ServerSocket serverSocket;
    private static int port;
    private static int SERVER_PORT = 13;


    public DaytimeServer(int port) {
        try {
            DaytimeServer.port = port;
            DaytimeServer.serverSocket = new ServerSocket(port);
        } catch (IOException ex) {
            Logger.getLogger(DaytimeServer.class.getName()).log(Level.SEVERE, null, ex);
            System.err.println("Error starting server on port " + port);
            System.exit(1);
        }

        DaytimeServer.port = port;
    }

    
    public void runServerLoop() throws IOException {

	while (true) {
            System.out.println("Waiting for connections on port #" + port); 
            
            // if we subclass from Thread 
            // (new EchoThread(serverSocket.accept())).start();
            
            // if we have a runnable
            (new Thread(new DaytimeThread(serverSocket.accept()))).start();
            
            /*
            // the above is equivalent with the below.
            // to avoid race conditions, however, we don't do it that way
            Socket clientSocket = serverSocket.accept();
            EchoThread echoThreadRunnable = new EchoThread(clientSocket);
            Thread echoThread = new Thread(echoThreadRunnable);
            echoThread.start();
            */
	}
    }

    public static void main(String args[]) throws Exception {
        // create instance of echo server
        // note that hardcoding the port is bad, here we do it just for simplicity reasons
        DaytimeServer echoServer = new DaytimeServer(SERVER_PORT);

        // fire up server loop
        echoServer.runServerLoop();
    }
}
