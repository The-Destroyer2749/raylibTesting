#include "raylib.h"
#include "rlImGui.h"
#include "rlImGuiColors.h"
#include "vector"
#include "imgui.h"
#include <cmath>
#include <numeric>
#include <iostream>


struct Block {
    Rectangle rectangle;
    Color color;
    unsigned int health;
};

void recalculateBlockGrid(int blockGridWidth, int blockGridHeight, int& blockGridSize, std::vector<Block>& blockGrid) {
    blockGridSize = blockGridHeight * blockGridWidth;

    blockGrid.clear();

    // if you static_cast to float for these values it causes no rounding so the blocks end up having spaces around them
    float blockWidth = ceil(static_cast<float>(GetScreenWidth()) / static_cast<float>(blockGridWidth));
    float blockHeight = ceil(static_cast<float>(GetScreenHeight()) / static_cast<float>(blockGridHeight));


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

int getNumberOfDigits(int num) {
    if (num != 0) {
        return log10(num);
    }
    else {
        return 1;
    }
}

char* findAspectRatio(int width, int height) { // Since I'm working with char* I just decided to use a mix of c and c++ as I'm most comfortable with c when I'm dealing with them
    int gcd = std::gcd(width, height);
    // std::cout << "gcd: " << gcd << std::endl;

    int numOfDigits;
    int resultStringLength;

    numOfDigits = getNumberOfDigits(width / gcd);

    char* temp = (char*)malloc(numOfDigits * sizeof(char));

    resultStringLength = getNumberOfDigits(numOfDigits + (height / gcd) + 1);
    char* result = (char*)malloc(numOfDigits * sizeof(char)); // I don't free the result char* as it is what's returned for the function

    sprintf(result, "%d", width / gcd);
    sprintf(temp, "%d", height / gcd);

    result = strcat(result, ":");
    result = strcat(result, temp);

    free(temp);
    return result;
}

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 450, "raylibTesting");
    rlImGuiSetup(true);
    SetExitKey(0);

    std::vector<Block> blockGrid;

    int blockGridMinWidth = 16;
    int blockGridMinHeight = 9;
    int blockGridMaxWidth = 320;
    int blockGridMaxHeight = 180;
    int blockGridWidth = 30;
    int blockGridHeight = 20;
    int blockGridSize = blockGridMaxWidth * blockGridMaxHeight;

    float backgroundColor[3] = {80.0f / 255.0f, 80.0f / 255.0f, 80.0f / 255.0f};

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

            ImGui::Text("Total Blocks: %d", blockGridSize);
            ImGui::Text("Grid aspect ratio: %s", findAspectRatio(blockGridWidth, blockGridHeight));

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