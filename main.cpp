#include <raylib.h>
#include <vector>
#include <iostream>

struct Tetromino
{
    std::vector<Vector2> positions;
    Texture2D tetroTexture;
    bool canMove;
};

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const int CELL_SIZE = 50;

// kinda stupid, prefer to find a better way but will worry about that later...(we'll see)
static const void DrawBorder(Rectangle gameSpace, Texture2D borderTexture)
{
    for (int column = 0; column < gameSpace.height / CELL_SIZE; column++)
    {
        for (int row = 0; row < gameSpace.width / CELL_SIZE; row++)
        {
            if ((row == 0 || row == 11) || (column == 0 || column == 11))
            {
                Vector2 cell{ gameSpace.x + row * CELL_SIZE, gameSpace.y + column * CELL_SIZE };

                DrawTexture(borderTexture, cell.x, cell.y, WHITE);
            }
        }
    }
}

static const void DrawTetromino(Tetromino tetromino, Vector2 offset)
{
    for (Vector2 position : tetromino.positions)
    {
        Rectangle square{};
        square.x = offset.x + position.x * CELL_SIZE;
        square.y = offset.y + position.y * CELL_SIZE;
        square.width = CELL_SIZE;
        square.height = CELL_SIZE;

        // DrawRectangleRec(square, tetromino.color);
        DrawTexture(tetromino.tetroTexture, square.x, square.y, WHITE);
    }
}

static void UpdateTetromino(Tetromino& tetromino)
{
    for (Vector2& position : tetromino.positions)
    {
        position.y += 1;
    }
}

static void CheckBorderCollision(Tetromino& tetromino)
{
    for (Vector2& position : tetromino.positions)
    {
        if (position.y == 11)
        {
            position.y = 10;
        }
    }
}

int main()
{
	InitWindow(1280, 720, "Tetris Clone");
    SetTargetFPS(5);

    Texture2D blueTexture = LoadTexture("Assets/Tetros/blue_tetro.png");
    Texture2D yellowTexture = LoadTexture("Assets/Tetros/yellow_tetro.png");
    Texture2D magentaTexture = LoadTexture("Assets/Tetros/magenta_tetro.png");
    Texture2D grayTexture = LoadTexture("Assets/Tetros/gray_tetro.png");

    // The calculations to place these elements are arbitrary
    // I just did it this way after some trial and error, and ended up liking this
    Rectangle gameSpace{};
    gameSpace.width = 600;
    gameSpace.height = 600;
    gameSpace.x = (SCREEN_WIDTH - gameSpace.width) / 2;
    gameSpace.y = (SCREEN_HEIGHT - gameSpace.height) / 2;

    Rectangle scoreSpace{};
    scoreSpace.width = 150;
    scoreSpace.height = 300;
    scoreSpace.x = (SCREEN_WIDTH - scoreSpace.width) / 16;
    scoreSpace.y = (SCREEN_HEIGHT - scoreSpace.height) / 2;

    Rectangle nextTetrominoSpace{};
    nextTetrominoSpace.width = 150;
    nextTetrominoSpace.height = 300;
    nextTetrominoSpace.x = gameSpace.x + gameSpace.width + (nextTetrominoSpace.width / 2);
    nextTetrominoSpace.y = (SCREEN_HEIGHT - nextTetrominoSpace.height) / 2;

    Tetromino tetroI{}; // straight line
    tetroI.positions = { Vector2{ 3, 1 }, Vector2{ 4, 1 }, Vector2{ 5, 1 }, Vector2{ 6, 1 } };
    tetroI.tetroTexture = blueTexture;

    Tetromino tetroO; // square, O shaped
    tetroO.positions = { Vector2{ 4, 1 }, Vector2{ 5, 1 }, Vector2{ 4, 2 }, Vector2{ 5, 2 } };
    tetroO.tetroTexture = yellowTexture;

    Tetromino tetroT; // T-shaped, cross
    tetroT.positions = { Vector2{ 3, 1 }, Vector2{ 4, 1 }, Vector2{ 5, 1 }, Vector2{ 4, 2 } };
    tetroT.tetroTexture = magentaTexture;

    std::vector<Tetromino> placedTetrominoes;

    int movement = 0;


    while (!WindowShouldClose())
    {
        //UpdateTetromino(tetroI);
        //CheckBorderCollision(tetroI);

        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawBorder(gameSpace, grayTexture);

        DrawRectangleRec(scoreSpace, BLACK);
        DrawRectangleRec(nextTetrominoSpace, BLACK);

        

        DrawTetromino(tetroT, { gameSpace.x, gameSpace.y });

        EndDrawing();
    }

	CloseWindow();

	return 0;
}