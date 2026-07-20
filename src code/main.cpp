#include <iostream>
#include <vector>
#include <cctype>
// NEWLY ADDED: for abs() function used in castling and en passant
#include <cmath>

using namespace std;

class Board {
    vector<vector<char>> board;
    bool isWhiteTurn = true; // Track whose turn it is
    
    // NEWLY ADDED: Variables to track Castling and En Passant states
    bool wKingMoved = false, bKingMoved = false;
    bool wRookLMoved = false, wRookRMoved = false;
    bool bRookLMoved = false, bRookRMoved = false;
    int epX = -1, epY = -1; 
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

    // NEWLY ADDED: silent parameter to check moves quietly
    bool check(bool silent = false){
        if(isWhiteTurn){
            int x,y;
            // NEWLY ADDED: initialize to prevent undefined behavior if king is missing during checks
            x = -1; y = -1;
            for(int i = 7; i >= 0; i--){
                for(int j = 0; j < 8; j++){
                    if(board[i][j] == 'K'){
                        x = i;
                        y = j;
                    }
                }
            }
            // NEWLY ADDED: safety return if king isn't found
            if(x == -1) return false;
            
            {///////// row check ///////////
                int l = x + 1;
                int m = y;
                while(l < 8 && board[l][m] == ' ')l++;
                if(l < 8 && (board[l][m] == 'r' || board[l][m] == 'q')){
                    if(!silent) cout<<"white king is under check\n";
                    return true;
                }
                l = x - 1;
                while(l >= 0 && board[l][m] == ' ')l--;
                if(l >= 0 && (board[l][m] == 'r' || board[l][m] == 'q')){
                    if(!silent) cout<<"white king is under check\n";
                    return true;
                }
            }
            {/////////// column check ///////////
                int l = x;
                int m = y + 1;
                while(m < 8 && board[l][m] == ' ')m++;
                if(m < 8 && (board[l][m] == 'r' || board[l][m] == 'q')){
                    if(!silent) cout<<"white king is under check\n";
                    return true;
                }
                m = y - 1;
                while(m >= 0 && board[l][m] == ' ')m--;
                if(m >= 0 && (board[l][m] == 'r' || board[l][m] == 'q')){
                    if(!silent) cout<<"white king is under check\n";
                    return true;
                }
            }

            { //////////// + diagonal check ////////
                int l = x + 1;
                int m = y + 1;
                while(m < 8 && l < 8 && board[l][m] == ' '){
                    l++;
                    m++;
                }
                if(m < 8 && l < 8 && (board[l][m] == 'b' || board[l][m] == 'q')){
                    if(!silent) cout<<"white king is under check\n";
                    return true;
                }
                
                l = x - 1;
                m = y - 1;
                while(m >= 0 && l >= 0 && board[l][m] == ' '){
                    l--;
                    m--;
                }
                if(m >= 0 && l >= 0 && (board[l][m] == 'b' || board[l][m] == 'q')){
                    if(!silent) cout<<"white king is under check\n";
                    return true;
                }
            }

            {/////////// - diagonal check ////////////////
                int l = x + 1;
                int m = y - 1;
                while(l < 8 && m >= 0 && board[l][m] == ' '){
                    l++;
                    m--;
                }
                if(m >= 0 && l < 8 && (board[l][m] == 'b' || board[l][m] == 'q')){
                    if(!silent) cout<<"white king is under check\n";
                    return true;
                }

                l = x - 1;
                m = y + 1;
                while(m < 8 && l >= 0 && board[l][m] == ' '){
                    m++;
                    l--;
                }
                if(l >= 0 && m < 8 && (board[l][m] == 'b' || board[l][m] == 'q')){
                    if(!silent) cout<<"white king is under check\n";
                    return true;
                }
            }

            {//////// for check by pawn ////////////////////
                if(x - 1 >= 0){
                    if(y - 1 >= 0 && board[x - 1][y - 1] == 'p'){
                        if(!silent) cout<<"white king is under check\n";
                        return true;
                    }
                    else if(y + 1 < 8 && board[x - 1][y + 1] == 'p'){
                        if(!silent) cout<<"white king is under check\n";
                        return true;
                    }
                }
            }

            {///////////// for check by knight ///////////////////////
                if(x - 1 >= 0){
                    if(y - 2 >= 0 && board[x - 1][y - 2] == 'n'){
                        if(!silent) cout<<"white king is under check\n";
                        return true;
                    }
                    if(y + 2 < 8 && board[x - 1][y + 2] == 'n'){
                        if(!silent) cout<<"white king is under check\n";
                        return true;
                    }
                }
                if(x + 1 < 8){
                    if(y - 2 >= 0 && board[x + 1][y - 2] == 'n'){
                        if(!silent) cout<<"white king is under check\n";
                        return true;
                    }
                    if(y + 2 < 8 && board[x + 1][y + 2] == 'n'){
                        if(!silent) cout<<"white king is under check\n";
                        return true;
                    }
                }
                if(y - 1 >= 0){
                    if(x - 2 >= 0 && board[x - 2][y - 1] == 'n'){
                        if(!silent) cout<<"white king is under check\n";
                        return true;
                    }
                    if(x + 2 < 8 && board[x + 2][y - 1] == 'n'){
                        if(!silent) cout<<"white king is under check\n";
                        return true;
                    }
                }
                if(y + 1 < 8){
                    if(x - 2 >= 0 && board[x - 2][y + 1] == 'n'){
                        if(!silent) cout<<"white king is under check\n";
                        return true;
                    }
                    if(x + 2 < 8 && board[x + 2][y + 1] == 'n'){
                        if(!silent) cout<<"white king is under check\n";
                        return true;
                    }
                }
            }
        }
        else { // Black's turn
            int x, y;
            // NEWLY ADDED: initialize to prevent undefined behavior if king is missing during checks
            x = -1; y = -1;
            for(int i = 7; i >= 0; i--){
                for(int j = 0; j < 8; j++){
                    if(board[i][j] == 'k'){
                        x = i;
                        y = j;
                    }
                }
            }
            // NEWLY ADDED: safety return if king isn't found
            if(x == -1) return false;
            
            {///////// row check ///////////
                int l = x + 1;
                int m = y;
                while(l < 8 && board[l][m] == ' ')l++;
                if(l < 8 && (board[l][m] == 'R' || board[l][m] == 'Q')){
                    if(!silent) cout<<"black king is under check\n";
                    return true;
                }
                l = x - 1;
                while(l >= 0 && board[l][m] == ' ')l--;
                if(l >= 0 && (board[l][m] == 'R' || board[l][m] == 'Q')){
                    if(!silent) cout<<"black king is under check\n";
                    return true;
                }
            }
            
            {/////////// column check ///////////
                int l = x;
                int m = y + 1;
                while(m < 8 && board[l][m] == ' ')m++;
                if(m < 8 && (board[l][m] == 'R' || board[l][m] == 'Q')){
                    if(!silent) cout<<"black king is under check\n";
                    return true;
                }
                m = y - 1;
                while(m >= 0 && board[l][m] == ' ')m--;
                if(m >= 0 && (board[l][m] == 'R' || board[l][m] == 'Q')){
                    if(!silent) cout<<"black king is under check\n";
                    return true;
                }
            }

            { //////////// + diagonal check ////////
                int l = x + 1;
                int m = y + 1;
                while(m < 8 && l < 8 && board[l][m] == ' '){
                    l++;
                    m++;
                }
                if(m < 8 && l < 8 && (board[l][m] == 'B' || board[l][m] == 'Q')){
                    if(!silent) cout<<"black king is under check\n";
                    return true;
                }
                
                l = x - 1;
                m = y - 1;
                while(m >= 0 && l >= 0 && board[l][m] == ' '){
                    l--;
                    m--;
                }
                if(m >= 0 && l >= 0 && (board[l][m] == 'B' || board[l][m] == 'Q')){
                    if(!silent) cout<<"black king is under check\n";
                    return true;
                }
            }

            {/////////// - diagonal check ////////////////
                int l = x + 1;
                int m = y - 1;
                while(l < 8 && m >= 0 && board[l][m] == ' '){
                    l++;
                    m--;
                }
                if(m >= 0 && l < 8 && (board[l][m] == 'B' || board[l][m] == 'Q')){
                    if(!silent) cout<<"black king is under check\n";
                    return true;
                }

                l = x - 1;
                m = y + 1;
                while(m < 8 && l >= 0 && board[l][m] == ' '){
                    m++;
                    l--;
                }
                if(l >= 0 && m < 8 && (board[l][m] == 'B' || board[l][m] == 'Q')){
                    if(!silent) cout<<"black king is under check\n";
                    return true;
                }
            }

            {//////// for check by pawn ////////////////////
                // White pawns attack from the row below the black king
                if(x + 1 < 8){
                    if(y - 1 >= 0 && board[x + 1][y - 1] == 'P'){
                        if(!silent) cout<<"black king is under check\n";
                        return true;
                    }
                    else if(y + 1 < 8 && board[x + 1][y + 1] == 'P'){
                        if(!silent) cout<<"black king is under check\n";
                        return true;
                    }
                }
            }

            {///////////// for check by knight ///////////////////////
                if(x - 1 >= 0){
                    if(y - 2 >= 0 && board[x - 1][y - 2] == 'N'){
                        if(!silent) cout<<"black king is under check\n";
                        return true;
                    }
                    if(y + 2 < 8 && board[x - 1][y + 2] == 'N'){
                        if(!silent) cout<<"black king is under check\n";
                        return true;
                    }
                }
                if(x + 1 < 8){
                    if(y - 2 >= 0 && board[x + 1][y - 2] == 'N'){
                        if(!silent) cout<<"black king is under check\n";
                        return true;
                    }
                    if(y + 2 < 8 && board[x + 1][y + 2] == 'N'){
                        if(!silent) cout<<"black king is under check\n";
                        return true;
                    }
                }
                if(y - 1 >= 0){
                    if(x - 2 >= 0 && board[x - 2][y - 1] == 'N'){
                        if(!silent) cout<<"black king is under check\n";
                        return true;
                    }
                    if(x + 2 < 8 && board[x + 2][y - 1] == 'N'){
                        if(!silent) cout<<"black king is under check\n";
                        return true;
                    }
                }
                if(y + 1 < 8){
                    if(x - 2 >= 0 && board[x - 2][y + 1] == 'N'){
                        if(!silent) cout<<"black king is under check\n";
                        return true;
                    }
                    if(x + 2 < 8 && board[x + 2][y + 1] == 'N'){
                        if(!silent) cout<<"black king is under check\n";
                        return true;
                    }
                }
            }
        }
        // NEWLY ADDED: base return statement if no checks found
        return false;
    }

