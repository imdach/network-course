import java.io.BufferedReader;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.Socket;
import java.nio.charset.StandardCharsets;

public class EchoClient {
    public static void main(String[] args) throws IOException {

        if (args.length != 2) {
            System.err.println("Usage: java EchoClient <host name> <port number>");
            System.exit(1);
        }

        String hostName = args[0];
        int portNumber = Integer.parseInt(args[1]);

        Socket echoSocket = new Socket(hostName, portNumber);
        DataOutputStream out = new DataOutputStream(echoSocket.getOutputStream());
        DataInputStream in = new DataInputStream(echoSocket.getInputStream());
        BufferedReader stdIn = new BufferedReader(new InputStreamReader(System.in));

        String userInput;
        while ((userInput = stdIn.readLine()) != null) {
            System.out.println(">>>>>");
            out.write((userInput + "\n").getBytes(StandardCharsets.UTF_8));
            out.flush();

            System.out.println("echo: " + respStr);
        }
    }
}
