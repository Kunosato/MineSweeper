#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#define NUM_OF_ROWS_AT_EASY 9
#define NUM_OF_LINES_AT_EASY 9
#define NUM_OF_MINES_AT_EASY 10
#define NUM_OF_ROWS_AT_NORMAL 16
#define NUM_OF_LINES_AT_NORMAL 16
#define NUM_OF_MINES_AT_NORMAL 40
#define NUM_OF_ROWS_AT_HARD 30
#define NUM_OF_LINES_AT_HARD 16
#define NUM_OF_MINES_AT_HARD 99

typedef enum {easy, normal, hard} Level;
typedef enum {close, flag, question, open} State;
typedef struct {
	int isMine_;
	int numOfSurrounding_;
	State state_;
} Cell;

int turn;
int isFinished = 0;
Level level;
Cell cells[NUM_OF_LINES_AT_HARD][NUM_OF_ROWS_AT_HARD];

Level SelectLevel();
void SetField();
void InitializeField();
void SetMines(int numOfMines);
void SetNumOfSurrounding();
// 0以上、引数未満の整数を返します。
int RandomNext(int max);
int ActiveRows();
int ActiveLines();
int NumOfMines();
void Draw();
// 引数のセルに関して表示すべき文字列(正確にはポインタ)を返します。
char *GetString(Cell cell);
void Input();

int main(){
	srand((unsigned int)time(NULL));
	level = SelectLevel();
	SetField();
	/*while(!isFinished){
		Draw();
		Input();
	}*/
	Draw();
	return 0;
}

Level SelectLevel(){
	int l;
	do{
		printf("難易度を選んでください(初級：1 中級：2 上級：3)：");
		scanf("%d", &l);
	}while(l < 1 || l > 3);
	switch(l){
	case 1 : return easy;
	case 2 : return normal;
	case 3 : return hard;
	}
}

void SetField(){
	InitializeField();
	SetMines(NumOfMines());
	SetNumOfSurrounding();
}

void InitializeField(){
	int x, y;
	for(y = 0; y < NUM_OF_LINES_AT_HARD; y++){
		for(x = 0; x < NUM_OF_ROWS_AT_HARD; x++){
			cells[y][x].isMine_ = 0;
			cells[y][x].numOfSurrounding_ = 0;
			cells[y][x].state_ = close;
		}
	}
}

void SetMines(int numOfMines){
	if(numOfMines > 0){
		int x, y;
		do{
			x = RandomNext(ActiveRows());
			y = RandomNext(ActiveLines());
		}while(cells[y][x].isMine_);
		cells[y][x].isMine_ = 1;
		SetMines(numOfMines - 1);
	}
}

void SetNumOfSurrounding(){
	int x, y;
	for(y = 0; y < ActiveLines(); y++){
		for(x = 0; x < ActiveRows(); x++){
			if(!cells[y][x].isMine_){
				if(x > 0 && y > 0 && cells[y - 1][x - 1].isMine_){
					cells[y][x].numOfSurrounding_++;
				}
				if(y > 0 && cells[y - 1][x].isMine_){
					cells[y][x].numOfSurrounding_++;
				}
				if(x < ActiveRows() - 1 && y > 0 && cells[y - 1][x + 1].isMine_){
					cells[y][x].numOfSurrounding_++;
				}
				if(x > 0 && cells[y][x - 1].isMine_){
					cells[y][x].numOfSurrounding_++;
				}
				if(x < ActiveRows() - 1 && cells[y][x + 1].isMine_){
					cells[y][x].numOfSurrounding_++;
				}
				if(x > 0 && y < ActiveLines() - 1 && cells[y + 1][x - 1].isMine_){
					cells[y][x].numOfSurrounding_++;
				}
				if(y < ActiveLines() - 1 && cells[y + 1][x].isMine_){
					cells[y][x].numOfSurrounding_++;
				}
				if(x < ActiveRows() - 1 && y < ActiveLines() - 1 && cells[y + 1][x + 1].isMine_){
					cells[y][x].numOfSurrounding_++;
				}
			}
		}
	}
}

int RandomNext(int max){
	return rand() % max;
}
int ActiveRows(){
	switch(level){
	case easy : return NUM_OF_ROWS_AT_EASY;
	case normal : return NUM_OF_ROWS_AT_NORMAL;
	case hard : return NUM_OF_ROWS_AT_HARD;
	}
}
int ActiveLines(){
	switch(level){
	case easy : return NUM_OF_LINES_AT_EASY;
	case normal : return NUM_OF_LINES_AT_NORMAL;
	case hard : return NUM_OF_LINES_AT_HARD;
	}
}
int NumOfMines(){
	switch(level){
	case easy : return NUM_OF_MINES_AT_EASY;
	case normal : return NUM_OF_MINES_AT_NORMAL;
	case hard : return NUM_OF_MINES_AT_HARD;
	}
}

void Draw(){
	int x, y;
	printf("  |");
	for(x = 0; x < ActiveRows(); x++){
		printf("%3d", x + 1);
	}
	putchar('\n');
	printf("--|");
	for(x = 0; x < ActiveRows(); x++){
		printf("---");
	}
	putchar('\n');
	for(y = 0; y < ActiveLines(); y++){
		printf("%2d|", y + 1);
		for(x = 0; x < ActiveRows(); x++){
			printf(" %s", GetString(cells[y][x]));
			//printf("%2d ", cells[y][x].numOfSurrounding_);
		}
		putchar('\n');
	}
}

char *GetString(Cell cell){
	if(cell.state_ == close){
		return "■";
	}else if(cell.state_ == flag){
		return "|>";
	}else if(cell.state_ == question){
		return "？";
	}else if(cell.state_ == open && cell.isMine_){
		return "※";
	}else{
		switch(cell.numOfSurrounding_){
		case 0 : return "０";
		case 1 : return "１";
		case 2 : return "２";
		case 3 : return "３";
		case 4 : return "４";
		case 5 : return "５";
		case 6 : return "６";
		case 7 : return "７";
		case 8 : return "８";
		default : return "□";
		}
	}

	//if(cell.isMine_){
	//	return "※";
	//}else{
	//	switch(cell.numOfSurrounding_){
	//	case 0 : return "０";
	//	case 1 : return "１";
	//	case 2 : return "２";
	//	case 3 : return "３";
	//	case 4 : return "４";
	//	case 5 : return "５";
	//	case 6 : return "６";
	//	case 7 : return "７";
	//	case 8 : return "８";
	//	case 9 : return "９";
	//	case 10 : return "10";
	//	default : return "□";
	//	}
	//}
}

void Input(){

}