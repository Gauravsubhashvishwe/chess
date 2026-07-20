#include <iostream>
#include <vector>
#include <cctype>
#include <cmath>

using namespace std;

class Board {
    vector<vector<char>> board;
    bool isWhiteTurn = true; 
    
    // Variables to track Castling and En Passant states
    bool wKingMoved = false, bKingMoved = false;
    bool wRookLMoved = false, wRookRMoved = false;
    bool bRookLMoved = false, bRookRMoved = false;
    int epX = -1, epY = -1; 

public:
    Board() : board(8, vector<char>(8, ' ')) {
        for(int i = 0; i < 8; i++){
            board[1][i] = 'p'; 
            board[6][i] = 'P'; 
        }

        board[0][0] = board[0][7] = 'r'; 
        board[0][1] = board[0][6] = 'n'; 
        board[0][2] = board[0][5] = 'b'; 
        board[0][3] = 'q';               
        board[0][4] = 'k';               

        board[7][0] = board[7][7] = 'R'; 
        board[7][1] = board[7][6] = 'N'; 
        board[7][2] = board[7][5] = 'B'; 
        board[7][3] = 'Q';               
        board[7][4] = 'K';               
    }

    void display() {
        const string BLACK_TEXT_WHITE_BG = "\033[30;47m";
        const string RESET_COLOR = "\033[0m";

        cout<<"   0  1  2  3  4  5  6  7\n";
        for(int i = 0; i < 8; i++){
            cout<<i<<" ";
            for(int j = 0; j < 8; j++){
                char piece = board[i][j];
                
                if (islower(piece)) {
                    cout<<BLACK_TEXT_WHITE_BG<< " " <<char(toupper(piece))<<" "<<RESET_COLOR;
                } else {
                    cout<<" "<<piece<<" ";
                }
            }
            cout<<endl;
        }
        cout<<(isWhiteTurn ? "\nWhite's turn.\n" : "\nBlack's turn.\n");
    }

