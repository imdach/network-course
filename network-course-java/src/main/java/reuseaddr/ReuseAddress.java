package reuseaddr;

import java.io.IOException;
import java.io.OutputStream;
import java.net.InetSocketAddress;
import java.net.ServerSocket;
import java.net.Socket;

public class ReuseAddress {
    public static void main(String[] args) throws IOException, InterruptedException {
        ServerSocket serverSocket = new ServerSocket();
        serverSocket.setReuseAddress(false);
        serverSocket.bind(new InetSocketAddress(8080));
        System.out.println("reuse address: " + serverSocket.getReuseAddress());
        while (true) {
            Socket socket = serverSocket.accept();
            System.out.println("incoming socket..");
            OutputStream out = socket.getOutputStream();
            out.write("Hello\n".getBytes());
            out.close();
            socket.close();
        }
    }
}
