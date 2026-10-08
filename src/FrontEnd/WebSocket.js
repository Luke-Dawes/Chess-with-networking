const { WebSocketServer } = require("ws");

const wss = new WebSocketServer({
    host: "127.0.0.1",
    port: 9000,
    verifyClient: ({ origin }) => !origin || origin.startsWith("http://localhost"),
});

wss.on("connection", (socket) => {
    console.log("browser connected")

    socket.on("message", (data) => {
        console.log("got:", data.toString());
        socket.send("echo: " + data.toString());
    });
}); 

console.log("listening on ws://localhost:9000");