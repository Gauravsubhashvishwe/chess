#include<iostream>
#include<vector>
using namespace std;

class Board{
    vector<vector<char>> board;
public:
    Board() : board(8, vector<char>(8, '.')) { // Initialize an 8x8 board with empty squares represented by '.'
        for(int i=0; i<8; i++){
            board[1][i] = 'p'; // Pawns
            board[6][i] = 'P'; // Pawns
        }

        board[0][0] = board[0][7] = 'r'; // Rooks
        board[0][1] = board[0][6] = 'n'; // Knights
        board[0][2] = board[0][5] = 'b'; // Bishops
        board[0][3] = 'q'; // Queen
        board[0][4] = 'k'; // King

        board[7][0] = board[7][7] = 'R'; // Rooks
        board[7][1] = board[7][6] = 'N'; // Knights
        board[7][2] = board[7][5] = 'B'; // Bishops
        board[7][3] = 'Q'; // Queen
        board[7][4] = 'K'; // King

    }

    void display(){
        for(int i=0; i<8; i++){
            for(int j=0; j<8; j++){
                cout << board[i][j] << " ";
            }
            cout << endl;
        }
    }
};

int main(){
    Board chessBoard;
    chessBoard.display();
    return 0;
}