# A console-based chess game written in C language

My first ever project at university is a chess game written in C, because I was bored. Though, at the time I were writing this game, I only had some basic knowledge about the game, which resulted in plenty of weird game rules :). Oh well. 

## Getting Started

The setup for program is straightforward, just clone the repository and compile `chess.c` file using *clang* or *gcc* compilers, then run the compiled file.

Follow the steps below:

1. Clone the repository:

    `git clone https://github.com/thanhlongb/cchess`

2. Compile the program and rename the compiled file to `game`:

    `clang cchess/chess.c -o game`

3. Run the game:

    `./chess/game`

4. Play :D

## How To Play?

This game will be played by two players: one with blue chess pieces, and red pieces for the other player. The blue player will make the first move. 

Each player will take turn to select their chess piece and enter a coordinate to move the piece to. For example:

Blue select the pawn at [3, 6]: `choose piece [x][y]: 3 6`

Blue move the pawn to [3, 4]: `move to [x][y]: 3 4` 

If the move is valid, the chess piece will be moved and it would be the red player's turn to play.

The players continues until one player got checkmated. 
