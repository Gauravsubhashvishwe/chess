#include <iostream>
#include <vector>
#include <cctype>

using namespace std;

class Board {
    vector<vector<char>> board;

public:
    Board() : board(8, vector<char>(8, ' ')) {
        for(int i = 0; i < 8; i++){
            board[1][i] = 'p'; // Black Pawns
            board[6][i] = 'P'; // White Pawns
        }

        board[0][0] = board[0][7] = 'r'; // Black Rooks
        board[0][1] = board[0][6] = 'n'; // Black Knights
        board[0][2] = board[0][5] = 'b'; // Black Bishops
        board[0][3] = 'q';               // Black Queen
        board[0][4] = 'k';               // Black King

        board[7][0] = board[7][7] = 'R'; // White Rooks
        board[7][1] = board[7][6] = 'N'; // White Knights
        board[7][2] = board[7][5] = 'B'; // White Bishops
        board[7][3] = 'Q';               // White Queen
        board[7][4] = 'K';               // White King
    }

    void display() {
        // ANSI escape codes
        const string BLACK_TEXT_WHITE_BG = "\033[30;47m";
        const string RESET_COLOR = "\033[0m";

        for(int i = 0; i < 8; i++){
            for(int j = 0; j < 8; j++){
                char piece = board[i][j];
                
                // If the piece is lowercase, it belongs to the black player
                if (islower(piece)) {
                    cout << BLACK_TEXT_WHITE_BG << piece  << " " << RESET_COLOR;
                } else {
                    // Print empty spaces and white pieces normally
                    cout << piece << " ";
                }
            }
            cout << endl;
        }
    }
};

int main() {
    Board chessBoard;
    chessBoard.display();
    return 0;
}