package delayack;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.net.InetSocketAddress;
import java.net.Socket;

public class DelayAckClient {
    public static void main(String[] args) throws IOException {
        Socket socket = new Socket();
        socket.setTcpNoDelay(true);
        socket.connect(new InetSocketAddress("10.211.55.3", 8888));
        InputStream inputStream = socket.getInputStream();
        OutputStream outputStream = socket.getOutputStream();
        BufferedReader reader = new BufferedReader(new InputStreamReader(inputStream));
        String head = "hello, ";
        String body = "world\n";

        for (int i = 0; i < 10; i++) {
            long start = System.currentTimeMillis();
            outputStream.write(("#" + i + " " + head).getBytes()); // write
            outputStream.write((body).getBytes()); // write
            String line = reader.readLine(); // read
            System.out.println("RTT: " + (System.currentTimeMillis() - start) + ": " + line);
        }
        inputStream.close();
        outputStream.close();
        socket.close();
    }
}
