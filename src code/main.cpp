#include <iostream>
#include <vector>
#include <cctype>

using namespace std;

class Board {
    vector<vector<char>> board;
    bool isWhiteTurn = true; // Track whose turn it is
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
                    cout << BLACK_TEXT_WHITE_BG<< " " << char(toupper(piece)) << " " << RESET_COLOR;
                } else {
                    // Print empty spaces and white pieces normally
                    cout << " " << piece << " ";
                }
            }
            cout << endl;
        }
    }

    bool isValidpeace(int x1, int y1){
        if(isWhiteTurn){
            if(board[x1][y1] >= 'A' && board[x1][y1] <= 'Z'){
                return true;
            }
            cout<<"It't whites turn give coordinates of white;"<<endl;
            return false;
        }
        else {
            if(board[x1][y1] >= 'a' && board[x1][y1] <= 'z'){
                return true;
            }
            cout<<"It't Blacks turn give coordinates of black"<<endl;
            return false;
        }
    }

    void upgrad_p(int x, int y){
        cout<<"You Have Option to upgrad your Pawn. Type q-Queen, n-night, b-bishop, r-rook other charachter for exit: ";
        bool valid = false;
        do{
            char ch; cin>>ch;
            if(board[x][y] >= 'A' && board[x][y] <= 'Z'){
                if(ch == 'q' || ch =='n' || ch == 'b' || ch == 'r'){
                    board[x][y] = char(toupper(ch));
                    valid = true;
                }
            }
            else{
                if(ch == 'q' || ch =='n' || ch == 'b' || ch == 'r'){
                    board[x][y] = ch;
                    valid = true;
                }
            }
        }while(!valid);
        return;
    }

    bool valid_p_m(int x1, int y1, int x2, int y2){
        if(board[x1][y1] >= 'a' && board[x1][y1] <= 'z'){
            if(x1 == 1){
                if(x2 == 2){
                    if(y1 == y2 && board[x2][y2] == ' '){
                        return true;
                    }
                    else if((y1 == y2 - 1 || y1 == y2 + 1) && (board[x2][y2] >= 'A' && board[x2][y2] <= 'Z')){
                        return true;
                    }
                }
                else if(x2 == 3 && y1 == y2 && board[2][y1] == ' ' && board[3][y1] == ' '){
                    return true;
                }
            }
            // else if(x1 == 4 && (x2 == x1 + 1 && ((y1 == y2 + 1 || y1 == y2 - 1)))){
            //     if(board[4][y2] == 'P' && board[5][y2] == ' ') return true;
            // }
            else{
                if(x2 == x1 + 1 && ((y1 == y2 && board[x2][y2] == ' ') || ((y1 == y2 + 1 || y1 == y2 - 1) && (board[x2][y2] >= 'A' && board[x2][y2] <= 'Z'))))return true;
            }
        }
        else{
            if(x1 == 6){
                if(x2 == 5){
                    if(y1 == y2 && board[x2][y2] == ' '){
                        return true;
                    }
                    else if((y1 == y2 - 1 || y1 == y2 + 1) && (board[x2][y2] >= 'a' && board[x2][y2] <= 'z')){
                        return true;
                    }
                }
                else if(x2 == 4 && y1 == y2 && board[5][y1] == ' ' && board[4][y1] == ' '){
                    return true;
                }
            }
            // else if(x1 == 3 && (x2 == x1 - 1 && ((y1 == y2 + 1 || y1 == y2 - 1)))){
            //     if(board[3][y2] == 'p' && board[2][y2] == ' ') return true;
            // }
            else{
                if(x2 == x1 - 1 && ((y1 == y2 && board[x2][y2] == ' ') || ((y1 == y2 + 1 || y1 == y2 - 1) && (board[x2][y2] >= 'a' && board[x2][y2] <= 'z'))))return true;
            }
        }
        return false;
    }

    bool valid_r_m(int x1, int y1, int x2, int y2){
        return true;
    }
    bool valid_n_m(int x1, int y1, int x2, int y2){
        return true;
    }
    bool valid_b_m(int x1, int y1, int x2, int y2){
        return true;
    }
    bool valid_k_m(int x1, int y1, int x2, int y2){
        return true;
    }
    bool valid_q_m(int x1, int y1, int x2, int y2){
        return true;
    }
    bool isValidMove(int x1, int y1, int x2, int y2){
        if(board[x1][y1] == 'p' || board[x1][y1] == 'P'){
            if(valid_p_m(x1, y1, x2, y2)){
                if(x2 == 0 || x2 == 7){
                    upgrad_p(x1, y1);
                }
                return true;
            }

        }
        else if(board[x1][y1] == 'r' || board[x1][y1] == 'R'){
            return valid_r_m(x1, y1, x2, y2);
        }
        else if(board[x1][y1] == 'n' || board[x1][y1] == 'N'){
            return valid_n_m(x1, y1, x2, y2);
        }
        else if(board[x1][y1] == 'b' || board[x1][y1] == 'B'){
            return valid_b_m(x1, y1, x2, y2);
        }
        else if(board[x1][y1] == 'k' || board[x1][y1] == 'K'){
            return valid_k_m(x1, y1, x2, y2);
        }
        else if(board[x1][y1] == 'q' || board[x1][y1] == 'Q'){
            return valid_q_m(x1, y1, x2, y2);
        }
        return false;
    }

    void move(int x1, int y1, int x2, int y2){
        board[x2][y2] = board[x1][y1];
        board[x1][y1] = ' ';
        isWhiteTurn ^= 1;
        return;
    }
};

void nextmove(Board* chessBoard) {
    int x1, y1, x2, y2;
    do{
        cout<<"Enter the coordinates of the piece you want to move ex. 0 0 & 0 7 for black rook: ";
        cin>>x1>>y1;
        cout<<"Enter the coordinates of the destination: ";
        cin>>x2>>y2;
    }while(!chessBoard->isValidpeace(x1,y1) || !chessBoard->isValidMove(x1, y1, x2, y2));
    chessBoard->move(x1, y1, x2, y2);
    return;
}

int main() {
    Board* chessBoard = new Board();
    chessBoard->display();
    while(1){
        nextmove(chessBoard);
        std::cout << "\033[2J\033[3J\033[1;1H" << std::flush;
        chessBoard->display(); 
    }
    delete chessBoard;
    return 0;
}