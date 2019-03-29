#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const short AXIS  = 8;
const char EMPTY  = ' ';
const char PAWN   = 'p';
const char ROCK   = 'r';
const char KNIGHT = 'n';
const char BISHOP = 'b';
const char KING   = 'k';
const char QUEEN  = 'q';
const char BLACK_PARTY = 'b';
const char WHITE_PARTY = 'w';

char chess_board[AXIS][AXIS];
char party_board[AXIS][AXIS];
char winner = EMPTY;

void init_chess_board();
void print_chess_board();
void move_piece(int from[2], int to[2]);
void move(int from[2], int to[2]);
void to_unicode_symbol(char *chess_piece, char text);
short is_empty_space(int coordinate[2]);
short is_valid_piece_move(int from[2], int to[2]);
short is_other_party(int from[2], int to[2]);
short is_out_bound(int coordinate[2]);

// test functions
void test_piece(char piece, char party, int x, int y);

int main(void) {
    int from[2], to[2];

    init_chess_board();
    // test_piece(PAWN, WHITE_PARTY, 4, 4);

    while (winner == EMPTY) {
        print_chess_board();
        printf("choose piece [x][y]: ");
        scanf("%d %d", &from[1], &from[0]);
        printf("move to      [x][y]: ");
        scanf("%d %d", &to[1], &to[0]);
        move_piece(from, to);
    }

    print_chess_board();
    printf("Game Over!");

    return 0;
}

void test_piece(char piece, char party, int x, int y) {
    int from[2] = {x, y};
    chess_board[x][y] = piece;
    party_board[x][y] = party;
    for (int column = 0; column < AXIS; ++column) {
        for (int row = 0; row < AXIS; ++row) {
            int to[2] = {column, row};
            if (is_valid_piece_move(from, to)) {
                chess_board[column][row] = 'x';
            }
        }
    }
}

void init_chess_board() {
    const char PLACEMENT_ORDER[AXIS] = {ROCK, KNIGHT, BISHOP,
                                        QUEEN, KING,
                                        BISHOP, KNIGHT, ROCK};

    for (int column = 0; column < AXIS; ++column) {
        for (int row = 0; row < AXIS; ++row) {
            switch (column)
            {
            case 0:
                chess_board[column][row] = PLACEMENT_ORDER[row];
                party_board[column][row] = BLACK_PARTY;
                break;
            case 1:
                chess_board[column][row] = PAWN;
                party_board[column][row] = BLACK_PARTY;
                break;
            case AXIS - 1:
                chess_board[column][row] = PLACEMENT_ORDER[row];
                party_board[column][row] = WHITE_PARTY;
                break;
            case AXIS - 2:
                chess_board[column][row] = PAWN;
                party_board[column][row] = WHITE_PARTY;
                break;
            default:
                chess_board[column][row] = EMPTY;
                party_board[column][row] = EMPTY;
                break;
            }
            if (column == 0 || column == AXIS - 1) {
                chess_board[column][row] = PLACEMENT_ORDER[row];
            } else if (column == 1 || column == AXIS - 2) {
                chess_board[column][row] = PAWN;
            } else {
                chess_board[column][row] = EMPTY;
            }
        }
    }
}

void print_chess_board() {
    short const COLOR_BLACK = 31;
    short const COLOR_WHITE = 34;
    short const COLOR_EMPTY = 36;
    short const COLOR_NORMAL = 0;
    short const BGCOLOR_BLACK = 40;
    short const BGCOLOR_WHITE = 47;

    short text_color;
    short background_color;
    char chess_piece[4];

    // printf("\n");
    printf("\x1B[%dmx ", COLOR_NORMAL);
    for (int row = 0; row < AXIS; ++row) {
        printf(" %d ", row);
    }
    printf("\n");
    for (int column = 0; column < AXIS; ++column) {
        printf("%d ", column);
        background_color = (column % 2 == 0) ? BGCOLOR_WHITE : BGCOLOR_BLACK;
        for (int row = 0; row < AXIS; ++row) {
            if (party_board[column][row] == BLACK_PARTY) {
                text_color = COLOR_BLACK;
            } else if (party_board[column][row] == WHITE_PARTY) {
                text_color = COLOR_WHITE;
            } else if (party_board[column][row] == EMPTY) {
                text_color = COLOR_EMPTY;
            } else {
                text_color = COLOR_NORMAL;
            }
            to_unicode_symbol(chess_piece, chess_board[column][row]);
            printf("\x1B[%d;%dm %s \x1B[0m", background_color,
                                             text_color,
                                             chess_piece);
            if (background_color == BGCOLOR_BLACK) {
                background_color = BGCOLOR_WHITE;
            } else {
                background_color = BGCOLOR_BLACK;
            }
        }
        printf("\x1B[%dm\n", COLOR_NORMAL);
    }
    // printf("\n");
    
}

