package chess.example;

//import com.google.gson.Gson;

import java.net.InetSocketAddress;

import org.java_websocket.handshake.ClientHandshake;
import org.java_websocket.server.WebSocketServer;
import org.java_websocket.WebSocket;

//java connecting to the JS frontend

public class Server extends WebSocketServer {
    
    public Server(int port) {
        super(new InetSocketAddress(port));
    }

    @Override 
    public void onOpen(WebSocket conn, ClientHandshake handshake) {
        System.out.println("Client connected" + conn.getRemoteSocketAddress());

        conn.send("hello from java");
    }

    @Override 
    public void onClose(WebSocket conn, int code, String reason, boolean remote) {
        System.out.println("Client discconected");
    }

    @Override 
    public void onMessage(WebSocket conn, String message) {
        System.out.println("recieved" + message);

        conn.send("Java recieved " + message);
    }

    @Override 
    public void onError(WebSocket conn, Exception err) {
        err.printStackTrace();
        System.out.println("Error:" + err.getMessage());
    }

    @Override 
    public void onStart() {
        System.out.println("Web socket started");
    }


    static class Message {
        MessageType type; 
        String data;

        Message(MessageType type, String data) {
            this.type = type;
            this.data = data;

        }

    }

}