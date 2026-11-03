#include <deque>
#include <iostream>
#include "raylib.h"

// Cell Settings
const int CELL_SIZE = 32;
const int GRID_WIDTH = 20;
const int GRID_HEIGHT = 20;

const int SCREEN_WIDTH = GRID_WIDTH * CELL_SIZE;
const int SCREEN_HEIGHT = GRID_HEIGHT * CELL_SIZE;

enum Direction
{
    North,
    East,
    South,
    West
} ;

struct GridPos
{
    int x;
    int y;

    bool operator==(const GridPos& other) const
    {
        return x == other.x && y == other.y;
    }

    bool operator!=(const GridPos& other) const
    {
        return !(*this == other);
    }
};

void DrawGrid()
{
    Color gridColor = DARKGRAY;

    // Vertical lines
    for (int x = 0; x <= GRID_WIDTH; x++)
    {
        DrawLine(x * CELL_SIZE, 0, x * CELL_SIZE, SCREEN_HEIGHT, gridColor);
    }

    // Horizontal lines
    for (int y = 0; y <= GRID_HEIGHT; y++)
    {
        DrawLine(0, y * CELL_SIZE, SCREEN_WIDTH, y * CELL_SIZE, gridColor);
    }
}

void DrawCell(GridPos pos, Color color)
{
    DrawRectangle(pos.x * CELL_SIZE, pos.y * CELL_SIZE, CELL_SIZE, CELL_SIZE, color);
}

struct Snake
{
    std::deque<GridPos> body;
    Direction moveDir;

    float moveTimer = 0.0f;
    float moveInterval = 0.15f; // seconds

    Snake()
    {
        Reset();
    }

    void Reset()
    {
        body.clear();

        moveDir = East;
        body.push_front({0,0});
        body.push_front({1,0});
        body.push_front({2,0});
        body.push_front({3,0});
    }

    void Update(GridPos foodPos)
    {
        moveTimer += GetFrameTime();

        if (moveTimer < moveInterval)
            return;

        moveTimer = 0.0f;

        GridPos newHead = body.front();

        switch (moveDir)
        {
            case North: newHead.y--; break;
            case East:  newHead.x++; break;
            case South: newHead.y++; break;
            case West:  newHead.x--; break;
        }

        // Wall collision
        if (newHead.x < 0 ||
            newHead.x >= GRID_WIDTH ||
            newHead.y < 0 ||
            newHead.y >= GRID_HEIGHT)
        {
            Reset();
            return;
        }

        // Self collision
        for (const auto& segment : body)
        {
            if (newHead == segment)
            {
                Reset();
                return;
            }
        }

        body.push_front(newHead);

        bool ateFood = (newHead == foodPos);

        if (!ateFood)
        {
            body.pop_back();
        }
    }

    void Draw()
    {
        for (const GridPos& segment : body)
        {
            DrawCell(segment, GREEN);
        }
    }

    GridPos GetHead() const
    {
        return body.front();
    }

};

void GetMovementDir(Snake& snake)
{

    if (IsKeyPressed(KEY_W) && snake.moveDir != South)
        snake.moveDir = North;

    if (IsKeyPressed(KEY_D) && snake.moveDir != West)
        snake.moveDir = East;

    if (IsKeyPressed(KEY_S) && snake.moveDir != North)
        snake.moveDir = South;

    if (IsKeyPressed(KEY_A) && snake.moveDir != East)
        snake.moveDir = West;
}



struct Food
{
    GridPos position;

    Food(const std::deque<GridPos>& snakeBody)
    {
        Respawn(snakeBody);
    }

    void Respawn(const std::deque<GridPos>& snakeBody)
    {
        while (true)
        {
            GridPos candidate {
                GetRandomValue(0, GRID_WIDTH - 1),
                GetRandomValue(0, GRID_HEIGHT - 1)
            };

            bool insideSnake = false;

            for (const auto& segment : snakeBody)
            {
                if (segment == candidate)
                {
                    insideSnake = true;
                    break;
                }
            }

            if (!insideSnake)
            {
                position = candidate;
                return;
            }
        }
    }

    void Draw()
    {
        DrawCell(position, RED);
    }
};

int main()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "RaySnake");

    Snake snake;
    Food food(snake.body);

    while (!WindowShouldClose())
    {
        // Update
        GetMovementDir(snake);
        snake.Update(food.position);
        if (snake.GetHead() == food.position)
        {
            food.Respawn(snake.body);
        }

        // Drawing
        BeginDrawing();
        ClearBackground(BLACK);

        DrawGrid();
        food.Draw();
        snake.Draw();

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
