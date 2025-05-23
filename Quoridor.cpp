#include <iostream>
#include <vector>
#include <string>
#include <limits>

using namespace std;

const int BOARD_SIZE = 17;
const int GRID_SIZE = 9;

enum Player { RED, GREEN };
enum WallType { HORIZONTAL, VERTICAL };

struct Position {
    int x;
    int y;
};

class QuoridorGame {
private:
    char board[BOARD_SIZE][BOARD_SIZE];
    Position redPos;
    Position greenPos;
    Player currentPlayer;
    int redWalls;
    int greenWalls;
    bool gameOver;

    void initializeBoard() {
        for (int i = 0; i < BOARD_SIZE; i++) {
            for (int j = 0; j < BOARD_SIZE; j++) {
                if (i % 2 != 0 && j % 2 != 0) {
                    board[i][j] = '+';
                } else {
                    board[i][j] = ' ';
                }
            }
        }
        
        // Set initial positions
        redPos = {8, 0};
        greenPos = {8, 16};
        board[redPos.y][redPos.x] = 'R';
        board[greenPos.y][greenPos.x] = 'G';
        
        // Initialize wall counts
        redWalls = 10;
        greenWalls = 10;
        
        currentPlayer = RED;
        gameOver = false;
    }

    void printBoard() const {
        cout << "   ";
        for (int i = 0; i < BOARD_SIZE; i++) {
            cout << (i % 10); // Column numbers
        }
        cout << endl;
        
        for (int i = 0; i < BOARD_SIZE; i++) {
            cout << (i % 10) << "  "; // Row numbers
            for (int j = 0; j < BOARD_SIZE; j++) {
                cout << board[i][j];
            }
            cout << endl;
        }
    }

    bool isValidMove(const Position& from, const Position& to) const {
        // Check if move is within bounds
        if (to.x < 0 || to.x >= BOARD_SIZE || to.y < 0 || to.y >= BOARD_SIZE) {
            return false;
        }
        
        // Check if target position is empty
        if (board[to.y][to.x] != ' ') {
            return false;
        }
        
        // Check if move is adjacent (2 squares away in grid terms)
        int dx = abs(to.x - from.x);
        int dy = abs(to.y - from.y);
        
        // Normal move (one step in grid)
        if ((dx == 2 && dy == 0) || (dx == 0 && dy == 2)) {
            // Check for walls between positions
            int wallX = (from.x + to.x) / 2;
            int wallY = (from.y + to.y) / 2;
            
            if (dx == 2) { // Horizontal move
                if (board[wallY][wallX] == '-') {
                    return false;
                }
            } else { // Vertical move
                if (board[wallY][wallX] == '|') {
                    return false;
                }
            }
            return true;
        }
        
        // TODO: Add jump move logic (when opponent is in the way)
        
        return false;
    }

    bool isValidWallPlacement(int gridX, int gridY, WallType type) const {
        // Convert grid coordinates to board coordinates
        int x = gridX * 2;
        int y = gridY * 2;
        
        if (type == HORIZONTAL) {
            // Check if wall fits on board
            if (gridX < 0 || gridX >= GRID_SIZE - 1 || gridY <= 0 || gridY >= GRID_SIZE) {
                return false;
            }
            
            // Check if space is available
            if (board[y-1][x] == '-' || board[y-1][x+2] == '-' ||
                (x > 0 && board[y-1][x-1] == '|') || 
                board[y-1][x+1] == '|') {
                return false;
            }
        } else { // VERTICAL
            // Check if wall fits on board
            if (gridX <= 0 || gridX >= GRID_SIZE || gridY < 0 || gridY >= GRID_SIZE - 1) {
                return false;
            }
            
            // Check if space is available
            if (board[y][x-1] == '|' || board[y+2][x-1] == '|' ||
                (y > 0 && board[y-1][x-1] == '-') || 
                board[y+1][x-1] == '-') {
                return false;
            }
        }
        
        // TODO: Check if wall doesn't block all paths to goal
        
        return true;
    }

