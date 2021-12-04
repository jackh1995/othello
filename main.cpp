#include <iostream>
#include "color.h"
#include "ncurses.h"
using namespace std;

enum block_type {
    EMPTY,
    CROSS,
    CIRCLE
};

class Board {
public:
    Board() : board(nullptr), X(0), Y(0), round(1), printColumnIndices(true), printRowIndices(true) {}
    Board(int _x, int _y) : X(_x), Y(_y), round(1), printColumnIndices(true), printRowIndices(true) {
        board = new block_type*[X];
        for (int i=0; i!= X; ++i) {
            board[i] = new block_type[Y]();
        }
        for (int i=0; i!=X; ++i) {
            for (int j=0; j!=Y; ++j) {
                board[i][j] = EMPTY;
            }
        }
    }
    
    void printBoard() {
        if (board) {

            if (printColumnIndices) {
                cout << "   ";
                for (int i=0; i!=Y; ++i) {
                    if (i % 10 == 0)
                        cout << YELLOW <<  i/10;
                    else
                        cout << ' ';
                } cout << endl;
                cout << "   ";
                for (int i=0; i!=Y; ++i) {
                    cout << i%10;
                } cout  << RESET << endl;
            }
            
                
            for (int i=0; i!=X; ++i) {
                
                if (printRowIndices) {
                    cout << YELLOW;
                    if (i % 10 == 0)
                        cout << i / 10;
                    else
                        cout << ' ';
                    cout << i % 10 << ' ';
                    cout << RESET;
                }
                for (int j=0; j!=Y; ++j) {
                    if (board[i][j] == CIRCLE) 
                        cout << GREEN << 'O' << RESET;
                    else if (board[i][j] == CROSS)
                        cout << RED << 'X' << RESET;
                    else
                        cout << j % 10;
                }
                cout << endl;
            }
        }
    }
    ~Board() {
        if (board) {
            for (int i=0; i!=X; ++i) {
                delete[] board[i];
            }
            delete[] board;
        }
    }

    void play(int x, int y) {
        if (board[x][y] == EMPTY) {
            cout << YELLOW << "ROUND: " << round << RESET << endl;
            if (round % 2)
                board[x][y] = CIRCLE;    
            else
                board[x][y] = CROSS;
            updateBoard(x, y);
        } else {
            cout << "Error" << endl;
        }
    }

    int findLeft(int x, int y) {
        if (y == 0)
            return -1;
        bool hasSeen(false);
        for (int i=y-1; i>=0; --i) {
            if (board[x][i] != EMPTY && board[x][i] != board[x][y]) {
                hasSeen = true;
            } else if (board[x][i] == board[x][y] && hasSeen) {
                return i;
            } else
                break;
        }
        return -1;
    }

    int findRight(int x, int y) {
        if (y == Y-1)
            return -1;
        bool hasSeen(false);
        for (int i=y+1; i<=Y-1; ++i) {
            if (board[x][i] != EMPTY && board[x][i] != board[x][y]) {
                hasSeen = true;
            } else if (board[x][i] == board[x][y] && hasSeen) {
                return i;
            } else
                break;
        }
        return -1;
    }

    int findUp(int x, int y) {
        if (x == 0)
            return -1;
        bool hasSeen(false);
        for (int i=x-1; i>=0; --i) {
            if (board[i][y] != EMPTY && board[i][y] != board[x][y]) {
                hasSeen = true;
            } else if (board[i][y] == board[x][y] && hasSeen) {
                return i;
            } else
                break;
        }
        return -1;
    }

    int findDown(int x, int y) {
        if (x == X-1)
            return -1;
        bool hasSeen(false);
        for (int i=x+1; i<=X-1; ++i) {
            if (board[i][y] != EMPTY && board[i][y] != board[x][y]) {
                hasSeen = true;
            } else if (board[i][y] == board[x][y] && hasSeen) {
                return i;
            } else
                break;
        }
        return -1;
    }

    void updateLeft(int x, int y) {
        int guard = findLeft(x, y);
        if (guard >= 0) {
            for (int i=y-1; i>=guard; --i) {
                board[x][i] = board[x][y];
            }
        }
    }

    void updateRight(int x, int y) {
        int guard = findRight(x, y);
        if (guard > 0) {
            for (int i=y+1; i<=guard; ++i) {
                board[x][i] = board[x][y];
            }
        }
    }

    void updateUp(int x, int y) {
        int guard = findUp(x, y);
        if (guard >= 0) {
            for (int i=x-1; i>=guard; --i) {
                board[i][y] = board[x][y];
            }
        }
    }

    void updateDown(int x, int y) {
        int guard = findDown(x, y);
        if (guard >= 0) {
            for (int i=x+1; i<=guard; ++i) {
                board[i][y] = board[x][y];
            }
        }
    }

    void updateBoard(int x, int y) {
        updateLeft(x, y);
        updateRight(x, y);
        updateUp(x, y);
        updateDown(x, y);
        ++round;
    }

    bool announce(void) {
        if (round % 2)
            cout << "Play " << GREEN << 'O' << RESET << " at ";
        else
            cout << "Play " << RED << 'X' << RESET << " at ";
        return true;
    }

    void start() {
        int x; int y;
        cout << YELLOW << "GAME STARTED" << RESET << endl;
        printBoard();
        while (announce() && cin >> x >> y){
            cout << endl;
            play(x, y);
            printBoard();
        }
    }

private:
    int X;
    int Y;
    block_type** board;
    int round;
    bool printColumnIndices, printRowIndices;
};

int main() {
    Board game(20, 30);
    game.start();
    return 0;
}