    bool check(bool silent = false){
        if(isWhiteTurn){
            int x = -1, y = -1;
            for(int i = 7; i >= 0; i--){
                for(int j = 0; j < 8; j++){
                    if(board[i][j] == 'K'){
                        x = i;
                        y = j;
                    }
                }
            }
            if(x == -1) return false;

            {
                int l = x + 1; int m = y;
                while(l < 8 && board[l][m] == ' ') l++;
                if(l < 8 && (board[l][m] == 'r' || board[l][m] == 'q')){
                    if(!silent) cout<<"White king is under check\n";
                    return true;
                }
                l = x - 1;
                while(l >= 0 && board[l][m] == ' ') l--;
                if(l >= 0 && (board[l][m] == 'r' || board[l][m] == 'q')){
                    if(!silent) cout<<"White king is under check\n";
                    return true;
                }
            }
            {
                int l = x; int m = y + 1;
                while(m < 8 && board[l][m] == ' ') m++;
                if(m < 8 && (board[l][m] == 'r' || board[l][m] == 'q')){
                    if(!silent) cout<<"White king is under check\n";
                    return true;
                }
                m = y - 1;
                while(m >= 0 && board[l][m] == ' ') m--;
                if(m >= 0 && (board[l][m] == 'r' || board[l][m] == 'q')){
                    if(!silent) cout<<"White king is under check\n";
                    return true;
                }
            }
            { 
                int l = x + 1; int m = y + 1;
                while(m < 8 && l < 8 && board[l][m] == ' '){ l++; m++; }
                if(m < 8 && l < 8 && (board[l][m] == 'b' || board[l][m] == 'q')){
                    if(!silent) cout<<"White king is under check\n";
                    return true;
                }
                
                l = x - 1; m = y - 1;
                while(m >= 0 && l >= 0 && board[l][m] == ' '){ l--; m--; }
                if(m >= 0 && l >= 0 && (board[l][m] == 'b' || board[l][m] == 'q')){
                    if(!silent) cout<<"White king is under check\n";
                    return true;
                }
            }
            {
                int l = x + 1; int m = y - 1;
                while(l < 8 && m >= 0 && board[l][m] == ' '){ l++; m--; }
                if(m >= 0 && l < 8 && (board[l][m] == 'b' || board[l][m] == 'q')){
                    if(!silent) cout<<"White king is under check\n";
                    return true;
                }

                l = x - 1; m = y + 1;
                while(m < 8 && l >= 0 && board[l][m] == ' '){ m++; l--; }
                if(l >= 0 && m < 8 && (board[l][m] == 'b' || board[l][m] == 'q')){
                    if(!silent) cout<<"White king is under check\n";
                    return true;
                }
            }
            {
                if(x - 1 >= 0){
                    if(y - 1 >= 0 && board[x - 1][y - 1] == 'p'){
                        if(!silent) cout<<"White king is under check\n";
                        return true;
                    }
                    else if(y + 1 < 8 && board[x - 1][y + 1] == 'p'){
                        if(!silent) cout<<"White king is under check\n";
                        return true;
                    }
                }
            }
            {
                int dx[] = {-2, -2, -1, -1, 1, 1, 2, 2};
                int dy[] = {-1, 1, -2, 2, -2, 2, -1, 1};
                for(int i=0; i<8; i++){
                    int nx = x + dx[i], ny = y + dy[i];
                    if(nx >= 0 && nx < 8 && ny >= 0 && ny < 8 && board[nx][ny] == 'n'){
                        if(!silent) cout<<"White king is under check\n";
                        return true;
                    }
                }
            }
        }
        else { 
            int x = -1, y = -1;
            for(int i = 7; i >= 0; i--){
                for(int j = 0; j < 8; j++){
                    if(board[i][j] == 'k'){
                        x = i; y = j;
                    }
                }
            }
            if(x == -1) return false;
            
            {
                int l = x + 1; int m = y;
                while(l < 8 && board[l][m] == ' ') l++;
                if(l < 8 && (board[l][m] == 'R' || board[l][m] == 'Q')){
                    if(!silent) cout<<"Black king is under check\n";
                    return true;
                }
                l = x - 1;
                while(l >= 0 && board[l][m] == ' ') l--;
                if(l >= 0 && (board[l][m] == 'R' || board[l][m] == 'Q')){
                    if(!silent) cout<<"Black king is under check\n";
                    return true;
                }
            }
            {
                int l = x; int m = y + 1;
                while(m < 8 && board[l][m] == ' ') m++;
                if(m < 8 && (board[l][m] == 'R' || board[l][m] == 'Q')){
                    if(!silent) cout<<"Black king is under check\n";
                    return true;
                }
                m = y - 1;
                while(m >= 0 && board[l][m] == ' ') m--;
                if(m >= 0 && (board[l][m] == 'R' || board[l][m] == 'Q')){
                    if(!silent) cout<<"Black king is under check\n";
                    return true;
                }
            }
            { 
                int l = x + 1; int m = y + 1;
                while(m < 8 && l < 8 && board[l][m] == ' '){ l++; m++; }
                if(m < 8 && l < 8 && (board[l][m] == 'B' || board[l][m] == 'Q')){
                    if(!silent) cout<<"Black king is under check\n";
                    return true;
                }
                
                l = x - 1; m = y - 1;
                while(m >= 0 && l >= 0 && board[l][m] == ' '){ l--; m--; }
                if(m >= 0 && l >= 0 && (board[l][m] == 'B' || board[l][m] == 'Q')){
                    if(!silent) cout<<"Black king is under check\n";
                    return true;
                }
            }
            {
                int l = x + 1; int m = y - 1;
                while(l < 8 && m >= 0 && board[l][m] == ' '){ l++; m--; }
                if(m >= 0 && l < 8 && (board[l][m] == 'B' || board[l][m] == 'Q')){
                    if(!silent) cout<<"Black king is under check\n";
                    return true;
                }

                l = x - 1; m = y + 1;
                while(m < 8 && l >= 0 && board[l][m] == ' '){ m++; l--; }
                if(l >= 0 && m < 8 && (board[l][m] == 'B' || board[l][m] == 'Q')){
                    if(!silent) cout<<"Black king is under check\n";
                    return true;
                }
            }
            {
                if(x + 1 < 8){
                    if(y - 1 >= 0 && board[x + 1][y - 1] == 'P'){
                        if(!silent) cout<<"Black king is under check\n";
                        return true;
                    }
                    else if(y + 1 < 8 && board[x + 1][y + 1] == 'P'){
                        if(!silent) cout<<"Black king is under check\n";
                        return true;
                    }
                }
            }
            {
                int dx[] = {-2, -2, -1, -1, 1, 1, 2, 2};
                int dy[] = {-1, 1, -2, 2, -2, 2, -1, 1};
                for(int i=0; i<8; i++){
                    int nx = x + dx[i], ny = y + dy[i];
                    if(nx >= 0 && nx < 8 && ny >= 0 && ny < 8 && board[nx][ny] == 'N'){
                        if(!silent) cout<<"Black king is under check\n";
                        return true;
                    }
                }
            }
        }
        return false;
    }