void move_piece(int from[2], int to[2]) {
    if (is_out_bound(from) || is_out_bound(to)) {
        printf("> out of bound!\n");
        return;
    }
    if (!is_valid_piece_move(from, to)) {
        printf("> not valid move!\n");
        return;
    }
    if (!is_other_party(from, to)) {
        printf("> can't kill chess piece in the same party!\n");
        return;
    }
    if (chess_board[to[0]][to[1]] == KING) {
        if (party_board[to[0]][to[1]] == BLACK_PARTY) {
            winner = WHITE_PARTY;
        } else if (party_board[to[0]][to[1]] == WHITE_PARTY) {
            winner = BLACK_PARTY;
        } else {
            // this doesn't supposed to happen
            printf("oops!");
        }
    }
    move(from, to);
}

void to_unicode_symbol(char *chess_piece, char text) {
    if (text == QUEEN) { strcpy(chess_piece, "♕"); }
    else if (text == KING) { strcpy(chess_piece, "♔"); }
    else if (text == BISHOP) { strcpy(chess_piece, "♗"); }
    else if (text == KNIGHT) { strcpy(chess_piece, "♘"); }
    else if (text == ROCK) { strcpy(chess_piece, "♖"); }
    else if (text == PAWN) { strcpy(chess_piece, "♙"); }
    else { strcpy(chess_piece, " "); }
}

void move(int from[2], int to[2]) {
    chess_board[to[0]][to[1]] = chess_board[from[0]][from[1]];
    party_board[to[0]][to[1]] = party_board[from[0]][from[1]];
    chess_board[from[0]][from[1]] = EMPTY;
    party_board[from[0]][from[1]] = EMPTY;
}

short is_other_party(int from[2], int to[2]) {
    if (party_board[from[0]][from[1]] == party_board[to[0]][to[1]]) {
        return 0;
    }
    return 1;
}

short is_empty_space(int coordinate[2]) {
    if (party_board[coordinate[0]][coordinate[1]] == EMPTY) {
        return 1;
    }
    return 0;
}

