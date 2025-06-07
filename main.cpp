#include "raylib.h"
#include "rlImGui.h"
#include "rlImGuiColors.h"
#include "vector"
#include "imgui.h"
#include <iostream>


struct Block {
    Rectangle rectangle;
    Color color;
    unsigned int health;
};

void recalculateBlockGrid(int blockGridWidth, int blockGridHeight, int& blockGridSize, std::vector<Block>& blockGrid) { // TODO: Fix the bug that prevents the grid from occupying the whole screen in certain window sizes and grid sizes
    blockGridSize = blockGridHeight * blockGridWidth;

    blockGrid.clear();

    // if you static_cast to float for these values it causes no rounding so the blocks end up having spaces around them
    float blockWidth = GetScreenWidth() / blockGridWidth;
    float blockHeight = GetScreenHeight() / blockGridHeight;


    for (int i=0; i<blockGridSize; i++) {
        int randomColorValue = GetRandomValue(160, 255);
        blockGrid.push_back(Block {
            Rectangle {
                static_cast<float>(i % blockGridWidth) * blockWidth,
                static_cast<float>(i / blockGridWidth) * blockHeight,
                static_cast<float>(blockWidth),
                static_cast<float>(blockHeight)
                },
            Color {
                static_cast<unsigned char>(randomColorValue),
                static_cast<unsigned char>(randomColorValue),
                static_cast<unsigned char>(randomColorValue),
                255
            },
            static_cast<unsigned int>(GetRandomValue(0, 10))
        });
    }
}

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 450, "raylibTesting");
    rlImGuiSetup(true);

    std::vector<Block> blockGrid;

    int blockGridMinWidth = 5;
    int blockGridMinHeight = 4;
    int blockGridMaxWidth = 432;
    int blockGridMaxHeight = 240;
    int blockGridWidth = 30;
    int blockGridHeight = 20;
    int blockGridSize = blockGridMaxWidth * blockGridMaxHeight;

    float backgroundColor[3] = {80, 80, 80};

    blockGrid.reserve(blockGridSize);


    int windowWidth = GetScreenWidth();
    int windowHeight = GetScreenHeight();

    recalculateBlockGrid(blockGridWidth, blockGridHeight, blockGridSize, blockGrid);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(ColorFromNormalized({backgroundColor[0], backgroundColor[1], backgroundColor[2], 255.0f}));

        if (windowWidth != GetScreenWidth() || windowHeight != GetScreenHeight()) {
            windowWidth = GetScreenWidth();
            windowHeight = GetScreenHeight();

            blockGridSize = blockGridHeight * blockGridWidth;

            recalculateBlockGrid(blockGridWidth, blockGridHeight, blockGridSize, blockGrid);
            /*
            for (int i=0; i<blockGridSize; i++) {
                std::cout << "X pos: " << static_cast<float>((i % blockGridWidth) * blockWidth) << " | Y pos: " << static_cast<float>((i / blockGridWidth) * blockHeight) << " ][ Width: " << blockWidth << " | Height: " << blockHeight << std::endl;
            }
            std::cout << std::endl << std::endl;
            */
        }

        for (int i=0; i<blockGridSize; i++) {
            if (blockGrid[i].health != 0) {
                DrawRectangle(blockGrid[i].rectangle.x, blockGrid[i].rectangle.y, blockGrid[i].rectangle.width, blockGrid[i].rectangle.height, blockGrid[i].color);
            }
        }

        rlImGuiBegin();

        if (ImGui::Begin("Settings")) {
            ImGui::Text("Block grid width");
            if (ImGui::SliderInt("Block grid width slider", &blockGridWidth, blockGridMinWidth, blockGridMaxWidth)) {
                recalculateBlockGrid(blockGridWidth, blockGridHeight, blockGridSize, blockGrid);
            }

            ImGui::Text("Block grid height");
            if (ImGui::SliderInt("Block grid height slider", &blockGridHeight, blockGridMinHeight, blockGridMaxHeight)) {
                recalculateBlockGrid(blockGridWidth, blockGridHeight, blockGridSize, blockGrid);
            }

            ImGui::Text("Background color");
            ImGui::ColorEdit3("Background color", backgroundColor);
        }
        ImGui::End();



        rlImGuiEnd();
        EndDrawing();
    }

    CloseWindow();
    rlImGuiShutdown();

    return 0;
}