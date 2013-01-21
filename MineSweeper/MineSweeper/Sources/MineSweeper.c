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
int numOfFlag = 0;
int numOfOpendCells = 0;
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
void Lose();
void Win();

int main(){
	srand((unsigned int)time(NULL));
	level = SelectLevel();
	SetField();
	while(!isFinished){
		Draw();
		Input();
	}
	return 0;
}

Level SelectLevel(){
	int l;
	do{
		printf("難易度を選んでください(初級：1 中級：2 上級：3)：");
		scanf("%d", &l);
		if(l < 1 || l > 3){
			puts("入力値が不正です。");
		}
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
}

void Input(){
	char commnd;
	int inputX, inputY;
	printf("地雷：残り%d個\n", NumOfMines() - numOfFlag);
	puts("以下の例のように半角英数を半角スペースで区切って入力してください。");
	puts("例1）左から3マス目、上から5マス目を開ける場合");
	puts("入力欄：x 3 5");
	puts("例2）左から7マス目、上から1マス目に旗を立てる(又は取り除く)場合");
	puts("入力欄：f 7 1");
	puts("例3）左から2マス目、上から10マス目に？マークをつける(又は取り除く)場合");
	puts("入力欄：q 2 10");
	putchar('\n');
	do{
		printf("入力欄：");
		scanf(" %c %d %d", &commnd, &inputX, &inputY);
		if(inputX < 1 || inputX > ActiveRows() || inputY < 1 || inputY > ActiveLines()){
			puts("対象が範囲外です。");
		}else if(commnd != 'x' && commnd != 'f' && commnd != 'q'){
			puts("コマンドが不正です。");
		}else if(cells[inputY - 1][inputX - 1].state_ == open){
			puts("すでに開いています。");
		}else if(commnd == 'x' && cells[inputY - 1][inputX - 1].state_ == flag){
			puts("旗が立っています。");
		}
	}while(inputX < 1 || inputX > ActiveRows() || inputY < 1 || inputY > ActiveLines()
		|| (commnd != 'x' && commnd != 'f' && commnd != 'q')
		|| cells[inputY - 1][inputX - 1].state_ == open
		|| (commnd == 'x' && cells[inputY - 1][inputX - 1].state_ == flag));
	switch(commnd){
	case 'x' :
		cells[inputY - 1][inputX - 1].state_ = open;
		numOfOpendCells++;
		if(cells[inputY - 1][inputX - 1].isMine_){
			Lose();
		}else if(numOfOpendCells == ActiveLines() * ActiveRows() - NumOfMines()){
			Win();
		}
		break;
	case 'f' :
		if(cells[inputY - 1][inputX - 1].state_ == flag){
			cells[inputY - 1][inputX - 1].state_ = close;
			numOfFlag--;
		}else{
			cells[inputY - 1][inputX - 1].state_ = flag;
			numOfFlag++;
		}
		break;
	case 'q' :
		if(cells[inputY - 1][inputX - 1].state_ == question){
			cells[inputY - 1][inputX - 1].state_ = close;
		}else{
			cells[inputY - 1][inputX - 1].state_ = question;
		}
		break;
	}
}

void Lose(){
	isFinished = 1;
}

void Win(){
	isFinished = 1;
}
