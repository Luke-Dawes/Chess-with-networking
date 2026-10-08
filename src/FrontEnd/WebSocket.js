const { WebSocketServer } = require("ws");

const wss = new WebSocketServer({port: 9000})

wss.on("connection", (socket) => {
    console.log("browser connected")

    socket.on("message", (data) => {
        console.log("got:", data.toString());
        socket.send("echo: " + data.toString());
    });
}); 

console.log("listening on ws://localhost:9000");