    // NEWLY ADDED: silent parameter to check valid piece quietly
    bool isValidpeace(int x1, int y1, bool silent = false){
        if(x1 < 0 || x1 > 7 || y1 < 0 || y1 > 7)return false;
        if(isWhiteTurn){
            if(board[x1][y1] >= 'A' && board[x1][y1] <= 'Z'){
                return true;
            }
            if(!silent) cout<<"It't whites turn give coordinates of white;"<<endl;
            return false;
        }
        else {
            if(board[x1][y1] >= 'a' && board[x1][y1] <= 'z'){
                return true;
            }
            if(!silent) cout<<"It't Blacks turn give coordinates of black"<<endl;
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
            
            // NEWLY ADDED: En Passant Validation for Black
            if(x2 == epX && y2 == epY && x1 == 4 && (y1 == y2 + 1 || y1 == y2 - 1) && x2 == 5) return true;
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
            
            // NEWLY ADDED: En Passant Validation for White
            if(x2 == epX && y2 == epY && x1 == 3 && (y1 == y2 + 1 || y1 == y2 - 1) && x2 == 2) return true;
        }
        return false;
    }

    bool valid_r_m(int x1, int y1, int x2, int y2){
        bool white = false;
        if(board[x1][y1] >= 'A' && board[x1][y1] <= 'Z')white = true;
        if(x1 == x2){
            for(int i = min(y1, y2) + 1; i < max(y1, y2); i++ ){
                if(board[x1][i] != ' ')return false;
            }
            if(white){
                if(board[x2][y2] == ' ' || (board[x2][y2] >= 'a' && board[x2][y2] <= 'z'))return true;
            }
            else{
                if(board[x2][y2] == ' ' || (board[x2][y2] >= 'A' && board[x2][y2] <= 'Z'))return true;
            }
        }
        else if(y1 == y2){
            for(int i = min(x1,x2) + 1; i < max(x1, x2); i++){
                if(board[i][y1] != ' ')return false;
            }
            if(white){
                if(board[x2][y2] == ' ' || (board[x2][y2] >= 'a' && board[x2][y2] <= 'z'))return true;
            }
            else{
                if(board[x2][y2] == ' ' || (board[x2][y2] >= 'A' && board[x2][y2] <= 'Z'))return true;
            }
        }
        return false;
    }

