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
};

struct GridPos
{
    int x, y;
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
    std::deque<GridPos> snake;
    Direction currentDir, nextDir;

    Snake()
    {
        snake.push_front({0,0});
        snake.push_front({1,0});
        snake.push_front({2,0});
        snake.push_front({3,0});
    }

    void Update()
    {

    }

    void Draw()
    {
        for (const GridPos& segment : snake)
        {
            DrawCell(segment, GREEN);
        }
    }
};

GridPos food;


int main()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "RaySnake");

    Snake snake;

    while (!WindowShouldClose())
    {
        // Update

        // Drawing
        BeginDrawing();

        DrawGrid();
        snake.Draw();

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