    bool isValidpeace(int x1, int y1, bool silent = false){
        if(x1 < 0 || x1 > 7 || y1 < 0 || y1 > 7) return false;
        if(isWhiteTurn){
            if(board[x1][y1] >= 'A' && board[x1][y1] <= 'Z') return true;
            if(!silent) cout<<"It's White's turn! Select a white piece.\n";
            return false;
        }
        else {
            if(board[x1][y1] >= 'a' && board[x1][y1] <= 'z') return true;
            if(!silent) cout<<"It's Black's turn! Select a black piece.\n";
            return false;
        }
    }

    void upgrad_p(int x, int y){
        cout<<"Pawn Promotion! Type q (Queen), n (Knight), b (Bishop), or r (Rook): ";
        bool valid = false;
        do{
            char ch; cin>>ch;
            ch = tolower(ch);
            if(ch == 'q' || ch =='n' || ch == 'b' || ch == 'r'){
                board[x][y] = isWhiteTurn ? char(toupper(ch)) : ch;
                valid = true;
            } else {
                cout << "Invalid choice. Try again: ";
            }
        }while(!valid);
    }

    bool valid_p_m(int x1, int y1, int x2, int y2){
        if(!isWhiteTurn){ 
            if(x2 == x1 + 1 && y1 == y2 && board[x2][y2] == ' ') return true;
            if(x2 == x1 + 1 && abs(y1 - y2) == 1 && board[x2][y2] >= 'A' && board[x2][y2] <= 'Z') return true;
            if(x1 == 1 && x2 == 3 && y1 == y2 && board[2][y1] == ' ' && board[3][y1] == ' ') return true;
            
            // NEWLY ADDED: En Passant Validation for Black
            if(x2 == epX && y2 == epY && x1 == 4 && abs(y1 - y2) == 1 && x2 == 5) return true;
        }
        else{ 
            if(x2 == x1 - 1 && y1 == y2 && board[x2][y2] == ' ') return true;
            if(x2 == x1 - 1 && abs(y1 - y2) == 1 && board[x2][y2] >= 'a' && board[x2][y2] <= 'z') return true;
            if(x1 == 6 && x2 == 4 && y1 == y2 && board[5][y1] == ' ' && board[4][y1] == ' ') return true;
            
            // NEWLY ADDED: En Passant Validation for White
            if(x2 == epX && y2 == epY && x1 == 3 && abs(y1 - y2) == 1 && x2 == 2) return true;
        }
        return false;
    }

    bool valid_r_m(int x1, int y1, int x2, int y2){
        bool white = (board[x1][y1] >= 'A' && board[x1][y1] <= 'Z');
        if(x1 == x2){
            for(int i = min(y1, y2) + 1; i < max(y1, y2); i++ ){
                if(board[x1][i] != ' ') return false;
            }
        }
        else if(y1 == y2){
            for(int i = min(x1,x2) + 1; i < max(x1, x2); i++){
                if(board[i][y1] != ' ') return false;
            }
        } else return false;

        if(white) return (board[x2][y2] == ' ' || islower(board[x2][y2]));
        else return (board[x2][y2] == ' ' || isupper(board[x2][y2]));
    }

    bool valid_n_m(int x1, int y1, int x2, int y2){
        bool white = (board[x1][y1] >= 'A' && board[x1][y1] <= 'Z');
        int dx = abs(x1 - x2);
        int dy = abs(y1 - y2);
        if((dx == 1 && dy == 2) || (dx == 2 && dy == 1)){
            if(white) return (board[x2][y2] == ' ' || islower(board[x2][y2]));
            else return (board[x2][y2] == ' ' || isupper(board[x2][y2]));
        }
        return false;
    }

    bool valid_b_m(int x1, int y1, int x2, int y2){
        bool white = (board[x1][y1] >= 'A' && board[x1][y1] <= 'Z');
        int dx = x2 - x1;
        int dy = y2 - y1;
        if(dx != 0 && abs(dx) == abs(dy)){
            int stepX = dx / abs(dx);
            int stepY = dy / abs(dy);
            for(int i = 1; i < abs(dx); i++){
                if(board[x1 + stepX*i][y1 + stepY*i] != ' ') return false;
            }
            if(white) return (board[x2][y2] == ' ' || islower(board[x2][y2]));
            else return (board[x2][y2] == ' ' || isupper(board[x2][y2]));
        }
        return false;
    }
    
