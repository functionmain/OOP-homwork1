#include <iostream>
#include <vector>
#include <unordered_set>
#include <string>

using namespace std;

// ANSI 颜色转义序列
const string RED = "\033[31m";
const string GREEN = "\033[32m";
const string RESET = "\033[0m";

// 定义Grid类
class Grid {
protected:
    vector<vector<int>> grid; // 棋盘
    vector<vector<bool>> isOriginal;  // 标记哪些是原始数字
    unsigned int BOX_SIZE ;   // 小宫的边长
    unsigned int GRID_SIZE ;  // 棋盘的边长
    unsigned int totel_len ;   // 棋盘的大小

public:
    // 构造函数，初始化空棋盘
    Grid(const string& input) {
        totel_len = input.length();
        GRID_SIZE = sqrt(totel_len);
        BOX_SIZE = sqrt(GRID_SIZE);

        grid.resize(GRID_SIZE, vector<int>(GRID_SIZE, 0));
        isOriginal.resize(GRID_SIZE, vector<bool>(GRID_SIZE, false));
    }

    // 获取指定行的值
    vector<int> getRow(int row) {
        return grid[row];
    }

    // 获取指定列的值
    vector<int> getColumn(int col) {
        vector<int> column(GRID_SIZE);
        for (int i = 0; i < GRID_SIZE; ++i) {
            column[i] = grid[i][col];
        }
        return column;
    }

    // 获取指定小宫格的值
    vector<int> getBox(int row, int col) {
        vector<int> box;
        int boxStartRow = (row / BOX_SIZE) * BOX_SIZE;
        int boxStartCol = (col / BOX_SIZE) * BOX_SIZE;
        for (int i = 0; i < BOX_SIZE; ++i) {
            for (int j = 0; j < BOX_SIZE; ++j) {
                box.push_back(grid[boxStartRow + i][boxStartCol + j]);
            }
        }
        return box;
    }

    // 打印当前棋盘，并根据状态决定颜色
    void printGrid() {
        for (int i = 0; i < GRID_SIZE; ++i) {
            for (int j = 0; j < GRID_SIZE; ++j) {
                if (grid[i][j] == 0) {
                    cout << ". ";
                }
                else if (isOriginal[i][j]) {
                    // 原始数独数字，红色输出
                    cout << RED << grid[i][j] << RESET << " ";
                }
                else {
                    // 解法填写的数字，绿色输出
                    cout << GREEN << grid[i][j] << RESET << " ";
                }
            }
            cout << endl;
        }
    }
};

// Sudoku类继承自Grid类
class Sudoku : public Grid {
public:

    Sudoku(const string& input) : Grid(input) {
        parse(input);
    }

    // 从字符串解析数独输入
    void parse(const string& input) {
        if (input.size() != GRID_SIZE * GRID_SIZE) {
            throw invalid_argument("输入不合法，长度必须为81");
        }

        for (int i = 0; i < GRID_SIZE; ++i) {
            for (int j = 0; j < GRID_SIZE; ++j) {
                int value = input[i * GRID_SIZE + j] - '0';
                grid[i][j] = value;
                if (value != 0) {
                    isOriginal[i][j] = true; // 标记为原始数字
                }
            }
        }
    }

    // 获取每个单元格的候选值
    vector<int> getInference(int row, int col) {
        if (grid[row][col] != 0) {
            // cout << " 该格非空 : " << grid[row][col] << endl;
            return {}; // 如果已经有值了，返回空
        }

        unordered_set<int> usedValues;
        // 获取行、列和小宫格的已使用数字
        vector<int> rowValues = getRow(row);
        vector<int> colValues = getColumn(col);
        vector<int> boxValues = getBox(row, col);

        // 将这些值加入已使用的集合
        for (int val : rowValues) {
            if (val != 0) usedValues.insert(val);
        }
        for (int val : colValues) {
            if (val != 0) usedValues.insert(val);
        }
        for (int val : boxValues) {
            if (val != 0) usedValues.insert(val);
        }

        // 判断1-9的数字哪些可以是候选值
        vector<int> candidates;
        for (int num = 1; num <= 9; ++num) {
            if (usedValues.find(num) == usedValues.end()) {
                candidates.push_back(num);
            }
        }

        return candidates;
    }

    // 深度优先搜索 (DFS) 解数独
    bool solve_DFS(int row = 0, int col = 0) {
        // 如果已经到达最后一行，说明解决成功
        if (row == GRID_SIZE) {
            return true;
        }

        // 如果已经到达最后一列，移动到下一行
        if (col == GRID_SIZE) {
            return solve_DFS(row + 1, 0);
        }

        // 如果当前单元格已经有值，跳过
        if (grid[row][col] != 0) {
            return solve_DFS(row, col + 1);
        }

        // 获取当前空单元格的候选值
        vector<int> candidates = getInference(row, col);
        if (candidates.empty()) {
            return false; // 如果没有候选值，回溯
        }

        // 尝试所有候选值
        for (int num : candidates) {
            grid[row][col] = num; // 填入候选值
            if (solve_DFS(row, col + 1)) { // 递归解决下一个单元格
                return true;
            }
            grid[row][col] = 0; // 回溯
        }

        return false; // 如果所有候选值都无效，回溯
    }

};


int main() {
    string input = "017903600000080000900000507072010430000402070064370250701000065000030000005601720";

    //Sudoku sudoku;
    Sudoku sudoku(input);

    try {
        sudoku.parse(input);  // 解析输入
    }
    catch (exception& e) {
        cout << e.what() << endl;
        return -1;
    }

    cout << "[*] 数独棋盘解析成功 : \n" << endl;
    sudoku.printGrid();  // 打印棋盘

    cout << endl;

    // 尝试求解数独
    if (sudoku.solve_DFS()) {
        cout << "\n[*] 数独有解, 解法如下 : \n" << endl;
        sudoku.printGrid();  // 打印解法后的棋盘
    }
    else {
        cout << "无法填写" << endl;
    }

    return 0;
}