package solinger;

import java.io.OutputStream;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.net.SocketAddress;

public class Client {
    private static int PORT = 9999;
    private static String HOST = "10.211.55.3";

    public static void main(String[] args) throws Exception {
        Socket socket = new Socket();
        // 测试#1: 默认设置
//        socket.setSoLinger(false, 0);
        // 测试#2
//         socket.setSoLinger(true, 0);
        // 测试#3
        socket.setSoLinger(true, 1);

        SocketAddress address = new InetSocketAddress(HOST, PORT);
        socket.connect(address);

        OutputStream output = socket.getOutputStream();
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < 1024 * 20; i++) {
            sb.append("hel");
        }
        byte[] request = sb.toString().getBytes("utf-8");
        output.write(request);
        long start = System.currentTimeMillis();
        socket.close();
        long end = System.currentTimeMillis();
        System.out.println("close time cost: " + (end - start));
    }
}
