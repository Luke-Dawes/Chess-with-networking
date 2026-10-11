const socket = new WebSocket("ws://127.0.0.1:8081");
const game_ID = 1; //place holder for the message


const board = document.getElementById("board");
const files = ["a", "b", "c", "d", "e", "f", "g", "h"];

const pieceImages = {
    "K": "Pieces/WhiteKing.svg",
    "Q": "Pieces/WhiteQueen.svg",
    "R": "Pieces/WhiteRook.svg",
    "B": "Pieces/WhiteBishop.svg",
    "N": "Pieces/WhiteKnight.svg",
    "P": "Pieces/WhitePawn.svg",

    "k": "Pieces/BlackKing.svg",
    "q": "Pieces/BlackQueen.svg",
    "r": "Pieces/BlackRook.svg",
    "b": "Pieces/BlackBishop.svg",
    "n": "Pieces/BlackKnight.svg",
    "p": "Pieces/BlackPawn.svg"
};

let boardPosition = {
    "e1": "K",
    "e8": "k",
    "d1": "Q",
    "d8": "q"
};

let sideToMove = "white";
let selected = null;
let targetSqaures = new Set()

const squareButtons = {};

//send a message
function send(type, data) {
    if (socket.readyState != socket.OPEN) return;
    socket.send(JSON.stringify({type, gameID:game_ID, data }))
}

const handlers = {
    POSSIBLE_MOVES(data) {
        if (data.from !== selected) return;

        targetSqaures = new Set(data.moves);
        highlightTargets();
    },

    POSITION(data) {
        boardPosition = data.board;
        sideToMove = data.sideToMove;
        clearSelection();
        render();
    },

    ERROR(data) {
        console.warn("server error:", data)
        clearSelection();
    }

};

socket.addEventListener("message", (event) => {
    let msg;

    try {
        msg = JSON.parse(even.datat);
    }
    catch {
        console.log("unknown / not json format");
        return
    }

    const handler = handlers[msg.type];
    if (handler) handler(msg.data);
    else console.warn("unknown handler / data type");

})

//listner for the board
board.addEventListener("click", (event) => {

    const button = event.target.closest("button");
    if (!button) return;
    onSquareClicked(button.dataset.square);
})

function onSquareClicked(square) {

    //selected square already has valid moves returned and you click on a valid one
    if (selected && targetSqaures.has(square)) {
        send("MAKE_MOVE", {from: selected, to:square});
        clearSelection();
        return;
        //need to add moving pieces in a minute for front end
    }

    //user clicks their own piece but doesnt have any valid moves currently
    if (isOwnPiece(boardPosition[square])) {
        select(square);
        return;
    }

    //user clicks elsewhere on the board
    clearSelection();
}

function isOwnPiece(piece) {
    if (!piece) return false;

    //white is capatlised
    const isWhitePiece = piece === piece.toUpperCase();

    return isWhitePiece === (sideToMove == "white");
}

function select(square) {
    clearSelection();
    selected = square;

    squareButtons[square].classList.add("selected");
    console.log("sent asking for possible moves for square: " + square);
    send("ASK_FOR_POSSIBLE_MOVES", square);
}

function clearSelection() {
    if (selected) squareButtons[selected].classList.remove("selected");
    for (const sq of targetSqaures) squareButtons[sq].classList.remove("target");
    selected = null;
    targetSqaures = new Set()
}

function buildBoard() {
    for (let y = 7; y >= 0; y--) {
        for (let x = 0; x < 8; x++) {
            const button = document.createElement("button");
            const sqaure = `${files[x]}${y + 1}`;
            button.dataset.x = x;
            button.dataset.y = y;
            button.dataset.square = sqaure
            button.classList.add((x + y) % 2 === 1 ? "white-square" : "black-square");
            squareButtons[sqaure] = button;
            board.appendChild(button);

        }
    }
}

function render() {
    for (const [sqaure, button] of Object.entries(squareButtons)) {
        button.replaceChildren();
        const piece = boardPosition[sqaure];
        if (piece) button.appendChild(createImage(piece));
    }
}

function highlightTargets() {
    for (const sq of targetSqaures) squareButtons[sq].classList.add("target");
}


function createImage(piece) {
    const image = document.createElement("img");
    image.src = pieceImages[piece];
    return image;
}

buildBoard();
render();