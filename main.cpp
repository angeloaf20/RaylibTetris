#include <raylib.h>
#include <vector>
#include <iostream>

struct Tetromino
{
    int id;
    Vector2 movement;
    std::vector<Vector2> positions;
};

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const int CELL_SIZE = 50;

double lastTime = GetTime();
static bool ElapsedTime(double interval)
{
    double currentTime = GetTime();

    if (currentTime - lastTime >= interval)
    {
        lastTime = currentTime;
        return true;
    }
    return false;
}

static const Tetromino GetRandomTetromino(std::vector<Tetromino> tetros)
{
    int randomTetro = GetRandomValue(0, 4);
    return tetros[randomTetro];
}

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

static const void DrawTetromino(Tetromino tetromino, std::vector<Texture2D> textures, Vector2 offset)
{
    for (Vector2 position : tetromino.positions)
    {
        Vector2 offsetPos { offset.x + position.x * CELL_SIZE , offset.y + position.y * CELL_SIZE };

        DrawTexture(textures[tetromino.id], offsetPos.x, offsetPos.y, WHITE);
    }
}

static bool CheckWallCollision(Tetromino& tetromino)
{
    int size = 11;

    for (Vector2& position : tetromino.positions)
    {
        if (position.x <= 0 || position.x >= size)
        {
            return true;
        }
    }

    return false;
}

static bool CheckFloorCollision(Tetromino& tetromino)
{
    int size = 11;

    for (Vector2& position : tetromino.positions)
    {
        if (position.y >= size)
        {
            return true;
        }
    }

    return false;
}

static bool GridCellEmpty(int grid[11][11], Vector2 position)
{
    int posX = (int)position.x;
    int posY = (int)position.y;

    return grid[posX][posY] == 0;
}

static bool TetroCanFit(int grid[11][11], Tetromino& tetromino)
{
    for (Vector2 position : tetromino.positions)
    {
        if (!GridCellEmpty(grid, position))
        {
            return false;
        }
    }
    return true;
}

static void PlaceTetromino(Tetromino& tetromino, int grid[11][11])
{
    for (Vector2 position : tetromino.positions)
    {
        grid[(int)position.x][(int)position.y] = tetromino.id;
    }
}

static void PrintGrid(int grid[11][11])
{
    for (int columns = 0; columns < 11; columns++)
    {
        for (int rows = 0; rows < 11; rows++)
        {
            std::cout << "[" << grid[rows][columns] << "] ";
        }
        std::cout << "\n";
    }

    std::cout << "\n";
}

static const void DrawGrid(int grid[11][11], Vector2 offset, std::vector<Texture2D> textures)
{
    for (int columns = 0; columns < 11; columns++)
    {
        for (int rows = 0; rows < 11; rows++)
        {
            if (grid[rows][columns] == 0)
                continue;

            Vector2 pos = { offset.x + rows * CELL_SIZE, offset.y + columns * CELL_SIZE };

            DrawTexture(textures[grid[rows][columns]], pos.x, pos.y, WHITE);
        }
    }
}

static const void MoveTetroLeft(int grid[11][11], Tetromino& tetromino)
{
    for (Vector2& position : tetromino.positions)
    {
        position.x -= 1;
    }

    if (!TetroCanFit(grid, tetromino) || CheckWallCollision(tetromino))
    {
        for (Vector2& position : tetromino.positions)
        {
            position.x += 1;
        }
    }
}

static const void MoveTetroRight(int grid[11][11], Tetromino& tetromino)
{
    for (Vector2& position : tetromino.positions)
    {
        position.x += 1;
    }

    if (!TetroCanFit(grid, tetromino) || CheckWallCollision(tetromino))
    {
        for (Vector2& position : tetromino.positions)
        {
            position.x -= 1;
        }
    }
}

static const bool MoveTetroDown(int grid[11][11], Tetromino& tetromino, bool keyDown)
{
    for (Vector2& position : tetromino.positions)
    {
        position.y += 1;
    }

    if (!TetroCanFit(grid, tetromino) || CheckFloorCollision(tetromino))
    {
        for (Vector2& position : tetromino.positions)
        {
            position.y -= 1;
        }

        return true;
    }

    return false;
}

