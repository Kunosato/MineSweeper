#include<stdio.h>
#define ROWS_OF_EASY 9
#define LINES_OF_EASY 9
#define ROWS_OF_NORMAL 15
#define LINES_OF_NORMAL 15
#define ROWS_OF_HARD 15
#define LINES_OF_HARD 20
#define NUM_OF_ALL_CELLS ROWS_OF_HARD*LINES_OF_HARD

enum Level {easy, normal, hard};
typedef struct {
	int isActive_;
	int isMine_;
	int state_;
	int info_;
} Cell;
int RowsOf(Level level);
int LinesOf(Level level);
void SelectLevel(Level *pLevel);
void SetField(Cell *pCells, Level *pLevel);
void Display();

int main(){

	Level level;
	Level *pLevel = &level;
	int turn;

	Cell cells[ROWS_OF_HARD][LINES_OF_HARD];

	SelectLevel(pLevel);
	SetField(cells, pLevel);
	Display();


	return 0;
}

int RowsOf(Level level){
	switch(level){
	case easy : return ROWS_OF_EASY;
	case normal : return ROWS_OF_NORMAL;
	case hard : return ROWS_OF_HARD;
	}
}
int LinesOf(Level level){
	switch(level){
	case easy : return LINES_OF_EASY;
	case normal : return LINES_OF_NORMAL;
	case hard : return LINES_OF_HARD;
	}
}

void SelectLevel(Level *pLevel){
	do{
		printf("難易度を選んでください(初級：1 中級：2 上級：3)：");
		scanf("%d", pLevel);
	}while(*pLevel < 1 || *pLevel > 3);
}

void SetField(Cell *pCells, Level *pLevel){
	//Cell *ip = pCells;
	int i;
	for(i = 0; i < NUM_OF_ALL_CELLS; i++){
		switch((pCells + i)->info_){
			case 
		}
	}
}

void Display(){

}