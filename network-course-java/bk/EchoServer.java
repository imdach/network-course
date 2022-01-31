import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.concurrent.TimeUnit;

public class EchoServer {

    public static void main(String[] args) throws Exception {

        int port = 9898;
        ServerSocket serverSocket = new ServerSocket(port);

        while (true) {
            Socket clientSocket = serverSocket.accept();
            System.err.println("Accepted connection from client");

            BufferedReader in = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
            PrintWriter out = new PrintWriter(clientSocket.getOutputStream(), true);

            String s;
            int count = 0;
            while ((s = in.readLine()) != null) {
                TimeUnit.SECONDS.sleep(5);
                System.out.println("receive  message " + (count++));
                try {
                    out.println(s);
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }

            System.err.println("Closing connection with client");
            out.close();
            in.close();
            clientSocket.close();
        }
    }
}
