﻿#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#define RECOMMENDED_BUFFER_ROWS 100
#define RECOMMENDED_BUFFER_LINES 35
#define NUM_OF_ROWS_AT_EASY 9
#define NUM_OF_LINES_AT_EASY 9
#define NUM_OF_MINES_AT_EASY 10
#define NUM_OF_ROWS_AT_NORMAL 16
#define NUM_OF_LINES_AT_NORMAL 16
#define NUM_OF_MINES_AT_NORMAL 40
#define NUM_OF_ROWS_AT_HARD 30
#define NUM_OF_LINES_AT_HARD 16
#define NUM_OF_MINES_AT_HARD 99
#define NUM_OF_MESSEAGE_LINES_AT_INPUT 14

enum GameState { standby, playing, win, lose };
enum Level { easy, normal, hard };
enum State { close, flag, question, open };
struct Cell {
	int isMine_;
	int numOfSurrounding_;
	enum State state_;
};

enum GameState gameState;
time_t startTime;
time_t winTime;
int numOfFlag;
int numOfOpendCells;
int isReplay;
enum Level level;
struct Cell cells[NUM_OF_LINES_AT_HARD][NUM_OF_ROWS_AT_HARD];

void DisplayCheck();
enum Level SelectLevel();
void Initialize();
void CleanField();
void Draw();
// 引数の数だけ改行します。
void CleanDisplay(int numOfLine);
int GetNumOfRows();
int GetNumOfLines();
// 引数のセルに関して表示すべき文字列(正確にはポインタ)を返します。
char *GetString(struct Cell cell);
void Input();
int GetNumOfMines();
// 引数のインデックスを持つセルを開け、値が0の場合、再帰的に周囲も開きます。
void OpenCellAt(int x, int y);
void SetField();
void SetMines(int numOfMines);
// 0以上、引数未満の整数の乱数を返します。
int RandomNext(int max);
void SetNumOfSurrounding();
// 引数のインデックスを持つセルの周囲の地雷の数を返します。
int GetNumOfSurroundingAround(int x, int y);
// 引数のインデックスを持つすでに開いているセルの周囲のすべての旗や?が付いていないセルを開きます。
void OpenAllCellsAround(int x, int y);
void Lose();
void NewGame();
void Replay();
void CloseField();
void Win();
// 前回の記録が書かれたファイルを書き換えます。
void UpdateFile(double difftime);

int main(){
	srand((unsigned int)time(NULL));
	DisplayCheck();
	level = SelectLevel();
	Initialize();
	CleanField();
	while(gameState == standby || gameState == playing){
		Draw();
		Input();
		if(gameState == lose){
			Lose();
		}else if(gameState == win){
			Win();
		}
	}
	return 0;
}

void DisplayCheck(){
	char commnd;
	int x, y;
	for(y = 0; y < RECOMMENDED_BUFFER_LINES - 3; y++){
		if(y == 0){
			putchar('_');
		}else{
			putchar('|');
		}
		for(x = 0; x < RECOMMENDED_BUFFER_ROWS - 3; x++){
			if(y == 0 || y == RECOMMENDED_BUFFER_LINES -3 - 1){
				putchar('_');
			}else{
				putchar(' ');
			}
		}
		if(y == 0){
			putchar('_');
		}else{
			putchar('|');
		}
		putchar('\n');
	}
	do{
		printf("この上に大きい長方形全体がきれいに表示されていますか？(Yes : y / No : n)：");
		scanf(" %c", &commnd);
		if(commnd == 'n'){
			printf("バッファとウィンドウのサイズを大きく設定して下さい。(推奨サイズ：幅%d以上、高さ%d以上)\n", RECOMMENDED_BUFFER_ROWS, RECOMMENDED_BUFFER_LINES);
			do{
				printf("変更出来ましたか？(Yes : y / No : n)：");
				scanf(" %c", &commnd);
				if(commnd == 'y'){
					DisplayCheck();
				}else if(commnd == 'n'){
					puts("その場合は初級、もしくは中級でのプレイをおすすめします。");
				}else{
					puts("入力値が不正です。「y」もしくは「n」を入力して下さい。");
				}
			}while(commnd != 'y' && commnd != 'n');
		}else if(commnd != 'y'){
			puts("入力値が不正です。「y」もしくは「n」を入力して下さい。");
		}
	}while(commnd != 'y' && commnd != 'n');
}

