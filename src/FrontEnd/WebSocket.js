const socket = new WebSocket("ws://localhost:9000")

socket.onopen = () => {
    // const message = {
    //     type: "ASK_FOR_POSSIBLE_MOVES",
    //     data: "e2"
    // };
    //socket.send(JSON.stringify(message));

    console.log("connected to java");

}

socket.onmessage = (event) => {
    //const message = JSON.parse(event.data);

    //console.log(message.type);
    //console.log(message.data);

    console.log("Java:" + event.data);
}

socket.onclose = () => {
    console.log("disconnected from Java");
}

socket.onerror = (error) => {
    console.log("websocket error ", error);
}