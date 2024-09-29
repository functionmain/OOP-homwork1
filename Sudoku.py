class Grid:
    def __init__(self, input_str):
        self.total_len  = len(input_str)
        self.GRID_SIZE  = int(self.total_len ** 0.5)
        self.BOX_SIZE   = int(self.GRID_SIZE ** 0.5)
        self.my_str = input_str
        # self.BOX_SIZE = 3
        # self.GRID_SIZE = 9
        self.grid = [[0 for _ in range(self.GRID_SIZE)] for _ in range(self.GRID_SIZE)]
        self.isOriginal = [[False for _ in range(self.GRID_SIZE)] for _ in range(self.GRID_SIZE)]
        

    def get_row(self, row):
        return self.grid[row]

    def get_column(self, col):
        return [self.grid[i][col] for i in range(self.GRID_SIZE)]

    def get_box(self, row, col):
        box = []
        box_start_row = (row // self.BOX_SIZE) * self.BOX_SIZE
        box_start_col = (col // self.BOX_SIZE) * self.BOX_SIZE
        for i in range(self.BOX_SIZE):
            for j in range(self.BOX_SIZE):
                box.append(self.grid[box_start_row + i][box_start_col + j])
        return box

    def print_grid(self):
        for i in range(self.GRID_SIZE):
            for j in range(self.GRID_SIZE):
                if self.grid[i][j] == 0:
                    print('. ', end='')
                elif self.isOriginal[i][j]:
                    print(f"\033[31m{self.grid[i][j]}\033[0m ", end='')
                else:
                    print(f"\033[32m{self.grid[i][j]}\033[0m ", end='')
            print()

class Sudoku(Grid):
    def __init__(self, input_str):
        super().__init__(input_str)

    def parse(self):
        if len(self.my_str) != self.GRID_SIZE * self.GRID_SIZE:
            raise ValueError("Input is not valid, length must be 81")
        for i in range(self.GRID_SIZE):
            for j in range(self.GRID_SIZE):
                value = int(self.my_str[i * self.GRID_SIZE + j])
                self.grid[i][j] = value
                if value != 0:
                    self.isOriginal[i][j] = True

    def get_inference(self, row, col):
        if self.grid[row][col] != 0:
            return []
        used_values = set()
        row_values = self.get_row(row)
        col_values = self.get_column(col)
        box_values = self.get_box(row, col)
        for val in row_values + col_values + box_values:
            if val != 0:
                used_values.add(val)
        candidates = [num for num in range(1, 10) if num not in used_values]
        return candidates

    def solve_dfs(self, row=0, col=0):
        if row == self.GRID_SIZE:
            return True
        if col == self.GRID_SIZE:
            return self.solve_dfs(row + 1, 0)
        if self.grid[row][col] != 0:
            return self.solve_dfs(row, col + 1)
        candidates = self.get_inference(row, col)
        for num in candidates:
            self.grid[row][col] = num
            if self.solve_dfs(row, col + 1):
                return True
            self.grid[row][col] = 0
        return False

if __name__ == "__main__":

    input_str = "017903600000080000900000507072010430000402070064370250701000065000030000005601720"
    sudoku = Sudoku(input_str)
    try:
        sudoku.parse()
    except ValueError as e:
        print(e)
        exit(-1)
    print("\n[*] 数独棋盘解析成功 : \n")
    sudoku.print_grid()
    
    if sudoku.solve_dfs():
        print("\n[*] 数独有解, 解法如下:\n")
        sudoku.print_grid()
    else:
        print("[*] 无解.")

