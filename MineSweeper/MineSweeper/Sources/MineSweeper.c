#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#define RECOMMENDED_BUFFER_ROWS 100
#define RECOMMENDED_BUFFER_LINES 30
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
void InitializeField();
void Draw();
void CleanDisplay();
int ActiveRows();
int ActiveLines();
// 引数のセルに関して表示すべき文字列(正確にはポインタ)を返します。
char *GetString(Cell cell);
void Input();
int NumOfMines();
// 引数のインデックスを持つセルを開け、値が0の場合、再帰的に周囲も開きます。
void OpenCellAt(int x, int y);
void SetField();
void SetMines(int numOfMines);
// 0以上、引数未満の整数の乱数を返します。
int RandomNext(int max);
void SetNumOfSurrounding();
void Lose();
void Win();

int main(){
	srand((unsigned int)time(NULL));
	level = SelectLevel();
	InitializeField();
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

void Draw(){
	int x, y;
	CleanDisplay();
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
		}
		putchar('\n');
	}
}

void CleanDisplay(){
	int i;
	for(i = 0; i < RECOMMENDED_BUFFER_LINES; i++){
		putchar('\n');
	}
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

char *GetString(Cell cell){
	if(cell.state_ == close){
		if(isFinished && cell.isMine_){
			return "×";
		}else{
			return "■";
		}
	}else if(cell.state_ == flag){
		return "|>";
	}else if(cell.state_ == question){
		return "？";
	}else if(cell.state_ == open && cell.isMine_){
		return "※";
	}else{
		switch(cell.numOfSurrounding_){
		case 1 : return "１";
		case 2 : return "２";
		case 3 : return "３";
		case 4 : return "４";
		case 5 : return "５";
		case 6 : return "６";
		case 7 : return "７";
		case 8 : return "８";
		default : return "  ";
		}
	}
}

void Input(){
	char commnd;
	int inputX, inputY;
	int isCorrectCommand = 0;
	printf("地雷：残り%d個\n", NumOfMines() - numOfFlag);
	puts("以下の例のように半角英数を半角スペース(又は改行)で区切って入力してください。");
	puts("例1）左から3マス目、上から5マス目を開ける場合");
	puts("入力欄：x 3 5");
	puts("例2）左から7マス目、上から1マス目に旗を立てる(又は取り除く)場合");
	puts("入力欄：f 7 1");
	puts("例3）左から2マス目、上から10マス目に？マークをつける(又は取り除く)場合");
	puts("入力欄：q 2 10");
	puts("例4）ゲームを終了したい場合");
	puts("入力欄：e 1 1");
	putchar('\n');
	do{
		printf("入力欄：");
		scanf(" %c %d %d", &commnd, &inputX, &inputY);
		if(inputX < 1 || inputX > ActiveRows() || inputY < 1 || inputY > ActiveLines()){
			puts("対象が範囲外です。");
		}else if(commnd != 'x' && commnd != 'f' && commnd != 'q' && commnd != 'e'){
			puts("コマンドが不正です。");
		}else if(cells[inputY - 1][inputX - 1].state_ == open){
			puts("すでに開いています。");
		}else if(commnd == 'x' && cells[inputY - 1][inputX - 1].state_ == flag){
			puts("旗が立っています。");
		}else if(commnd == 'x' && cells[inputY - 1][inputX - 1].state_ == question){
			puts("？が付いています。");
		}else{
			isCorrectCommand = 1;
		}
	}while(!isCorrectCommand);
	switch(commnd){
	case 'x' :
		OpenCellAt(inputX - 1, inputY - 1);
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
	case 'e' :
		exit(0);
		break;
	}
}

int NumOfMines(){
	switch(level){
	case easy : return NUM_OF_MINES_AT_EASY;
	case normal : return NUM_OF_MINES_AT_NORMAL;
	case hard : return NUM_OF_MINES_AT_HARD;
	}
}

//void OpenCellAt(int x, int y){
//	if(cells[y][x].state_ != open){
//		cells[y][x].state_ = open;
//		if(numOfOpendCells++ == 0){
//			SetField();
//		}
//		if(cells[y][x].numOfSurrounding_ == 0){
//			if(x > 0 && y > 0){
//				OpenCellAt(x - 1, y - 1);
//			}
//			if(y > 0){
//				OpenCellAt(x, y - 1);
//			}
//			if(x < ActiveRows() - 1 && y > 0){
//				OpenCellAt(x + 1, y - 1);
//			}
//			if(x > 0){
//				OpenCellAt(x - 1, y);
//			}
//			if(x < ActiveRows() - 1){
//				OpenCellAt(x + 1, y);
//			}
//			if(x > 0 && y < ActiveLines() - 1){
//				OpenCellAt(x - 1, y + 1);
//			}
//			if(y < ActiveLines() - 1){
//				OpenCellAt(x, y + 1);
//			}
//			if(x < ActiveRows() - 1 && y < ActiveLines() - 1){
//				OpenCellAt(x + 1, y + 1);
//			}
//		}
//	}
//}
void OpenCellAt(int x, int y){
	cells[y][x].state_ = open;
	if(numOfOpendCells++ == 0){
		SetField();
	}
	if(cells[y][x].numOfSurrounding_ == 0){
		if(x > 0 && y > 0 && cells[y - 1][x - 1].state_ == close){
			OpenCellAt(x - 1, y - 1);
		}
		if(y > 0 && cells[y - 1][x].state_ == close){
			OpenCellAt(x, y - 1);
		}
		if(x < ActiveRows() - 1 && y > 0 && cells[y - 1][x + 1].state_ == close){
			OpenCellAt(x + 1, y - 1);
		}
		if(x > 0 && cells[y][x - 1].state_ == close){
			OpenCellAt(x - 1, y);
		}
		if(x < ActiveRows() - 1 && cells[y][x + 1].state_ == close){
			OpenCellAt(x + 1, y);
		}
		if(x > 0 && y < ActiveLines() - 1 && cells[y + 1][x - 1].state_ == close){
			OpenCellAt(x - 1, y + 1);
		}
		if(y < ActiveLines() - 1 && cells[y + 1][x].state_ == close){
			OpenCellAt(x, y + 1);
		}
		if(x < ActiveRows() - 1 && y < ActiveLines() - 1 && cells[y + 1][x + 1].state_ == close){
			OpenCellAt(x + 1, y + 1);
		}
	}
}

void SetField(){
	SetMines(NumOfMines());
	SetNumOfSurrounding();
}

void SetMines(int numOfMines){
	if(numOfMines > 0){
		int x, y;
		do{
			x = RandomNext(ActiveRows());
			y = RandomNext(ActiveLines());
		}while(cells[y][x].isMine_ || cells[y][x].state_ == open);
		cells[y][x].isMine_ = 1;
		SetMines(numOfMines - 1);
	}
}

int RandomNext(int max){
	return rand() % max;
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

void Lose(){
	isFinished = 1;
	Draw();
	puts("地雷を踏んでしまいました。あなたの負けです。");
}

void Win(){
	isFinished = 1;
	Draw();
	puts("おめでとうございます。あなたの勝ちです。");
}
