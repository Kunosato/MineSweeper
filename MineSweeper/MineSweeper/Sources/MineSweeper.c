#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#define ROWS_OF_EASY 9
#define LINES_OF_EASY 9
#define NUM_OF_MINES_AT_EASY 10
#define ROWS_OF_NORMAL 16
#define LINES_OF_NORMAL 16
#define NUM_OF_MINES_AT_NORMAL 40
#define ROWS_OF_HARD 30
#define LINES_OF_HARD 16
#define NUM_OF_MINES_AT_HARD 99

enum Level {easy, normal, hard};
enum State {close, flag, question, open};
typedef struct {
	int isMine_;
	State state_;
} Cell;

int turn;
int isFinished = 0;
Level level;
Cell cells[ROWS_OF_HARD][LINES_OF_HARD];

void SelectLevel();
void SetField();
void InitializeField();
int Random(int min,int max);
int ActiveRows();
int ActiveLines();
int NumOfMines();
void Draw();
void Input();

int main(){
	srand((unsigned int)time(NULL));
	SelectLevel();
	SetField();
	while(!isFinished){
		Draw();
		Input();
	}
	return 0;
}

void SelectLevel(){
	do{
		printf("難易度を選んでください(初級：1 中級：2 上級：3)：");
		scanf("%d", &level);
	}while(level < 1 || level > 3);
}

void SetField(){
	InitializeField();
	int x, y;
	/////////////////////////////
}

void InitializeField(){
	int x, y;
	for(y = 0; y < LINES_OF_HARD; y++){
		for(x = 0; x < ROWS_OF_HARD; x++){
			cells[y][x].isMine_ = 0;
			cells[y][x].state_ = close;
		}
	}
}

int Random(int min,int max){
	return min + rand() % (max - min + 1);
}

int ActiveRows(){
	switch(level){
	case easy : return ROWS_OF_EASY;
	case normal : return ROWS_OF_NORMAL;
	case hard : return ROWS_OF_HARD;
	}
}
int ActiveLines(){
	switch(level){
	case easy : return LINES_OF_EASY;
	case normal : return LINES_OF_NORMAL;
	case hard : return LINES_OF_HARD;
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
	for(y = 0; y < ActiveLines(); y++){
		for(x = 0; x < ActiveRows(); x++){

		}
	}
}