enum Level SelectLevel(){
	int l;
	do{
		printf("難易度を選んでください(初級：1 中級：2 上級：3)：");
		scanf(" %d", &l);
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

void Initialize(){
	gameState = standby;
	numOfFlag = 0;
	numOfOpendCells = 0;
	isReplay = 0;
}

void CleanField(){
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
	CleanDisplay(RECOMMENDED_BUFFER_LINES);
	printf("  |");
	for(x = 0; x < GetNumOfRows(); x++){
		printf("%3d", x + 1);
	}
	putchar('\n');
	printf("--|");
	for(x = 0; x < GetNumOfRows(); x++){
		printf("---");
	}
	putchar('\n');
	for(y = 0; y < GetNumOfLines(); y++){
		printf("%2d|", y + 1);
		for(x = 0; x < GetNumOfRows(); x++){
			printf(" %s", GetString(cells[y][x]));
		}
		putchar('\n');
	}
}

void CleanDisplay(int numOfLine){
	int i;
	for(i = 0; i < numOfLine; i++){
		putchar('\n');
	}
}

int GetNumOfRows(){
	switch(level){
	case easy : return NUM_OF_ROWS_AT_EASY;
	case normal : return NUM_OF_ROWS_AT_NORMAL;
	case hard : return NUM_OF_ROWS_AT_HARD;
	}
}

int GetNumOfLines(){
	switch(level){
	case easy : return NUM_OF_LINES_AT_EASY;
	case normal : return NUM_OF_LINES_AT_NORMAL;
	case hard : return NUM_OF_LINES_AT_HARD;
	}
}

char *GetString(struct Cell cell){
	if(cell.state_ == close){
		if(gameState == lose && cell.isMine_){
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
	printf("地雷：残り%d個", GetNumOfMines() - numOfFlag);
	if(gameState == playing && !isReplay){
		printf(" ﾀｲﾑ：%.f秒", difftime(time(NULL), startTime));
	}
	putchar('\n');
	puts("以下の例のように半角英数を半角スペース(又は改行)で区切って入力してください。");
	puts("例1）左から3マス目、上から5マス目を開ける場合");
	puts("入力欄：x 3 5");
	puts("例2）左から7マス目、上から1マス目に旗を立てる(又は取り除く)場合");
	puts("入力欄：f 7 1");
	puts("例3）左から2マス目、上から9マス目に？マークをつける(又は取り除く)場合");
	puts("入力欄：q 2 9");
	puts("例4）すでに開いている左から8マス目、上から2マス目の周囲の旗や？以外をすべて開ける場合");
	puts("入力欄：a 8 2");
	puts("例5）ゲームを終了したい場合");
	puts("入力欄：e");
	putchar('\n');
	do{
		printf("入力欄：");
		scanf(" %c", &commnd);
		if(commnd == 'e'){
			exit(0);
		}
		scanf(" %d %d", &inputX, &inputY);
		if(commnd != 'x' && commnd != 'f' && commnd != 'q' && commnd != 'a'){
			puts("コマンドが不正です。");
		}else if(inputX < 1 || inputX > GetNumOfRows() || inputY < 1 || inputY > GetNumOfLines()){
			puts("対象が範囲外です。");
		}else{
			if(commnd == 'a'){
				if(cells[inputY - 1][inputX - 1].state_ != open){
					puts("開いていません。");
				}else{
					isCorrectCommand = 1;
				}
			}else{
				if(cells[inputY - 1][inputX - 1].state_ == open){
					puts("すでに開いています。");
				}else if(commnd == 'x' && cells[inputY - 1][inputX - 1].state_ == flag){
					puts("旗が立っています。");
				}else if(commnd == 'x' && cells[inputY - 1][inputX - 1].state_ == question){
					puts("？が付いています。");
				}else{
					isCorrectCommand = 1;
				}
			}
		}
	}while(!isCorrectCommand);
	switch(commnd){
	case 'x' :
		OpenCellAt(inputX - 1, inputY - 1);
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
	case 'a' :
		OpenAllCellsAround(inputX - 1, inputY - 1);
		break;
	}
}

int GetNumOfMines(){
	switch(level){
	case easy : return NUM_OF_MINES_AT_EASY;
	case normal : return NUM_OF_MINES_AT_NORMAL;
	case hard : return NUM_OF_MINES_AT_HARD;
	}
}

void OpenCellAt(int x, int y){
	int isOpened = cells[y][x].state_ == open ? 1 : 0;
	if(!isOpened){
		cells[y][x].state_ = open;
		if(numOfOpendCells++ == 0 && !isReplay){
			if(isReplay){
				gameState = playing;
			}else{
				SetField();
			}
		}
	}
	if(cells[y][x].numOfSurrounding_ == 0 && !cells[y][x].isMine_){
		if(x > 0 && y > 0 && cells[y - 1][x - 1].state_ == close){
			OpenCellAt(x - 1, y - 1);
		}
		if(y > 0 && cells[y - 1][x].state_ == close){
			OpenCellAt(x, y - 1);
		}
		if(x < GetNumOfRows() - 1 && y > 0 && cells[y - 1][x + 1].state_ == close){
			OpenCellAt(x + 1, y - 1);
		}
		if(x > 0 && cells[y][x - 1].state_ == close){
			OpenCellAt(x - 1, y);
		}
		if(x < GetNumOfRows() - 1 && cells[y][x + 1].state_ == close){
			OpenCellAt(x + 1, y);
		}
		if(x > 0 && y < GetNumOfLines() - 1 && cells[y + 1][x - 1].state_ == close){
			OpenCellAt(x - 1, y + 1);
		}
		if(y < GetNumOfLines() - 1 && cells[y + 1][x].state_ == close){
			OpenCellAt(x, y + 1);
		}
		if(x < GetNumOfRows() - 1 && y < GetNumOfLines() - 1 && cells[y + 1][x + 1].state_ == close){
			OpenCellAt(x + 1, y + 1);
		}
	}
	if(isOpened){
		cells[y][x].numOfSurrounding_ = GetNumOfSurroundingAround(x, y);
	}else if(cells[y][x].isMine_){
		gameState = lose;
	}else if(numOfOpendCells == GetNumOfLines() * GetNumOfRows() - GetNumOfMines() && gameState != lose){
		gameState = win;
	}
}

void SetField(){
	SetMines(GetNumOfMines());
	SetNumOfSurrounding();
	gameState = playing;
	startTime = time(NULL);
}

void SetMines(int numOfMines){
	if(numOfMines > 0){
		int x, y;
		do{
			x = RandomNext(GetNumOfRows());
			y = RandomNext(GetNumOfLines());
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
	for(y = 0; y < GetNumOfLines(); y++){
		for(x = 0; x < GetNumOfRows(); x++){
			if(!cells[y][x].isMine_){
				cells[y][x].numOfSurrounding_ = GetNumOfSurroundingAround(x, y);
			}
		}
	}
}

int GetNumOfSurroundingAround(int x, int y){
	int numOfSurrounding = 0;
	if(x > 0 && y > 0 && cells[y - 1][x - 1].isMine_){
		numOfSurrounding++;
	}
	if(y > 0 && cells[y - 1][x].isMine_){
		numOfSurrounding++;
	}
	if(x < GetNumOfRows() - 1 && y > 0 && cells[y - 1][x + 1].isMine_){
		numOfSurrounding++;
	}
	if(x > 0 && cells[y][x - 1].isMine_){
		numOfSurrounding++;
	}
	if(x < GetNumOfRows() - 1 && cells[y][x + 1].isMine_){
		numOfSurrounding++;
	}
	if(x > 0 && y < GetNumOfLines() - 1 && cells[y + 1][x - 1].isMine_){
		numOfSurrounding++;
	}
	if(y < GetNumOfLines() - 1 && cells[y + 1][x].isMine_){
		numOfSurrounding++;
	}
	if(x < GetNumOfRows() - 1 && y < GetNumOfLines() - 1 && cells[y + 1][x + 1].isMine_){
		numOfSurrounding++;
	}
	return numOfSurrounding;
}

void OpenAllCellsAround(int x, int y){
	cells[y][x].numOfSurrounding_ = 0;
	OpenCellAt(x, y);
}

void Lose(){
	int input;
	Draw();
	CleanDisplay(NUM_OF_MESSEAGE_LINES_AT_INPUT - 2);
	puts("地雷を踏んでしまいました。あなたの負けです。");
	do{
		printf("どうしますか？(新しく始める：1、同じ配置でもう一度始める：2、やめる：3)：");
		scanf(" %d", &input);
		if(input < 1 || input > 3){
			puts("入力値が不正です。");
		}
	}while(input < 1 || input > 3);
	switch(input){
	case 1 :
		NewGame();
		break;
	case 2 :
		Replay();
		break;
	case 3 :
		break;
	}
}

void NewGame(){
	level = SelectLevel();
	Initialize();
	CleanField();
}

void Replay(){
	Initialize();
	CloseField();
	isReplay = 1;
}

void CloseField(){
	int x, y;
	for(y = 0; y < NUM_OF_LINES_AT_HARD; y++){
		for(x = 0; x < NUM_OF_ROWS_AT_HARD; x++){
			cells[y][x].state_ = close;
		}
	}
}

void Win(){
	int input;
	winTime = time(NULL);
	Draw();
	CleanDisplay(NUM_OF_MESSEAGE_LINES_AT_INPUT - (isReplay ? 2 : 4));
	puts("おめでとうございます。あなたの勝ちです。");
	if(!isReplay){
		UpdateFile(difftime(winTime, startTime));
		printf("今回の記録：%.f秒\n", difftime(winTime, startTime));
	}
	do{
		printf("どうしますか？(新しく始める：1、同じ配置でもう一度始める：2、やめる：3)：");
		scanf(" %d", &input);
		if(input < 1 || input > 3){
			puts("入力値が不正です。");
		}
	}while(input < 1 || input > 3);
	switch(input){
	case 1 :
		NewGame();
		break;
	case 2 :
		Replay();
		break;
	case 3 :
		break;
	}
}

void UpdateFile(double difftime){
	FILE *fp;
	int c;

	switch(level){
	case easy :
		fp = fopen("data0.txt", "r");
		break;
	case normal :
		fp = fopen("data1.txt", "r");
		break;
	case hard :
		fp = fopen("data2.txt", "r");
		break;
	}
	if(fp != NULL){
		printf("前回の記録：");
		while((c = getc(fp)) != EOF){
			putchar(c);
		}
		puts("秒");
		fclose(fp);
	}

	switch(level){
	case easy :
		fp = fopen("data0.txt", "w");
		break;
	case normal :
		fp = fopen("data1.txt", "w");
		break;
	case hard :
		fp = fopen("data2.txt", "w");
		break;
	}
	fprintf(fp, "%.f", difftime);
	fclose(fp);
}