    void placeWall(int gridX, int gridY, WallType type) {
        int x = gridX * 2;
        int y = gridY * 2;
        
        if (type == HORIZONTAL) {
            board[y-1][x] = '-';
            board[y-1][x+1] = '-';
            board[y-1][x+2] = '-';
        } else { // VERTICAL
            board[y][x-1] = '|';
            board[y+1][x-1] = '|';
            board[y+2][x-1] = '|';
        }
        
        if (currentPlayer == RED) {
            redWalls--;
        } else {
            greenWalls--;
        }
    }

    void movePlayer(const Position& newPos) {
        if (currentPlayer == RED) {
            board[redPos.y][redPos.x] = ' ';
            redPos = newPos;
            board[redPos.y][redPos.x] = 'R';
            
            // Check for win condition
            if (redPos.y == BOARD_SIZE - 1) {
                gameOver = true;
            }
        } else {
            board[greenPos.y][greenPos.x] = ' ';
            greenPos = newPos;
            board[greenPos.y][greenPos.x] = 'G';
            
            // Check for win condition
            if (greenPos.y == 0) {
                gameOver = true;
            }
        }
    }

    void switchPlayer() {
        currentPlayer = (currentPlayer == RED) ? GREEN : RED;
    }

public:
    QuoridorGame() {
        initializeBoard();
    }

    void displayWelcome() const {
        cout << "***** Hello and welcome to the game of QUORIDOR! *****" << endl << endl;
        cout << "Please choose from the following:" << endl << endl;
        cout << "1) Start game" << endl << "2) Exit" << endl;
    }

    void displayOptions() const {
        cout << "Player " << (currentPlayer == RED ? "R" : "G") << "'s turn. You have " 
             << (currentPlayer == RED ? redWalls : greenWalls) << " walls left." << endl;
        cout << "Options:" << endl;
        cout << "1) Move your pawn" << endl;
        cout << "2) Place a wall" << endl;
        cout << "3) Quit game" << endl;
    }

    void play() {
        while (!gameOver) {
            printBoard();
            displayOptions();
            
            int choice;
            while (!(cin >> choice) || choice < 1 || choice > 3) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input. Please enter 1, 2, or 3: ";
            }
            
            if (choice == 3) {
                cout << "Game ended by player." << endl;
                return;
            }
            
            if (choice == 1) { // Move pawn
                Position currentPos = (currentPlayer == RED) ? redPos : greenPos;
                Position newPos = currentPos;
                
                cout << "Enter move direction (w/a/s/d): ";
                char direction;
                cin >> direction;
                
                switch (tolower(direction)) {
                    case 'w': newPos.y -= 2; break;
                    case 'a': newPos.x -= 2; break;
                    case 's': newPos.y += 2; break;
                    case 'd': newPos.x += 2; break;
                    default:
                        cout << "Invalid direction!" << endl;
                        continue;
                }
                
                if (isValidMove(currentPos, newPos)) {
                    movePlayer(newPos);
                    switchPlayer();
                } else {
                    cout << "Invalid move!" << endl;
                }
            } 
            else if (choice == 2) { // Place wall
                if ((currentPlayer == RED && redWalls <= 0) || 
                    (currentPlayer == GREEN && greenWalls <= 0)) {
                    cout << "You have no walls left!" << endl;
                    continue;
                }
                
                cout << "Enter wall position (x y) and type (h/v): ";
                int x, y;
                char type;
                cin >> x >> y >> type;
                
                WallType wallType = (tolower(type) == 'h' ? HORIZONTAL : VERTICAL);
                
                if (isValidWallPlacement(x, y, wallType)) {
                    placeWall(x, y, wallType);
                    switchPlayer();
                } else {
                    cout << "Invalid wall placement!" << endl;
                }
            }
        }
        
        printBoard();
        cout << "Player " << (currentPlayer == RED ? "G" : "R") << " wins! Congratulations!" << endl;
    }
};

int main() {
    QuoridorGame game;
    
    game.displayWelcome();
    int choice;
    cin >> choice;
    
    if (choice == 1) {
        game.play();
    } else {
        cout << "Goodbye!" << endl;
    }
    
    return 0;
}