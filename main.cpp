#include "raylib.h"
#include "ray"
#include "vector"

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 450, "raylib [core] example - basic window");

    struct Block {
        Rectangle rectangle;
        Color color;
    };

    std::vector<Block> blockGrid;

    int blockGridWidth = 30;
    int blockGridHeight = 20;
    int blockGridSize = blockGridHeight * blockGridWidth;

    blockGrid.reserve(blockGridSize);

    for (int i=0; i<blockGridSize; i++) {
        blockGrid.push_back(Block { // TODO: implement actual proper grid creation alignment using modulo and other functions
            static_cast<float>(GetRandomValue(0, GetScreenWidth())),
            static_cast<float>(GetRandomValue(GetScreenHeight() / 2, GetScreenHeight())),
            static_cast<float>(GetScreenWidth()) / blockGridWidth,
            static_cast<float>(GetScreenHeight()) / blockGridHeight,
            RAYWHITE
        }); //
    }

    int windowWidth = GetScreenWidth();
    int windowHeight = GetScreenHeight();

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(DARKGRAY);

        if (windowWidth != GetScreenWidth() || windowHeight != GetScreenHeight()) {
            windowWidth = GetScreenWidth();
            windowHeight = GetScreenHeight();
        }

        for (int i=0; i<blockGridSize; i++) {
            DrawRectangle(blockGrid[i].rectangle.x, blockGrid[i].rectangle.y, blockGrid[i].rectangle.width, blockGrid[i].rectangle.height, blockGrid[i].color);
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}