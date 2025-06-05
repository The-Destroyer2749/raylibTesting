#include "raylib.h"
#include "vector"

int main() {
    InitWindow(800, 450, "raylib [core] example - basic window");

    struct Block {
        Vector2 position;
        Color color;
    };

    std::vector<Block> blockGrid;

    int blockGridWidth = 30;
    int blockGridHeight = 20;
    int blockGridSize = blockGridHeight * blockGridWidth;

    blockGrid.reserve(blockGridSize);

    for (int i=0; i<blockGridSize; i++) {
        blockGrid.push_back(Block { // TODO: implement
            static_cast<float>(GetRandomValue(0, GetScreenWidth())),
            static_cast<float>(GetRandomValue(0, GetScreenHeight())),
            RAYWHITE
        }); //
    }

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(DARKGRAY);
        DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}