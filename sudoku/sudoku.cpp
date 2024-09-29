#include <iostream>
#include <vector>
#include <unordered_set>
#include <string>

using namespace std;

// ANSI ��ɫת������
const string RED = "\033[31m";
const string GREEN = "\033[32m";
const string RESET = "\033[0m";

// ����Grid��
class Grid {
protected:
    vector<vector<int>> grid; // ����
    vector<vector<bool>> isOriginal;  // �����Щ��ԭʼ����
    unsigned int BOX_SIZE ;   // С���ı߳�
    unsigned int GRID_SIZE ;  // ���̵ı߳�
    unsigned int totel_len ;   // ���̵Ĵ�С

public:
    // ���캯������ʼ��������
    Grid(const string& input) {
        totel_len = input.length();
        GRID_SIZE = sqrt(totel_len);
        BOX_SIZE = sqrt(GRID_SIZE);

        grid.resize(GRID_SIZE, vector<int>(GRID_SIZE, 0));
        isOriginal.resize(GRID_SIZE, vector<bool>(GRID_SIZE, false));
    }

    // ��ȡָ���е�ֵ
    vector<int> getRow(int row) {
        return grid[row];
    }

    // ��ȡָ���е�ֵ
    vector<int> getColumn(int col) {
        vector<int> column(GRID_SIZE);
        for (int i = 0; i < GRID_SIZE; ++i) {
            column[i] = grid[i][col];
        }
        return column;
    }

    // ��ȡָ��С�����ֵ
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

    // ��ӡ��ǰ���̣�������״̬������ɫ
    void printGrid() {
        for (int i = 0; i < GRID_SIZE; ++i) {
            for (int j = 0; j < GRID_SIZE; ++j) {
                if (grid[i][j] == 0) {
                    cout << ". ";
                }
                else if (isOriginal[i][j]) {
                    // ԭʼ�������֣���ɫ���
                    cout << RED << grid[i][j] << RESET << " ";
                }
                else {
                    // �ⷨ��д�����֣���ɫ���
                    cout << GREEN << grid[i][j] << RESET << " ";
                }
            }
            cout << endl;
        }
    }
};

// Sudoku��̳���Grid��
class Sudoku : public Grid {
public:

    Sudoku(const string& input) : Grid(input) {
        parse(input);
    }

    // ���ַ���������������
    void parse(const string& input) {
        if (input.size() != GRID_SIZE * GRID_SIZE) {
            throw invalid_argument("���벻�Ϸ������ȱ���Ϊ81");
        }

        for (int i = 0; i < GRID_SIZE; ++i) {
            for (int j = 0; j < GRID_SIZE; ++j) {
                int value = input[i * GRID_SIZE + j] - '0';
                grid[i][j] = value;
                if (value != 0) {
                    isOriginal[i][j] = true; // ���Ϊԭʼ����
                }
            }
        }
    }

    // ��ȡÿ����Ԫ��ĺ�ѡֵ
    vector<int> getInference(int row, int col) {
        if (grid[row][col] != 0) {
            // cout << " �ø�ǿ� : " << grid[row][col] << endl;
            return {}; // ����Ѿ���ֵ�ˣ����ؿ�
        }

        unordered_set<int> usedValues;
        // ��ȡ�С��к�С�������ʹ������
        vector<int> rowValues = getRow(row);
        vector<int> colValues = getColumn(col);
        vector<int> boxValues = getBox(row, col);

        // ����Щֵ������ʹ�õļ���
        for (int val : rowValues) {
            if (val != 0) usedValues.insert(val);
        }
        for (int val : colValues) {
            if (val != 0) usedValues.insert(val);
        }
        for (int val : boxValues) {
            if (val != 0) usedValues.insert(val);
        }

        // �ж�1-9��������Щ�����Ǻ�ѡֵ
        vector<int> candidates;
        for (int num = 1; num <= 9; ++num) {
            if (usedValues.find(num) == usedValues.end()) {
                candidates.push_back(num);
            }
        }

        return candidates;
    }

    // ����������� (DFS) ������
    bool solve_DFS(int row = 0, int col = 0) {
        // ����Ѿ��������һ�У�˵������ɹ�
        if (row == GRID_SIZE) {
            return true;
        }

        // ����Ѿ��������һ�У��ƶ�����һ��
        if (col == GRID_SIZE) {
            return solve_DFS(row + 1, 0);
        }

        // �����ǰ��Ԫ���Ѿ���ֵ������
        if (grid[row][col] != 0) {
            return solve_DFS(row, col + 1);
        }

        // ��ȡ��ǰ�յ�Ԫ��ĺ�ѡֵ
        vector<int> candidates = getInference(row, col);
        if (candidates.empty()) {
            return false; // ���û�к�ѡֵ������
        }

        // �������к�ѡֵ
        for (int num : candidates) {
            grid[row][col] = num; // �����ѡֵ
            if (solve_DFS(row, col + 1)) { // �ݹ�����һ����Ԫ��
                return true;
            }
            grid[row][col] = 0; // ����
        }

        return false; // ������к�ѡֵ����Ч������
    }

};


int main() {
    string input = "017903600000080000900000507072010430000402070064370250701000065000030000005601720";

    //Sudoku sudoku;
    Sudoku sudoku(input);

    try {
        sudoku.parse(input);  // ��������
    }
    catch (exception& e) {
        cout << e.what() << endl;
        return -1;
    }

    cout << "[*] �������̽����ɹ� : \n" << endl;
    sudoku.printGrid();  // ��ӡ����

    cout << endl;

    // �����������
    if (sudoku.solve_DFS()) {
        cout << "\n[*] �����н�, �ⷨ���� : \n" << endl;
        sudoku.printGrid();  // ��ӡ�ⷨ�������
    }
    else {
        cout << "�޷���д" << endl;
    }

    return 0;
}