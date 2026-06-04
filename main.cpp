#include "raylib.h"
#include <cstdlib>
#include <ctime>

const int ROWS = 20;
const int COLS = 10;
const int CELL_SIZE = 30;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

const int BOARD_WIDTH = COLS * CELL_SIZE;
const int BOARD_HEIGHT = ROWS * CELL_SIZE;
const int BOARD_X = (SCREEN_WIDTH - BOARD_WIDTH) / 2;
const int BOARD_Y = 0;

struct Piece
{
    int shape[4][4];
    int row;
    int col;
    int type;
};

int tetrominoes[7][4][4] =
{
    {
        {0, 0, 0, 0},
        {1, 1, 1, 1},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },

    {
        {0, 1, 1, 0},
        {0, 1, 1, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },

    {
        {0, 1, 0, 0},
        {1, 1, 1, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },

    {
        {0, 1, 1, 0},
        {1, 1, 0, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },

    {
        {1, 1, 0, 0},
        {0, 1, 1, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },

    {
        {1, 0, 0, 0},
        {1, 1, 1, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    },

    {
        {0, 0, 1, 0},
        {1, 1, 1, 0},
        {0, 0, 0, 0},
        {0, 0, 0, 0}
    }
};

Color pieceColors[8] =
{
    BLACK, RED, BLUE, YELLOW, GREEN, SKYBLUE, WHITE, PURPLE
};

void CopyShape(int destination[4][4], int source[4][4])
{
    for (int r = 0; r < 4; r++)
    {
        for (int c = 0; c < 4; c++)
        {
            destination[r][c] = source[r][c];
        }
    }
}

Piece CreatePiece()
{
    Piece piece;
    int randomType = rand() % 7;
    CopyShape(piece.shape, tetrominoes[randomType]);
    piece.row = 0;
    piece.col = 3;
    piece.type = randomType + 1;
    return piece;
}

bool IsValidPosition(Piece piece, int board[ROWS][COLS])
{
    for (int r = 0; r < 4; r++)
    {
        for (int c = 0; c < 4; c++)
        {
            if (piece.shape[r][c] == 1)
            {
                int boardRow = piece.row + r;
                int boardCol = piece.col + c;

                if (boardCol < 0 || boardCol >= COLS || boardRow >= ROWS)
                {
                    return false;
                }

                if (boardRow >= 0 && board[boardRow][boardCol] != 0)
                {
                    return false;
                }
            }
        }
    }
    return true;
}

Piece RotatePiece(Piece piece)
{
    Piece rotated = piece;

    for (int r = 0; r < 4; r++)
    {
        for (int c = 0; c < 4; c++)
        {
            rotated.shape[r][c] = piece.shape[3 - c][r];
        }
    }

    return rotated;
}

void LockPiece(Piece piece, int board[ROWS][COLS])
{
    for (int r = 0; r < 4; r++)
    {
        for (int c = 0; c < 4; c++)
        {
            if (piece.shape[r][c] == 1)
            {
                int boardRow = piece.row + r;
                int boardCol = piece.col + c;

                if (boardRow >= 0 && boardRow < ROWS && boardCol >= 0 && boardCol < COLS)
                {
                    board[boardRow][boardCol] = piece.type;
                }
            }
        }
    }
}

int ClearLines(int board[ROWS][COLS])
{
    int linesCleared = 0;

    for (int row = ROWS - 1; row >= 0; row--)
    {
        bool fullRow = true;

        for (int col = 0; col < COLS; col++)
        {
            if (board[row][col] == 0)
            {
                fullRow = false;
            }
        }

        if (fullRow)
        {
            linesCleared++;

            for (int moveRow = row; moveRow > 0; moveRow--)
            {
                for (int col = 0; col < COLS; col++)
                {
                    board[moveRow][col] = board[moveRow - 1][col];
                }
            }

            for (int col = 0; col < COLS; col++)
            {
                board[0][col] = 0;
            }

            row++;
        }
    }

    return linesCleared;
}

int ScoreForLines(int linesCleared)
{
    if (linesCleared == 1) return 400;
    if (linesCleared == 2) return 800;
    if (linesCleared == 3) return 1200;
    if (linesCleared == 4) return 1600;
    return 0;
}

void ResetBoard(int board[ROWS][COLS])
{
    for (int r = 0; r < ROWS; r++)
    {
        for (int c = 0; c < COLS; c++)
        {
            board[r][c] = 0;
        }
    }
}

void DrawBoard(int board[ROWS][COLS])
{
    DrawRectangleLines(BOARD_X, BOARD_Y, BOARD_WIDTH, BOARD_HEIGHT, WHITE);

    for (int row = 0; row < ROWS; row++)
    {
        for (int col = 0; col < COLS; col++)
        {
            if (board[row][col] != 0)
            {
                DrawRectangle(
                    BOARD_X + col * CELL_SIZE,
                    BOARD_Y + row * CELL_SIZE,
                    CELL_SIZE,
                    CELL_SIZE,
                    pieceColors[board[row][col]]
                );
            }

            DrawRectangleLines(
                BOARD_X + col * CELL_SIZE,
                BOARD_Y + row * CELL_SIZE,
                CELL_SIZE,
                CELL_SIZE,
                DARKGRAY
            );
        }
    }
}

void DrawPiece(Piece piece)
{
    for (int r = 0; r < 4; r++)
    {
        for (int c = 0; c < 4; c++)
        {
            if (piece.shape[r][c] == 1)
            {
                DrawRectangle(
                    BOARD_X + (piece.col + c) * CELL_SIZE,
                    BOARD_Y + (piece.row + r) * CELL_SIZE,
                    CELL_SIZE,
                    CELL_SIZE,
                    pieceColors[piece.type]
                );

                DrawRectangleLines(
                    BOARD_X + (piece.col + c) * CELL_SIZE,
                    BOARD_Y + (piece.row + r) * CELL_SIZE,
                    CELL_SIZE,
                    CELL_SIZE,
                    BLACK
                );
            }
        }
    }
}

int main()
{
    srand((unsigned int)time(NULL));

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "NickTetris");
    SetTargetFPS(60);

    int board[ROWS][COLS] = {0};

    int score = 0;
    bool gameOver = false;
    int frameCounter = 0;
    int fallSpeed = 30;
    int totalLines = 0;
    int level = 0;

    Piece currentPiece = CreatePiece();

    if (!IsValidPosition(currentPiece, board))
    {
        gameOver = true;
    }

    while (!WindowShouldClose())
    {
        frameCounter++;

        if (IsKeyPressed(KEY_R))
        {
            ResetBoard(board);
            score = 0;
            gameOver = false;
            frameCounter = 0;
            totalLines = 0;
            level = 0;
            fallSpeed = 30;
            currentPiece = CreatePiece();

            if (!IsValidPosition(currentPiece, board))
            {
                gameOver = true;
            }
        }

        if (!gameOver)
        {
            if (IsKeyPressed(KEY_LEFT))
            {
                Piece moved = currentPiece;
                moved.col--;

                if (IsValidPosition(moved, board))
                {
                    currentPiece = moved;
                }
            }

            if (IsKeyPressed(KEY_RIGHT))
            {
                Piece moved = currentPiece;
                moved.col++;

                if (IsValidPosition(moved, board))
                {
                    currentPiece = moved;
                }
            }

            if (IsKeyPressed(KEY_DOWN))
            {
                Piece moved = currentPiece;
                moved.row++;

                if (IsValidPosition(moved, board))
                {
                    currentPiece = moved;
                }
            }

            if (IsKeyPressed(KEY_UP))
            {
                Piece rotated = RotatePiece(currentPiece);

                if (IsValidPosition(rotated, board))
                {
                    currentPiece = rotated;
                }
            }

            if (IsKeyPressed(KEY_SPACE))
            {
                Piece dropped = currentPiece;
                dropped.row++;

                while (IsValidPosition(dropped, board))
                {
                    currentPiece = dropped;
                    dropped.row++;
                }
            }

            if (frameCounter % fallSpeed == 0)
            {
                Piece moved = currentPiece;
                moved.row++;

                if (IsValidPosition(moved, board))
                {
                    currentPiece = moved;
                }
                else
                {
                    LockPiece(currentPiece, board);

                    int lines = ClearLines(board);
                    score += ScoreForLines(lines);
                    totalLines += lines;
                    level = totalLines / 10;

                    if (totalLines % 10 == 0 && lines > 0 && fallSpeed > 1)
                    {
                        fallSpeed -= 1;
                    }

                    currentPiece = CreatePiece();

                    if (!IsValidPosition(currentPiece, board))
                    {
                        gameOver = true;
                    }
                }
            }
        }

        BeginDrawing();
        ClearBackground(BLACK);

        DrawText(TextFormat("Score: %i", score), 20, 20, 30, WHITE);
        DrawText(TextFormat("Level: %i", level), 20, 50, 20, WHITE);
        DrawText(TextFormat("Lines: %i", totalLines), 20, 70, 20, WHITE);
        DrawText("LEFT/RIGHT = move", 20, 90, 20, GRAY);
        DrawText("UP = rotate", 20, 110, 20, GRAY);
        DrawText("SPACE = hard drop", 20, 130, 20, GRAY);
        DrawText("R = restart", 20, 150, 20, GRAY);

        DrawBoard(board);

        if (!gameOver)
        {
            DrawPiece(currentPiece);
        }

        if (gameOver)
        {
            DrawText("GAME OVER",
                     SCREEN_WIDTH / 2 - 140,
                     SCREEN_HEIGHT / 2 - 40,
                     50,
                     RED);

            DrawText(TextFormat("Final Score: %i", score),
                     SCREEN_WIDTH / 2 - 120,
                     SCREEN_HEIGHT / 2 + 20,
                     30,
                     WHITE);

            DrawText("Press R to restart",
                     SCREEN_WIDTH / 2 - 110,
                     SCREEN_HEIGHT / 2 + 60,
                     20,
                     GRAY);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}