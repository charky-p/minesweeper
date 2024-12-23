#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define CONTINUE 0
#define GAMEOVER 1
#define WIN 2
#define INVALID 1
#define VALID 0
#define EMPTY_CELL '*'
#define MINE 'X'
#define FLAG '<'

typedef struct {
  bool revealed;
  bool mine;
  bool flag;
  int neighbours;
} Cell;

void initialiseGrid(Cell **grid, int size, int mines);
void checkCellNeighbours(Cell **grid, int x, int y, int size);
void printGrid(Cell **grid, int size);
int validCell(Cell **grid, int size, int x, int y);
int revealCell(Cell **grid, int size, int mines, int x, int y);

int main(void) {
  int size, mines;
  printf("What size? ");
  scanf(" %d", &size);
  printf("How many mines? ");
  scanf(" %d", &mines);


  Cell **grid = malloc(sizeof(Cell *) * size);
  initialiseGrid(grid, size, mines);
  
  printGrid(grid, size);
  while (1) {
    int x, y;
    printf("Reveal which cell (x, y): ");
    scanf(" %d", &x);
    scanf(" %d", &y);

    if (validCell(grid, size, x, y) == INVALID) {
      printGrid(grid, size);
      printf("Invalid Cell!\n");
      continue;
    };

    char option;
    printf("Flag (F) or Reveal (R)? ");
    scanf(" %c", &option);
    if (option == 'F') {
      if (grid[x][y].flag == true) {
        printGrid(grid, size);
        printf("Cell Already Flagged!\n");
      } else if (grid[x][y].revealed == true) {
        printGrid(grid, size);
        printf("Cannot Flag Revealed Cell!\n");
      } else {
        grid[x][y].flag = true;
        printGrid(grid, size);
        printf("Cell Flagged!\n");
      }
    } else if (option == 'R') {
      if (grid[x][y].revealed == true) {
        printGrid(grid, size);
        printf("Cell Already Revealed!\n");
      }
      int status = revealCell(grid, size, mines, x, y);
      if (status == GAMEOVER) {
        printGrid(grid, size);
        printf("Gameover!\n");
        break;
      } else if (status == WIN) {
        printGrid(grid, size);
        printf("You Win!\n");
        break;
      } else {
        printGrid(grid, size);
        printf("You Revealed Cell %d %d!\n", x, y);
      }
    } else {
      printGrid(grid, size);
      printf("Invalid Option!\n");
    }
  }

  for (int i = 0; i < size; i++) {
    free(grid[i]);
  }
  free(grid);

  return 0;
}

void initialiseGrid(Cell **grid, int size, int mines) {
  for (int i = 0; i < size; i++) {
    grid[i] = malloc(sizeof(Cell) * size);
    
    for (int j = 0; j < size; j++) {
      grid[i][j].revealed = false;
      grid[i][j].mine = false;
      grid[i][j].neighbours = 0;
      grid[i][j].flag = false;
    }
  }

  // Generate mines
  int placedMines = 0;
  while (placedMines < mines) {
    int x = rand() % size;
    int y = rand() % size;
    if (grid[x][y].mine == false) {
      grid[x][y].mine = true;
      placedMines++;
    }
  }

  // Calculate neighbouring mines
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      if (grid[i][j].mine == true) continue;

      // Checking 8 surrounding cells
      checkCellNeighbours(grid, i, j, size);
    }
  }
}

void checkCellNeighbours(Cell **grid, int x, int y, int size) {
  for (int i = -1; i <= 1; i++) {
    if (x + i < 0 || x + i >= size) continue;
    for (int j = -1; j <= 1; j++) {
      if (y + j < 0 || y + j >= size) continue;
      if (i == 0 && j == 0) continue;

      if (grid[x + i][y + j].mine == true) {
        grid[x][y].neighbours++;
      }
    }
  }
}

void printGrid(Cell **grid, int size) {
  printf("  ");
  for (int j = 0; j < size; j++) {
    printf("___");
  }
  putchar('\n');

  for (int i = 0; i < size; i++) {
    printf(" |");

    for (int j = 0; j < size; j++) {
      putchar(' ');

      if (grid[i][j].revealed == false) {
        if (grid[i][j].flag == true) {
          putchar(FLAG);
        } else {
          putchar(EMPTY_CELL);
        }
      } else if (grid[i][j].mine) {
        putchar(MINE);
      } else if (grid[i][j].neighbours == 0) {
        putchar(' ');
      } else {
        printf("%d", grid[i][j].neighbours);
      }

      putchar(' ');
    }

    printf("|\n");
  }

  printf("  ");
  for (int j = 0; j < size; j++) {
    printf("---");
  }
  putchar('\n');
}

int validCell(Cell **grid, int size, int x, int y) {
  if (x < 0 || x >= size) {
    return INVALID;
  } else if (y < 0 || y >= size) {
    return INVALID;
  } else {
    return VALID;
  }
}

int revealCell(Cell **grid, int size, int mines, int x, int y) {
  if (grid[x][y].flag == true) {
    grid[x][y].flag = false;
    return CONTINUE;
  } else if (grid[x][y].mine == true) {
    grid[x][y].revealed = true;
    return GAMEOVER;
  }

  grid[x][y].revealed = true;
  int cellsRemaining = 0;
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      if (grid[x][y].revealed == true) continue;
      cellsRemaining++;
    }
  }

  if (cellsRemaining == mines) {
    return cellsRemaining == mines ? WIN : CONTINUE;
  }
}