package nagle;

import java.io.OutputStream;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.net.SocketAddress;
import java.util.concurrent.TimeUnit;

public class NagleClient {
    public static void main(String[] args) throws Exception {
        Socket socket = new Socket();
        socket.setTcpNoDelay(true);
        SocketAddress address = new InetSocketAddress("192.168.31.197", 9999);
        socket.connect(address);
        OutputStream output = socket.getOutputStream();
        byte[] request = new byte[10];
        for (int i = 0; i < 5; i++) {
            output.write(request);
        }
        System.out.printf("write done");
        TimeUnit.SECONDS.sleep(1);
        socket.close();
    }
}
