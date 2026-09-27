package chess.example;

public class Main {
    
    public static void main(String[] args) {
        Server server = new Server(9000);
    
        server.start();
    }
}
