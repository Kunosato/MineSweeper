#include<stdio.h>
#define EASY 1
#define NORMAL 2
#define HARD 3
#define ROWS_OF_EASY 9
#define LINES_OF_EASY 9
#define ROWS_OF_NORMAL 15
#define LINES_OF_NORMAL 15
#define ROWS_OF_HARD 15
#define LINES_OF_HARD 20

void SetField(int *pLevel);
void SelectLevel();


int main(){

	int level;
	int *pLevel = &level;
	struct Cell {
		int isActive_;
		int isMine_;
		int state_;
		int info_;
	};
	struct Cell cells[ROWS_OF_HARD][LINES_OF_HARD];

	SelectLevel(pLevel);

	return 0;
}

void SelectLevel(int *pLevel){
	do{
		printf("��Փx��I��ł�������(�����F1 �����F2 �㋉�F3)�F");
		scanf("%d", pLevel);
	}while(*pLevel < EASY || *pLevel > HARD);
}

void Display(){

}