static const bool DropTetro(int grid[11][11], Tetromino& tetromino)
{
    if (MoveTetroDown(grid, tetromino, false))
    {
        PlaceTetromino(tetromino, grid);
        return true;
    }

    return false;
}

int main()
{
	InitWindow(1280, 720, "Tetris Clone");
    SetTargetFPS(50);

    std::vector<Texture2D> textures = {
        LoadTexture("Assets/Tetros/gray_tetro.png"),
        LoadTexture("Assets/Tetros/blue_tetro.png"),
        LoadTexture("Assets/Tetros/yellow_tetro.png"),
        LoadTexture("Assets/Tetros/magenta_tetro.png"),
        LoadTexture("Assets/Tetros/red_tetro.png"),
        LoadTexture("Assets/Tetros/green_tetro.png")
    };

    Tetromino tetroI{}; // straight line
    tetroI.id = 1;
    tetroI.positions = { Vector2{ 3, 0 }, Vector2{ 4, 0 }, Vector2{ 5, 0 }, Vector2{ 6, 0 } };
    tetroI.movement = Vector2{ 0, 0 };

    Tetromino tetroO; // square, O shaped
    tetroO.id = 2;
    tetroO.positions = { Vector2{ 4, 0 }, Vector2{ 5, 0 }, Vector2{ 4, 1 }, Vector2{ 5, 1 } };
    tetroO.movement = Vector2{ 0, 0 };

    Tetromino tetroT; // T-shaped, cross
    tetroT.id = 3;
    tetroT.positions = { Vector2{ 3, 0 }, Vector2{ 4, 0 }, Vector2{ 5, 0 }, Vector2{ 4, 1 } };
    tetroT.movement = Vector2{ 0, 0 };

    Tetromino tetroL;
    tetroL.id = 4;
    tetroL.positions = { Vector2{ 3, 0 }, Vector2{ 3, 1 }, Vector2{ 3, 2 }, Vector2{ 4, 2 } };
    tetroL.movement = Vector2{ 0, 0 };

    Tetromino tetroS;
    tetroS.id = 5;
    tetroS.positions = { Vector2{ 3, 1 }, Vector2{ 3, 2 }, Vector2{ 4, 2 }, Vector2{ 4, 3 } };
    tetroS.movement = Vector2{ 0, 0 };

    std::vector<Tetromino> tetros = { tetroI, tetroL, tetroS, tetroT, tetroO };

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
    
    int gameGrid[11][11] = {};

    for (int i = 0; i < 11; i++)
    {
        for (int j = 0; j < 11; j++)
        {
            gameGrid[i][j] = 0;
        }
    }

    Tetromino currentTetromino = GetRandomTetromino(tetros);

    while (!WindowShouldClose())
    {
        /*if (IsKeyPressed(KEY_SPACE))
        {
            PrintGrid(gameGrid);
            currentTetromino = GetRandomTetromino(tetros);
        }*/

        if (IsKeyPressed(KEY_UP))
        {
            for (Vector2 &position : currentTetromino.positions)
            {
                std::cout << "Old position: " << position.x << ", " << position.y << "\n";

                float temp = position.x;
                position.x = position.y;
                position.y = -temp;

                std::cout << "New position: " << position.x << ", " << position.y << "\n";
            }
        }

        if (IsKeyPressed(KEY_LEFT))
        {
            MoveTetroLeft(gameGrid, currentTetromino);
        }

        if (IsKeyPressed(KEY_RIGHT))
        {
            MoveTetroRight(gameGrid, currentTetromino);
        }

        if (IsKeyPressed(KEY_DOWN))
        {
            MoveTetroDown(gameGrid, currentTetromino, true);
        }

        if (ElapsedTime(0.5))
        {
            if (DropTetro(gameGrid, currentTetromino))
            {
                currentTetromino = GetRandomTetromino(tetros);
            }
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawBorder(gameSpace, textures[0]);

        DrawRectangleRec(scoreSpace, BLACK);
        DrawRectangleRec(nextTetrominoSpace, BLACK);
        DrawGrid(gameGrid, { gameSpace.x, gameSpace.y }, textures);
        DrawTetromino(currentTetromino, textures, { gameSpace.x, gameSpace.y });

        EndDrawing();
    }

	CloseWindow();

	return 0;
}