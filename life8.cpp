#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <thread>
#include <chrono>
#include <cstdlib>

int countNeighbors(const std::vector<std::vector<int>>& grid, int r, int c, int rows, int cols) {
    int count = 0;
    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            if (i == 0 && j == 0) continue;
            
            int ni = r + i;
            int nj = c + j;
            
            if (ni >= 0 && ni < rows && nj >= 0 && nj < cols) {
                if (grid[ni][nj] == 1) {
                    count++;
                }
            }
        }
    }
    return count;
}

void printGrid(const std::vector<std::vector<int>>& grid, int rows, int cols) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (grid[i][j] == 1) {
                std::cout << "* ";
            } else {
                std::cout << "- ";
            }
        }
        std::cout << "\n";
    }
}

int main(int argc, char* argv[]) {
    std::string filename = "input.txt";
    if (argc > 1) {
        filename = argv[1];
    }
//если это ктото это читает то знайте что на 8 попытке я помераю T_T
    std::ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        std::cerr << "Ошибка: не удалось открыть файл " << filename << "\n";
        return 1;
    }

    int rows, cols;
    if (!(inputFile >> rows >> cols)) {
        std::cerr << "Ошибка: неверный формат файла (не указаны размеры).\n";
        return 1;
    }

    std::vector<std::vector<int>> grid(rows, std::vector<int>(cols, 0));

    int r, c;
    while (inputFile >> r >> c) {
        if (r >= 0 && r < rows && c >= 0 && c < cols) {
            grid[r][c] = 1;
        }
    }
    inputFile.close();

    int generation = 1;
    bool gameRunning = true;

    while (gameRunning) {

        int liveCount = 0;
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                if (grid[i][j] == 1) liveCount++;
            }
        }

        std::cout << "Generation: " << generation << "\n";
        std::cout << "Alive: " << liveCount << "\n\n";
        printGrid(grid, rows, cols);
        std::cout << "\n";

        if (liveCount == 0) {
            std::cout << "No alive things. Game over.\n";
            gameRunning = false;
            break;
        }

        std::vector<std::vector<int>> nextGrid(rows, std::vector<int>(cols, 0));
        bool stateChanged = false;

        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                int neighbors = countNeighbors(grid, i, j, rows, cols);
                
                if (grid[i][j] == 1) {
                    if (neighbors == 2 || neighbors == 3) {
                        nextGrid[i][j] = 1;
                    } else {
                        nextGrid[i][j] = 0;
                        stateChanged = true;
                    }
                } else {
                    if (neighbors == 3) {
                        nextGrid[i][j] = 1;
                        stateChanged = true;
                    } else {
                        nextGrid[i][j] = 0;
                    }
                }
            }
        }

        if (!stateChanged) {
            std::cout << "The world has stagnated. Game over.\n";
            gameRunning = false;
            break;
        }

        grid = nextGrid;
        generation++;

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}