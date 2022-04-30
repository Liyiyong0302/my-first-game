#include<stdio.h>
#include<stdlib.h>
#include<easyx.h>
#include<conio.h>
#include<time.h>


#define ROW 15
#define COL 15
#define IMGW 40

char Direction = 'd';

typedef struct List {
	int x;
	int y;
	struct List* next;
}*Queue;
typedef struct {
	Queue front;
	Queue rear;
}LinkQueue;


LinkQueue* InitQueue() {
	LinkQueue* Q = (LinkQueue*)malloc(sizeof(LinkQueue));
	Q->front = Q->rear = (Queue)malloc(sizeof(List));
	Q->front->next = NULL;
	return Q;
}
 
void EnQueue(LinkQueue* Q, int x, int y) {
	Queue p = (Queue)malloc(sizeof(List));
	p->x = x;
	p->y = y;
	p->next = NULL;
	Q->rear->next = p;
	Q->rear = p;
}

void DeQueue(LinkQueue* Q) {
	Queue p = Q->front->next;
	Q->front->next = p->next;
	free(p);
}





void Over(int map[ROW][COL],int* r,int* c,LinkQueue* Q) {
	int ret = MessageBox(GetHWnd(), "再来一局？", "Game Over!", MB_OKCANCEL);
	if (ret == IDOK) {
		map[ROW][COL] = { 0 };
		for (int j = 0; j < COL; j++) {
			map[0][j] = 1;
			map[14][j] = 1;
			map[j][0] = 1;
			map[j][14] = 1;
		}
		Q->front = Q->rear;
		Q->front->next = NULL;
		EnQueue(Q, 3, 3);
		*r = 3; *c = 3;


	}
	else {
		exit(0);
	}
}

void showMap(int map[ROW][COL]) {
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COL; j++) {
			printf("%2d", map[i][j]);
		}
		printf("\n");
	}
}

void drawMap(int map[ROW][COL], IMAGE img[]) {
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COL; j++) {
			if (map[i][j] == -1 ) {
				putimage(j*IMGW, i*IMGW, img + 0);
			}
			else if(map[i][j]==0){
				putimage(j*IMGW, i*IMGW, img + 1);
			}
			else if (map[i][j]==1) {
				putimage(j*IMGW, i*IMGW, img + 2);
			}
			else {
				putimage(j*IMGW, i*IMGW, img + 3);
			}
		}
	}
}



void Move(char* Direction, IMAGE img[3], int map[ROW][COL], int* r, int* c, LinkQueue* Q, int* x, int* y) {
	if (_kbhit()) {
		*Direction = _getch();
	}
	switch (*Direction) {
	case'w':
	case'W': { if (map[*r - 1][*c] == 1 || map[*r - 1][*c] == -1)
	{
		Over(map, r, c, Q);
	}
			 else if (map[*r - 1][*c] == 2) {
		EnQueue(Q, --*r, *c);
	}
			 else { 
		EnQueue(Q, --*r, *c);  DeQueue(Q); } } break;
	case'a':
	case'A': { if (map[*r][*c - 1] == 1 || map[*r][*c - 1] == -1)
	{
		Over(map, r, c, Q);
	}
			 else if (map[*r][*c - 1] == 2) {
		EnQueue(Q, *r, --*c);
	}
			 else {
		EnQueue(Q, *r, --*c); DeQueue(Q); } } break;
	case's':
	case'S': { if (map[*r + 1][*c] == 1 || map[*r + 1][*c] == -1)
	{
		Over(map, r, c, Q);
	}
			 else if (map[*r + 1][*c] == 2) {
		EnQueue(Q, ++*r, *c);
	}
			 else { 
		EnQueue(Q, ++*r, *c); DeQueue(Q); } } break;
	case'd':
	case'D': { if (map[*r][*c + 1] == 1 || map[*r][*c + 1] == -1)
	{
		Over(map, r, c, Q);
	}
			 else if (map[*r][*c + 1] == 2) {
		EnQueue(Q, *r, ++*c);
	}
			 else { 
		EnQueue(Q, *r, ++*c); DeQueue(Q); } } break;


	}
	for (int i = 1; i < ROW-1; i++) {
		for (int j = 1; j < COL-1; j++) {
			map[i][j] = 0;
		}
	}
	map[*x][*y] = 2;
	Queue p = Q->front->next;
	while (p) {
		map[p->x][p->y] = -1;
		p = p->next;
	}




	drawMap(map, img);
	showMap(map);
	Sleep(150);

}


int main() {

	LinkQueue* Q = InitQueue();
	EnQueue(Q, 3, 3);

	srand((unsigned)time(NULL));
	initgraph(COL*IMGW, ROW*IMGW);
	int map[ROW][COL] = { 0 };
	for (int j = 0; j < COL;j++) {
		map[0][j] = 1;
		map[14][j] = 1;
		map[j][0] = 1;
		map[j][14] = 1;
	}
	int r = 3, c = 3;
	map[r][c] = -1;



	IMAGE img[4];

	loadimage(img + 0, "./images/0.jpg", IMGW, IMGW);
	loadimage(img + 1, "./images/1.jpg", IMGW, IMGW);
	loadimage(img + 2, "./images/6.jpg", IMGW, IMGW);
	loadimage(img + 3, "./images/3.jpg", IMGW, IMGW);
	drawMap(map, img);
	showMap(map);

	int ret = MessageBox(GetHWnd(), "是否开始贪吃蛇", "提示信息", MB_OKCANCEL);
	if (ret == IDCANCEL) {
		exit(0);
	}

	while (true) {
		int x, y;
		while(true) {
			x = 1 + rand() % 13;
			y = 1 + rand() % 13;
			if (map[x][y] != -1) {
				map[x][y] = 2;
				break;
			}
		}
		while (map[x][y]==2) {
			Move(&Direction, img, map, &r, &c, Q,&x,&y);

		}
	}
}