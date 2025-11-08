#include <bits/stdc++.h>
using namespace std;

struct Pos {
    int x, y;
};

int n = 8;

bool inside(int x, int y) {
    return x >= 0 && y >= 0 && x < n && y < n;
}

//Legal moves
vector<pair<int,int>> getMoves(char piece) {
    piece = toupper(piece);
    if (piece == 'N') return {{2,1},{1,2},{-1,2},{-2,1},{-2,-1},{-1,-2},{1,-2},{2,-1}};
    if (piece == 'K') return {{1,0},{-1,0},{0,1},{0,-1},{1,1},{-1,1},{1,-1},{-1,-1}};
    return {}; // For R,B,Q handled differently
}

bool canAttack(char piece, int x, int y, int kx, int ky, vector<vector<char>>& board) {
    int dx = kx - x, dy = ky - y;

    piece = toupper(piece);
    if (piece == 'Q' || piece == 'R' || piece == 'B') {
        // direction normalization
        int sx = (dx == 0 ? 0 : dx / abs(dx));
        int sy = (dy == 0 ? 0 : dy / abs(dy));

        // Check valid direction for piece
        if (piece == 'R' && sx != 0 && sy != 0) return false;
        if (piece == 'B' && abs(dx) != abs(dy)) return false;
        if (piece == 'Q' && !((sx == 0 || sy == 0) || (abs(dx) == abs(dy)))) return false;

        // Check line of sight
        int cx = x + sx, cy = y + sy;
        while (inside(cx, cy)) {
            if (cx == kx && cy == ky) return true;
            if (board[cx][cy] != '.') break; // blocked
            cx += sx; cy += sy;
        }
        return false;
    }
    else if (piece == 'N') {
        vector<pair<int,int>> m = {{2,1},{1,2},{-1,2},{-2,1},{-2,-1},{-1,-2},{1,-2},{2,-1}};
        for (auto [a,b] : m)
            if (x+a == kx && y+b == ky) return true;
        return false;
    }
    else if (piece == 'K') {
        return abs(dx) <= 1 && abs(dy) <= 1;
    }
    else if (piece == 'P') {
        // Assuming white pawns move upward (decreasing x)
        return (kx == x-1 && (ky == y-1 || ky == y+1));
    }
    return false;
}

//BFS
int minMovesToCheck(vector<vector<char>>& board, int sx, int sy, Pos king, char piece) {
    queue<pair<Pos,int>> q;
    q.push({{sx,sy},0});
    vector<vector<int>> vis(8, vector<int>(8,0));
    vis[sx][sy] = 1;

    while(!q.empty()){
        auto [pos, d] = q.front(); q.pop();
        int x = pos.x, y = pos.y;

        if (canAttack(piece, x, y, king.x, king.y, board))
            return d;

        // Try possible moves
        if (toupper(piece) == 'N' || toupper(piece) == 'K' || toupper(piece) == 'P') {
            for (auto [dx,dy] : getMoves(piece)) {
                int nx = x + dx, ny = y + dy;
                if (!inside(nx,ny) || vis[nx][ny]) continue;
                if (board[nx][ny] == '.') {
                    vis[nx][ny] = 1;
                    q.push({{nx,ny}, d+1});
                }
            }
        } else {
            // Sliding pieces
            vector<pair<int,int>> dirs;
            if (toupper(piece) == 'R' || toupper(piece) == 'Q')
                dirs.insert(dirs.end(), {{1,0},{-1,0},{0,1},{0,-1}});
            if (toupper(piece) == 'B' || toupper(piece) == 'Q')
                dirs.insert(dirs.end(), {{1,1},{1,-1},{-1,1},{-1,-1}});

            for (auto [dx,dy] : dirs) {
                int nx = x+dx, ny = y+dy;
                while(inside(nx,ny) && board[nx][ny]=='.') {
                    if (!vis[nx][ny]) {
                        vis[nx][ny] = 1;
                        q.push({{nx,ny}, d+1});
                    }
                    nx += dx; ny += dy;
                }
            }
        }
    }
    return -1; // cannot check king
}

int main() {
    vector<vector<char>> board = {
        {'R','N','.','.','.','.','.','.'},
        {'.','.','.','.','.','.','.','.'},
        {'.','.','.','.','.','.','.','.'},
        {'.','.','.','K','.','.','.','.'},
        {'.','.','.','.','.','.','.','.'},
        {'.','.','.','.','.','Q','.','.'},
        {'.','.','.','p','p','.','.','.'},
        {'.','.','b','k','p','b','.','.'}
    };

    Pos blackKing, whiteKing;
    for(int i=0;i<8;i++){
        for(int j=0;j<8;j++){
            if(board[i][j]=='k') blackKing = {i,j};
            if(board[i][j]=='K') whiteKing = {i,j};
        }
    }

    cout << "White pieces attacking Black King:\n";
    for(int i=0;i<8;i++){
        for(int j=0;j<8;j++){
            char p = board[i][j];
            if (isupper(p)) {
                int moves = minMovesToCheck(board, i, j, blackKing, p);
                if(moves!=-1)
                    cout << p << " → " << moves << " moves\n";
                else
                    cout << p << " → cannot check\n";
            }
        }
    }

    return 0;
}
