package rst;

import java.io.OutputStream;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.net.SocketAddress;
import java.nio.charset.StandardCharsets;
import java.util.concurrent.TimeUnit;

public class CloseWhenRcvBuffHasData {
//    private static int PORT = 8000;
    private static int PORT = 9000;
    private static String HOST = "192.168.31.197";

    public static void main(String[] args) throws Exception {
        Socket socket = new Socket();
        SocketAddress address = new InetSocketAddress(HOST, PORT);
        socket.connect(address);

        OutputStream output = socket.getOutputStream();
        StringBuilder sb = new StringBuilder();
        sb.append("hello");
        System.out.println(sb.length());
        String req =
                "POST /route2 HTTP/1.1\r\n" + "Content-Type: text/plain; charset=utf-8\r\n" +
                        "Content-Length: " + sb.length() + "\r\n" + "Host: 192.168.31" + ".197" + ":8000\r\n" + "Connection: close\r\n" + "User-Agent: Paw/3.3.6\r\n" + "\r" + "\n" + sb;

        System.out.println(req);
        byte[] request = req.getBytes(StandardCharsets.UTF_8);
        output.write(request);
        TimeUnit.SECONDS.sleep(1);
        long start = System.currentTimeMillis();
        socket.close();
        long end = System.currentTimeMillis();
        System.out.println("close time cost: " + (end - start));
        System.in.read();
    }
}