    bool valid_n_m(int x1, int y1, int x2, int y2){
        bool white = false;
        if(board[x1][y1] >= 'A' && board[x1][y1] <= 'Z')white = true;
        int dx = abs(x1 - x2);
        int dy = abs(y1 - y2);
        if((dx == 1 && dy == 2) || (dx == 2 && dy == 1)){
            if(white){
                if(board[x2][y2] == ' ' || (board[x2][y2] >= 'a' && board[x2][y2] <= 'z'))return true;
            }
            else{
                if(board[x2][y2] == ' ' || (board[x2][y2] >= 'A' && board[x2][y2] <= 'Z'))return true;
            }
        }
        return false;
    }

    bool valid_b_m(int x1, int y1, int x2, int y2){
        bool white = false;
        if(board[x1][y1] >= 'A' && board[x1][y1] <= 'Z')white = true;
        int dx = x2 - x1;
        int dy = y2 - y1;
        int n = abs(dx);
        if( dx != 0 && abs(dx) == abs(dy)){
            dx /= abs(dx);
            dy /= abs(dy);
            for(int i = 1; i < n; i++){
                if(board[x1 + dx*i][y1 + dy*i] != ' ')return false;
            }
            if(white){
                if(board[x2][y2] == ' ' || (board[x2][y2] >= 'a' && board[x2][y2] <= 'z'))return true;
            }
            else{
                if(board[x2][y2] == ' ' || (board[x2][y2] >= 'A' && board[x2][y2] <= 'Z'))return true;
            }
        }
        return false;
    }
    