    bool valid_q_m(int x1, int y1, int x2, int y2){
        return (valid_r_m(x1, y1, x2, y2) || valid_b_m(x1, y1, x2, y2));
    }

    bool valid_k_m(int x1, int y1, int x2, int y2){
        bool white = (board[x1][y1] >= 'A' && board[x1][y1] <= 'Z');
        if(abs(x2 - x1) <= 1 && abs(y2 - y1) <= 1){
            if(white) return (board[x2][y2] == ' ' || islower(board[x2][y2]));
            else return (board[x2][y2] == ' ' || isupper(board[x2][y2]));
        }
        
        // NEWLY ADDED: Castling Validation
        int dx = x2 - x1;
        int dy = y2 - y1;
        if(dx == 0 && dy == 2) { 
            if(white && !wKingMoved && !wRookRMoved && board[7][7] == 'R' && board[7][5] == ' ' && board[7][6] == ' ') return true;
            if(!white && !bKingMoved && !bRookRMoved && board[0][7] == 'r' && board[0][5] == ' ' && board[0][6] == ' ') return true;
        }
        if(dx == 0 && dy == -2) { 
            if(white && !wKingMoved && !wRookLMoved && board[7][0] == 'R' && board[7][1] == ' ' && board[7][2] == ' ' && board[7][3] == ' ') return true;
            if(!white && !bKingMoved && !bRookLMoved && board[0][0] == 'r' && board[0][1] == ' ' && board[0][2] == ' ' && board[0][3] == ' ') return true;
        }
        
        return false;
    }
    
    bool isValidMove(int x1, int y1, int x2, int y2){
        if(x1 < 0 || x1 > 7 || x2 < 0 || x2 > 7 || y1 < 0 || y1 > 7 || y2 < 0 || y2 > 7) return false;
        char piece = tolower(board[x1][y1]);
        if(piece == 'p') return valid_p_m(x1, y1, x2, y2);
        if(piece == 'r') return valid_r_m(x1, y1, x2, y2);
        if(piece == 'n') return valid_n_m(x1, y1, x2, y2);
        if(piece == 'b') return valid_b_m(x1, y1, x2, y2);
        if(piece == 'k') return valid_k_m(x1, y1, x2, y2);
        if(piece == 'q') return valid_q_m(x1, y1, x2, y2);
        return false;
    }

    bool move(int x1, int y1, int x2, int y2){
        
        // NEWLY ADDED: Prevent castling out of or through check
        if(tolower(board[x1][y1]) == 'k' && abs(y2 - y1) == 2) {
            if(check(true)) {
                cout<<"\nInvalid Move! Cannot castle out of check.\n\n";
                return false; 
            }
            int step = (y2 > y1) ? 1 : -1;
            board[x1][y1 + step] = board[x1][y1];
            board[x1][y1] = ' ';
            bool intermediateCheck = check(true);
            board[x1][y1] = board[x1][y1 + step];
            board[x1][y1 + step] = ' ';
            if(intermediateCheck) {
                cout<<"\nInvalid Move! Cannot castle through check.\n\n";
                return false;
            }
        }

        char safety = board[x2][y2];
        
        // NEWLY ADDED: Remove captured pawn if En Passant
        char epPawnBackup = ' ';
        if(tolower(board[x1][y1]) == 'p' && x2 == epX && y2 == epY) {
            epPawnBackup = board[x1][y2];
            board[x1][y2] = ' '; 
        }

        board[x2][y2] = board[x1][y1];
        board[x1][y1] = ' ';
        
        // NEWLY ADDED: Move Rook during Castling
        if(tolower(board[x2][y2]) == 'k' && abs(y2 - y1) == 2) {
            if(y2 > y1) { 
                board[x1][y2 - 1] = board[x1][7]; 
                board[x1][7] = ' '; 
            } else { 
                board[x1][y2 + 1] = board[x1][0]; 
                board[x1][0] = ' '; 
            }
        }
        
        if(check(true)){
            cout<<"\nInvalid Move! You cannot expose your own king to check.\n\n";
            board[x1][y1] = board[x2][y2];
            board[x2][y2] = safety;
            
            // NEWLY ADDED: Revert Castling & EP board changes if reverting
            if(epPawnBackup != ' ') board[x1][y2] = epPawnBackup;
            if(tolower(board[x1][y1]) == 'k' && abs(y2 - y1) == 2) {
                if(y2 > y1) { board[x1][7] = board[x1][y2 - 1]; board[x1][y2 - 1] = ' '; }
                else { board[x1][0] = board[x1][y2 + 1]; board[x1][y2 + 1] = ' '; }
            }
            
            return false; 
        }

        if(board[x2][y2] == 'P' && x2 == 0) upgrad_p(x2, y2);
        else if(board[x2][y2] == 'p' && x2 == 7) upgrad_p(x2, y2);
        
        // NEWLY ADDED: Update Castling Tracking Flags
        if(board[x2][y2] == 'K') wKingMoved = true;
        if(board[x2][y2] == 'k') bKingMoved = true;
        if(x1 == 7 && y1 == 0) wRookLMoved = true;
        if(x1 == 7 && y1 == 7) wRookRMoved = true;
        if(x1 == 0 && y1 == 0) bRookLMoved = true;
        if(x1 == 0 && y1 == 7) bRookRMoved = true;
        
        // NEWLY ADDED: Update or Clear En Passant Target
        if(tolower(board[x2][y2]) == 'p' && abs(x2 - x1) == 2) {
            epX = (x1 + x2) / 2;
            epY = y1;
        } else {
            epX = -1;
            epY = -1;
        }
        
        isWhiteTurn ^= 1;
        return true;
    }

