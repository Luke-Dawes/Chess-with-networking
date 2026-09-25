const board = document.getElementById("board");
const files = ["a", "b", "c", "d", "e", "f", "g", "h"];

const boardPosition = {
    "e1": "K",
    "e8": "k",
    "d1": "Q",
    "d8": "q"
};

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

for (let y = 7; y >= 0; y--) {
    for (let x = 0; x < 8; x++) {
        const button = document.createElement("button");

        button.dataset.x = x;
        button.dataset.y = y;
        button.dataset.square = `${files[x]}${y + 1}`;

        if ((x + y) % 2 === 1) {
            button.classList.add("white-square");
        } else {
            button.classList.add("black-square");
        }

        button.addEventListener("click", () => {
        console.log(button.dataset.square);
        });

        const piece = boardPosition[button.dataset.square];

        if (piece) {
            button.appendChild(createImage(piece))
        }

        board.appendChild(button);
    }
}



function createImage(piece) {
    const image = document.createElement("img");
    image.src = pieceImages[piece];
    return image;
}