    bool valid_q_m(int x1, int y1, int x2, int y2){
        return (valid_r_m(x1, y1, x2, y2) || valid_b_m(x1, y1, x2, y2));
    }

    bool valid_k_m(int x1, int y1, int x2, int y2){
        bool white = false;
        if(board[x1][y1] >= 'A' && board[x1][y1] <= 'Z')white = true;
        int dx = x2 - x1;
        int dy = y2 - y1;
        if(abs(dx) <= 1 && abs(dy) <= 1){
            if(white){
                if(board[x2][y2] == ' ' || (board[x2][y2] >= 'a' && board[x2][y2] <= 'z'))return true;
            }
            else{
                if(board[x2][y2] == ' ' || (board[x2][y2] >= 'A' && board[x2][y2] <= 'Z'))return true;
            }
        }
        
        // NEWLY ADDED: Castling Validation
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
        if(x1 < 0 || x1 > 7 || x2 < 0 || x2 > 7 || y1 < 0 || y1 > 7 || y2 < 0 || y2 > 7)return false;
        if(board[x1][y1] == 'p' || board[x1][y1] == 'P'){
            return valid_p_m(x1, y1, x2, y2);
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

    // NEWLY ADDED: return changed to bool to fix turn skipping
    bool move(int x1, int y1, int x2, int y2){
        
        // NEWLY ADDED: Prevent castling out of or through check
        if((board[x1][y1] == 'k' || board[x1][y1] == 'K') && abs(y2 - y1) == 2) {
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

        char safty = board[x2][y2];
        
        // NEWLY ADDED: Remove captured pawn if En Passant
        char epPawnBackup = ' ';
        if((board[x1][y1] == 'p' || board[x1][y1] == 'P') && x2 == epX && y2 == epY) {
            epPawnBackup = board[x1][y2];
            board[x1][y2] = ' '; 
        }

        board[x2][y2] = board[x1][y1];
        board[x1][y1] = ' ';
        
        // NEWLY ADDED: Move Rook during Castling
        if((board[x2][y2] == 'k' || board[x2][y2] == 'K') && abs(y2 - y1) == 2) {
            if(y2 > y1) { 
                board[x1][y2 - 1] = board[x1][7]; 
                board[x1][7] = ' '; 
            } else { 
                board[x1][y2 + 1] = board[x1][0]; 
                board[x1][0] = ' '; 
            }
        }
        
        if(check(true)){
            cout<<"You are exposing your own king. move other peace."<<endl;
            board[x1][y1] = board[x2][y2];
            board[x2][y2] = safty;
            
            // NEWLY ADDED: Revert Castling & EP board changes if reverting
            if(epPawnBackup != ' ') board[x1][y2] = epPawnBackup;
            if((board[x1][y1] == 'k' || board[x1][y1] == 'K') && abs(y2 - y1) == 2) {
                if(y2 > y1) { board[x1][7] = board[x1][y2 - 1]; board[x1][y2 - 1] = ' '; }
                else { board[x1][0] = board[x1][y2 + 1]; board[x1][y2 + 1] = ' '; }
            }
            
            return false;
        }

        if(board[x2][y2] == 'P' && x2 == 0){
            upgrad_p(x2, y2);
        }
        else if(board[x2][y2] == 'p' && x2 == 7){
            upgrad_p(x2, y2);
        }
        
        // NEWLY ADDED: Update Castling Tracking Flags
        if(board[x2][y2] == 'K') wKingMoved = true;
        if(board[x2][y2] == 'k') bKingMoved = true;
        if(x1 == 7 && y1 == 0) wRookLMoved = true;
        if(x1 == 7 && y1 == 7) wRookRMoved = true;
        if(x1 == 0 && y1 == 0) bRookLMoved = true;
        if(x1 == 0 && y1 == 7) bRookRMoved = true;
        
        // NEWLY ADDED: Update or Clear En Passant Target
        if((board[x2][y2] == 'p' || board[x2][y2] == 'P') && abs(x2 - x1) == 2) {
            epX = (x1 + x2) / 2;
            epY = y1;
        } else {
            epX = -1;
            epY = -1;
        }

        isWhiteTurn ^= 1;
        return true;
    }
    
    // NEWLY ADDED: checks if the current player has any moves left
    bool hasLegalMoves() {
        for(int x1 = 0; x1 < 8; x1++){
            for(int y1 = 0; y1 < 8; y1++){
                if(isValidpeace(x1, y1, true)){ 
                    for(int x2 = 0; x2 < 8; x2++){
                        for(int y2 = 0; y2 < 8; y2++){
                            if((x1 != x2 || y1 != y2) && isValidMove(x1, y1, x2, y2)){
                                
                                char safty = board[x2][y2];
                                board[x2][y2] = board[x1][y1];
                                board[x1][y1] = ' ';
                                
                                // NEWLY ADDED: Account for En Passant removal during move simulation
                                char epSimBackup = ' ';
                                if ((board[x2][y2] == 'p' || board[x2][y2] == 'P') && x2 == epX && y2 == epY) {
                                    epSimBackup = board[x1][y2];
                                    board[x1][y2] = ' ';
                                }
                                
                                bool kingSafe = !check(true);
                                
                                // NEWLY ADDED: Restore En Passant removed piece
                                if (epSimBackup != ' ') board[x1][y2] = epSimBackup;
                                
                                board[x1][y1] = board[x2][y2];
                                board[x2][y2] = safty;
                                
                                if(kingSafe) return true;
                            }
                        }
                    }
                }
            }
        }
        return false;
    }

    // NEWLY ADDED: helper function to check turn
    bool getTurn() { return isWhiteTurn; }
};

void nextmove(Board &chessBoard) {
    int x1, y1, x2, y2;
    // NEWLY ADDED: track if move was successful to prevent turn skipping
    bool moveSuccessful = false;
    while (!moveSuccessful) {
        do{
            cout<<"Enter the coordinates of the piece you want to move ex. 0 0 & 0 7 for black rook: ";
            if (!(cin >> x1 >> y1)) {
                cin.clear(); // Clear the error flag
                cin.ignore(10000, '\n'); // Discard bad input
                continue;
            }
            cout<<"Enter the coordinates of the destination: ";
            if (!(cin>>x2>>y2)) {
                cin.clear(); // Clear the error flag
                cin.ignore(10000, '\n'); // Discard bad input
                continue;
            }
        }while(!chessBoard.isValidpeace(x1,y1) || !chessBoard.isValidMove(x1, y1, x2, y2) || (x1 == x2 && y1 == y2));
        // NEWLY ADDED: assign return of move to moveSuccessful
        moveSuccessful = chessBoard.move(x1, y1, x2, y2);
    }
    return;
}

int main() {
    Board chessBoard;
    chessBoard.display();
    while(1){
        nextmove(chessBoard);
        std::cout << "\033[2J\033[3J\033[1;1H" << std::flush;
        chessBoard.display(); 
        
        // NEWLY ADDED: Checkmate & Stalemate logic
        if (!chessBoard.hasLegalMoves()) {
            if (chessBoard.check(true)) { 
                cout << "\nCHECKMATE! " << (chessBoard.getTurn() ? "Black" : "White") << " wins!\n";
            } else {
                cout << "\nSTALEMATE! The game is a draw.\n";
            }
            break;
        }
        
        chessBoard.check();
    }
    return 0;
}