short is_valid_piece_move(int from[2], int to[2]) {
    char chess_piece = chess_board[from[0]][from[1]];
    char chess_piece_party = party_board[from[0]][from[1]];
    if (chess_piece == KNIGHT) {
        if ((abs(from[0] - to[0]) == 2 || abs(from[1] - to[1]) == 2) &&
                (abs(from[1] - to[1]) == 1 || abs(from[0] - to[0]) == 1)) {
            return 1;
        } else {
            return 0;
        }
    } else if (chess_piece == PAWN) {
        if (chess_piece_party == BLACK_PARTY) {
            if (to[0] - from[0] == 1) {
                if (to[1] == from[1] && is_empty_space(to)) {
                    return 1;
                } else if (abs(to[0] - from[0]) == 1 &&
                            !is_empty_space(to) &&
                            is_other_party(from, to)) {
                    return 1;
                } else {
                    return 0;
                }
            } else if ((to[0] - from[0] == 2) && (from[0] == 1)) {
                // 1 is black party pawn's inital position
                if (to[1] == from[1] && is_empty_space(to)) {
                    return 1;
                } else {
                    return 0;
                }
            } else {
                return 0;
            }
        } else if (chess_piece_party == WHITE_PARTY) {
            if (to[0] - from[0] == -1) {
                if (to[1] == from[1] && is_empty_space(to)) {
                    return 1;
                } else if (abs(to[0] - from[0]) == 1 &&
                            !is_empty_space(to) &&
                            is_other_party(from, to)) {
                    return 1;
                } else {
                    return 0;
                }
            } else if ((to[0] - from[0] == -2) && (from[0] == AXIS - 2)) {
                // AXIS - 2 is white party pawn's inital position
                if (to[1] == from[1] && is_empty_space(to)) {
                    return 1;
                } else {
                    return 0;
                }
            } else {
                return 0;
            }
        } else {
            return 0;
        }
    } else if (chess_piece == KING) {
        if (abs(to[0] - from[0]) <= 1 &&
                abs(to[1] - from[1]) <= 1 &&
                !(to[0] == from[0] && to[1] == from[1])) {
            return 1;
        } else {
            return 0;
        }
    } else if (chess_piece == ROCK) {
        int test_to[2];
        if (to[0] == from[0] && to[1] != from[1]) {
            test_to[0] = to[0];
            for (int i = 1; i < abs(to[1] - from[1]); ++i) {
                if (to[1] > from[1]) {
                    test_to[1] = from[1] + i;
                } else if (to[1] < from[1]) {
                    test_to[1] = from[1] - i;
                }
                if (!is_empty_space(test_to)) return 0;
            }
            return 1;
        } else if (to[1] == from[1] && to[0] != from[0]) {
            test_to[1] = to[1];
            for (int i = 1; i < abs(to[0] - from[0]); ++i) {
                if (to[0] > from[0]) {
                    test_to[0] = from[0] + i;
                } else if (to[0] < from[0]) {
                    test_to[0] = from[0] - i;
                }
                if (!is_empty_space(test_to)) return 0;
            }
            return 1;
        } else {
            return 0;
        }
    } else if (chess_piece == BISHOP) {
        int test_to[2];
        if (abs(to[0] - from[0]) == abs(to[1] - from[1]) &&
                !(to[0] == from[0] && to[1] == from[1])) {
            for (int i = 1; i < abs(to[0] - from[0]); ++i) {
                if (to[0] < from[0]) {
                    test_to[0] = from[0] - i;
                } else if (to[0] > from[0]) {
                    test_to[0] = from[0] + i;
                }
                if (to[1] < from[1]) {
                    test_to[1] = from[1] - i;
                } else if (to[1] > from[1]) {
                    test_to[1] = from[1] + i;
                }
                if (!is_empty_space(test_to)) return 0;
            }
            return 1;
        } else {
            return 0;
        }
    } else if (chess_piece == QUEEN) { //probably gonna have bug
        int test_to[2];
        if (abs(to[0] - from[0]) == abs(to[1] - from[1]) &&
                !(to[0] == from[0] && to[1] == from[1])) {
            for (int i = 1; i < abs(to[0] - from[0]); ++i) {
                if (to[0] < from[0]) {
                    test_to[0] = from[0] - i;
                } else if (to[0] > from[0]) {
                    test_to[0] = from[0] + i;
                }
                if (to[1] < from[1]) {
                    test_to[1] = from[1] - i;
                } else if (to[1] > from[1]) {
                    test_to[1] = from[1] + i;
                }
                if (!is_empty_space(test_to)) return 0;
            }
            return 1;
        } else if (to[0] == from[0] && to[1] != from[1]) {
            test_to[0] = to[0];
            for (int i = 1; i < abs(to[1] - from[1]); ++i) {
                if (to[1] > from[1]) {
                    test_to[1] = from[1] + i;
                } else if (to[1] < from[1]) {
                    test_to[1] = from[1] - i;
                }
                if (!is_empty_space(test_to)) return 0;
            }
            return 1;
        } else if (to[1] == from[1] && to[0] != from[0]) {
            test_to[1] = to[1];
            for (int i = 1; i < abs(to[0] - from[0]); ++i) {
                if (to[0] > from[0]) {
                    test_to[0] = from[0] + i;
                } else if (to[0] < from[0]) {
                    test_to[0] = from[0] - i;
                }
                if (!is_empty_space(test_to)) return 0;
            }
            return 1;
        }
    } else {
        return 0;
    }
    return 0;
}

short is_out_bound(int coordinate[2]) {
    if (coordinate[0] >= AXIS || coordinate[0] < 0) return 1;
    if (coordinate[1] >= AXIS || coordinate[1] < 0) return 1;
    return 0;
}
