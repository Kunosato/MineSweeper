#include<stdio.h>
#define ROWS_OF_EASY 9
#define LINES_OF_EASY 9
#define ROWS_OF_NORMAL 15
#define LINES_OF_NORMAL 15
#define ROWS_OF_HARD 15
#define LINES_OF_HARD 20
#define NUM_OF_ALL_CELLS ROWS_OF_HARD*LINES_OF_HARD

int turn;
int isFinished = 0;
enum Level {easy, normal, hard};
typedef struct {
	int isActive_;
	int isMine_;
	int state_;
	int info_;
} Cell;
Cell cells[ROWS_OF_HARD][LINES_OF_HARD];

void SelectLevel(Level *pLevel);
void SetField(Level level);
int RowsOf(Level level);
int LinesOf(Level level);
void Draw();
void Input();

int main(){

	Level level;
	Level *pLevel = &level;

	SelectLevel(pLevel);
	SetField(level);
	while(!isFinished){
		Draw();
		Input();
	}
	return 0;
}



void SelectLevel(Level *pLevel){
	do{
		printf("難易度を選んでください(初級：1 中級：2 上級：3)：");
		scanf("%d", pLevel);
	}while(*pLevel < 1 || *pLevel > 3);
}

void SetField(Level level){
	int x, y;
	for(y = 0; y < LINES_OF_HARD; y++){
		for(x = 0; x < ROWS_OF_HARD; x++){
			cells[y][x].isActive_ = (x < RowsOf(level) && y < LinesOf(level)) ? 1 : 0;
		}
		printf("\n");
	}
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

void Draw(){

}