    bool hasLegalMoves() {
        for(int x1 = 0; x1 < 8; x1++){
            for(int y1 = 0; y1 < 8; y1++){
                if(isValidpeace(x1, y1, true)){ 
                    for(int x2 = 0; x2 < 8; x2++){
                        for(int y2 = 0; y2 < 8; y2++){
                            if((x1 != x2 || y1 != y2) && isValidMove(x1, y1, x2, y2)){
                                
                                char safety = board[x2][y2];
                                board[x2][y2] = board[x1][y1];
                                board[x1][y1] = ' ';
                                
                                // NEWLY ADDED: Account for En Passant removal during move simulation
                                char epSimBackup = ' ';
                                if (tolower(board[x2][y2]) == 'p' && x2 == epX && y2 == epY) {
                                    epSimBackup = board[x1][y2];
                                    board[x1][y2] = ' ';
                                }
                                
                                bool kingSafe = !check(true);
                                
                                // NEWLY ADDED: Restore En Passant removed piece
                                if (epSimBackup != ' ') board[x1][y2] = epSimBackup;
                                
                                board[x1][y1] = board[x2][y2];
                                board[x2][y2] = safety;
                                
                                if(kingSafe) return true;
                            }
                        }
                    }
                }
            }
        }
        return false;
    }

    bool getTurn() { return isWhiteTurn; }
};

void nextmove(Board &chessBoard) {
    int x1, y1, x2, y2;
    bool moveSuccessful = false;
    
    while (!moveSuccessful) {
        do{
            cout<<"Enter piece coordinates (row col) e.g., '1 0' for a pawn: ";
            if (!(cin >> x1 >> y1)) {
                cin.clear(); cin.ignore(10000, '\n'); continue;
            }
            cout<<"Enter destination coordinates (row col): ";
            if (!(cin>>x2>>y2)) {
                cin.clear(); cin.ignore(10000, '\n'); continue;
            }
        } while(!chessBoard.isValidpeace(x1, y1) || !chessBoard.isValidMove(x1, y1, x2, y2) || (x1 == x2 && y1 == y2));
        
        moveSuccessful = chessBoard.move(x1, y1, x2, y2);
    }
}

int main() {
    Board chessBoard;
    
    while(true){
        std::cout << "\033[2J\033[3J\033[1;1H" << std::flush;
        chessBoard.display(); 
        
        if (!chessBoard.hasLegalMoves()) {
            if (chessBoard.check(true)) { 
                cout << "\nCHECKMATE! " << (chessBoard.getTurn() ? "Black" : "White") << " wins!\n";
            } else {
                cout << "\nSTALEMATE! The game is a draw.\n";
            }
            break;
        }

        chessBoard.check(false); 
        nextmove(chessBoard);
    }
    return 0;
}