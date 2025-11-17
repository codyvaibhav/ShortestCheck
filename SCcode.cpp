#include <bits/stdc++.h>
using namespace std;

// Board limits
bool inside(int x, int y) {
    return x >= 0 && x < 8 && y >= 0 && y < 8;
}

// Base Class for Chess Pieces
class ChessPiece {
public:
    virtual vector<pair<int,int>> getMoves(int x, int y) = 0;
    virtual ~ChessPiece() {}
};

// ----------------- Derived Classes ------------------

// 1. King
class King : public ChessPiece {
public:
    vector<pair<int,int>> getMoves(int x, int y) override {
        vector<pair<int,int>> moves;
        int kx[] = {-1,-1,-1,0,0,1,1,1};
        int ky[] = {-1,0,1,-1,1,-1,0,1};

        for (int i = 0; i < 8; i++) {
            int nx = x + kx[i], ny = y + ky[i];
            if (inside(nx, ny)) moves.push_back({nx, ny});
        }
        return moves;
    }
};

// 2. Knight
class Knight : public ChessPiece {
public:
    vector<pair<int,int>> getMoves(int x, int y) override {
        vector<pair<int,int>> moves;
        int kx[] = {2,2,1,1,-1,-1,-2,-2};
        int ky[] = {1,-1,2,-2,2,-2,1,-1};

        for (int i = 0; i < 8; i++) {
            int nx = x + kx[i], ny = y + ky[i];
            if (inside(nx, ny)) moves.push_back({nx, ny});
        }
        return moves;
    }
};

// 3. Rook
class Rook : public ChessPiece {
public:
    vector<pair<int,int>> getMoves(int x, int y) override {
        vector<pair<int,int>> moves;
        // Horizontal and Vertical
        int dx[] = {1,-1,0,0};
        int dy[] = {0,0,1,-1};

        for (int d = 0; d < 4; d++) {
            int nx = x, ny = y;
            while (true) {
                nx += dx[d];
                ny += dy[d];
                if (!inside(nx, ny)) break;
                moves.push_back({nx, ny});
            }
        }
        return moves;
    }
};

// 4. Bishop
class Bishop : public ChessPiece {
public:
    vector<pair<int,int>> getMoves(int x, int y) override {
        vector<pair<int,int>> moves;
        int dx[] = {1,1,-1,-1};
        int dy[] = {1,-1,1,-1};

        for (int d = 0; d < 4; d++) {
            int nx = x, ny = y;
            while (true) {
                nx += dx[d];
                ny += dy[d];
                if (!inside(nx, ny)) break;
                moves.push_back({nx, ny});
            }
        }
        return moves;
    }
};

// 5. Queen (Combination of Rook + Bishop)
class Queen : public ChessPiece {
public:
    vector<pair<int,int>> getMoves(int x, int y) override {
        vector<pair<int,int>> moves;

        // Rook moves
        int dx1[] = {1,-1,0,0};
        int dy1[] = {0,0,1,-1};
        for (int d = 0; d < 4; d++) {
            int nx = x, ny = y;
            while (true) {
                nx += dx1[d];
                ny += dy1[d];
                if (!inside(nx, ny)) break;
                moves.push_back({nx, ny});
            }
        }

        // Bishop moves
        int dx2[] = {1,1,-1,-1};
        int dy2[] = {1,-1,1,-1};
        for (int d = 0; d < 4; d++) {
            int nx = x, ny = y;
            while (true) {
                nx += dx2[d];
                ny += dy2[d];
                if (!inside(nx, ny)) break;
                moves.push_back({nx, ny});
            }
        }

        return moves;
    }
};

// 6. Pawn (Assuming white pawn moving upward)
class Pawn : public ChessPiece {
public:
    vector<pair<int,int>> getMoves(int x, int y) override {
        vector<pair<int,int>> moves;

        // Pawn attack moves (diagonal)
        int nx1 = x - 1, ny1 = y - 1;
        int nx2 = x - 1, ny2 = y + 1;

        if (inside(nx1, ny1)) moves.push_back({nx1, ny1});
        if (inside(nx2, ny2)) moves.push_back({nx2, ny2});

        return moves;
    }
};

// ---------------- BFS Shortest Path -------------------

int shortestCheck(ChessPiece* piece, pair<int,int> start, pair<int,int> king) {
    queue<pair<pair<int,int>, int>> q;
    bool visited[8][8] = {false};

    q.push({start, 0});
    visited[start.first][start.second] = true;

    while (!q.empty()) {
        auto [pos, dist] = q.front(); q.pop();
        int x = pos.first, y = pos.second;

        if (pos == king) return dist;

        for (auto m : piece->getMoves(x, y)) {
            int nx = m.first, ny = m.second;
            if (!visited[nx][ny]) {
                visited[nx][ny] = true;
                q.push({{nx, ny}, dist + 1});
            }
        }
    }
    return -1;  // theoretically never happens
}

// ---------------- Main Driver -------------------

int main() {
    string pieceType;
    int sx, sy, kx, ky;
    
    cin >> pieceType >> sx >> sy >> kx >> ky;

    ChessPiece* piece;

    if (pieceType == "king") piece = new King();
    else if (pieceType == "queen") piece = new Queen();
    else if (pieceType == "rook") piece = new Rook();
    else if (pieceType == "bishop") piece = new Bishop();
    else if (pieceType == "knight") piece = new Knight();
    else if (pieceType == "pawn") piece = new Pawn();
    else {
        cout << "Invalid piece type\n";
        return 0;
    }

    cout << shortestCheck(piece, {sx, sy}, {kx, ky}) << endl;

    delete piece;
    return 0;
}
