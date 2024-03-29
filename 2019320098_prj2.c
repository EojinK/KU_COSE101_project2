#include<stdio.h>
#include<time.h>
#include<conio.h>
#include<stdlib.h>
#include<windows.h>
#include<math.h>

// COSE101, Spring Semester, 2019
// Computer Science and Engineering, College of Informatics, Korea University, Seoul.

#define LEFT 75
#define RIGHT 77
#define DOWN 80
#define UP 72
#define SPACE 32 
#define p 112 
#define P 80 
#define ESC 27

#define MAP_ADJ_X 10
#define MAP_ADJ_Y 5
#define X 8
#define Y 5


typedef struct _block {
	int pos_x; //position of block
	int pos_y;
	int num;
	int nextnum;
	int isactive;
}Block;

Block block, block2;

// 자작 함수
int numORop(int x, int y);	//블록이 숫자인지 연산자인지 판단하는 함수

/* 'DO NOT MODIFY THESE FUNCTIONS.'		'이 함수들을 고치지 마세요.'		 */
void removeCursorFromCMD();
void setCoordinate(int x, int y); //cmd에서 커서를 x, y 위치로 세팅

								  /* display fuctions */
void goMainMenuScreen(); // 메인메뉴로 이동 Moving to main menu.
void printGameScreen();  // 게임화면 출력	 Printing game screen.
void printPauseScreen(); // 일시 정지 화면	 Prining pause screen.
void printEndScreen(int endType); //  // 스테이지 종료 화면 출력. 스테이지클리어, 모든스테이지클리어, 패배.
									//Printing result of end of stages including 'cleared the stage', 'cleared whole stages', and 'lost'.
void clearScreen();
/* 'DO NOT MODIFY THESE FUNCTIONS.' ends	'이 함수들을 고치지 마세요' 끝.	*/

		/* @ --- You can modify following functions ----  이 아래에있는 함수들을 고치세요.  */
//Implement these functions below to complete your project.
//이 함수들을 구현하셔서 프로젝트를 완성하세요.

		/* Block generating functions */
int getRandomNumber();
void newBlock();

/* block control and merging functions  */
int takeBlockControl();				//블록 조작 방향 입력 받음			Taking input for moving blocks
void flipBlock();					// Fliping function.
void moveBlock(int direction);		// 입력 받은 방향으로 블록 이동		Moving blocks to corresponding direction of input key.
int checkAdjacentBlock(int, int);	//Checking adjacent blocks for merging conditions
void checkNumber(int, int);			// 블록 인접 블록숫자 확인			Checking number of adjacent block

		/* game flow control functions */
int isStageEnd();  // 스테이지가 끝났는지 체크. (스테이지 클리어, 모든 스테이지 클리어, 패배)         Checking condition for end of the stage. (stage clear, all stage clear, lost)
void setGameFlow(int type); // 각 스테이지에 맞는 게임 환경 세팅.   Setting up status values for each stages.




int time_interval_moveBlockDown = 100;
int goal = 64;
int currentStage = 0;
int score = 0;
int best_score = 0;
int gameScreen[X][Y] = { 0 };


void main() {
	system("mode con cols=120 lines=38");
	srand(time(NULL));
	removeCursorFromCMD();
	goMainMenuScreen();

	while (1) {

		for (int j = 0; j < 5; j++) {
			if (takeBlockControl() == SPACE) break;
			printGameScreen();
			Sleep(time_interval_moveBlockDown);
		}
		moveBlock(DOWN);
		if (!block.isactive && !block2.isactive) { // 이 부분은 떨어지는 블록이 바닥이나 다른블록에 닿았는지 체크합니다. This statement is cheking that wether the falling blocks got touched by floor or other blocks.
			checkNumber(block.pos_x, block.pos_y);
			if (isStageEnd() != 1)
				newBlock();
		}

	}
}

int getRandomNumber() {
	/*

		일정 확률로 2, 4, 8 중 하나를 생성하여 그 값을 리턴함.
		Generate 2, 4, or 8 with certain percentage and return the number.

		해결

		*/

	int prob = 1 + rand() % 100;	// 1~100까지의 난수 생성

	if (prob <= 50)
		return 2;

	else if (prob > 50 && prob <= 80)
		return 4;

	else if (prob > 80 && prob <= 100)
		return 8;
}

char getRandomOper() {
	/*

	일정 확률로 +, - 중 하나를 생성하여 그 값을 리턴함.
	Generate +, or - with certain percentage and return the number.

	해결

	*/

	int prob = rand() % 2;

	if (currentStage >= 4) {  //  currentStage>=4 로 수정하기. test용으로 바꾸어놓음
		if (prob == 0)
			return '+';
		if (prob == 1)
			return '-'; 
	}

	else
		return '+';

	// You should modify example value for complete this function. 예시값을 고치셔서 함수를 완성하세요.
}


void newBlock() {
	/*

	이 함수는 생성된 값과 떨어지기 시작하는 위치를 입력해서 새 블록을 만듭니다.
	그리고 나서, 다음 블록에 사용될 새 값을 생성합니다.
	This fuction makes new blocks with setting generated value and the initial position where starts falling down.
	After that, generate new number for value of the next block.

	*/
	int random = rand() % 4;	// 0~3까지의 난수 생성
	static int count = 0;
	static int temp, temp2;

	
	// You should modify these example values for complete this function.

	if (count == 0) {
		block.nextnum = getRandomNumber();
		block2.nextnum = getRandomOper();

		block.num = getRandomNumber();
		block2.num = getRandomOper();

		temp = block.nextnum;
		temp2 = block2.nextnum;

		count++;
	}

	else {
		block.num = temp;
		block2.num = temp2;

		block.nextnum = getRandomNumber();
		block2.nextnum = getRandomOper();

		temp = block.nextnum;
		temp2 = block2.nextnum;

	}

	block.pos_x = 0; // 고정
	block.pos_y = random; // 0~3까지의 숫자
	block.isactive = 1;
	gameScreen[block.pos_x][block.pos_y] = block.num;

	block2.pos_x = 0; // 고정
	block2.pos_y = random + 1; // 1~4까지의 숫자
	block2.isactive = 1;
	gameScreen[block2.pos_x][block2.pos_y] = block2.num;
}

int takeBlockControl() {
	int input_blockControl = 0;

	if (_kbhit()) {
		input_blockControl = _getch();
		if (input_blockControl == 224 && block.isactive && block2.isactive) { //방향키인경우  Cases for direction keys
			do { input_blockControl = _getch(); } while (input_blockControl == 224);//방향키지시값을 버림  dropping the value of direction
			switch (input_blockControl) {

				/*

				Implement cases of each dilection key
				각 방향키에 대한 경우들을 구현하세요

				해결

				*/

			case UP:
				flipBlock();
				break;

			case DOWN:
				moveBlock(DOWN);
				break;

			case RIGHT:
				moveBlock(RIGHT);
				break;

			case LEFT:
				moveBlock(LEFT);
				break;

			}
		}
		else { //방향키가 아닌경우  Cases for other keys
			switch (input_blockControl) {
				/*

				Implement hard drop
				하드드랍을 구현하세요

				해결

				*/
			case SPACE:
				while (1) {
					if (block.pos_x < X - 1 && gameScreen[block.pos_x + 1][block.pos_y] == 0) {
						gameScreen[block.pos_x + 1][block.pos_y] = block.num;
						gameScreen[block.pos_x][block.pos_y] = 0;
						block.pos_x++;
					}
					else {
						block.isactive = 0;
					}

					if (block2.pos_x < X - 1 && gameScreen[block2.pos_x + 1][block2.pos_y] == 0) {
						gameScreen[block2.pos_x + 1][block2.pos_y] = block2.num;
						gameScreen[block2.pos_x][block2.pos_y] = 0;
						block2.pos_x++;
					}
					else {
						block2.isactive = 0;
					}

					if (!block.isactive && !block2.isactive) // 블록이 다른블록에 닿았는지 판단
						break;
				}
				score += 5;
				break;
			case P:
			case p:
				printPauseScreen();
				break;
			case ESC:
				printEndScreen(0);
				break;
			}
		}
	}
	while (_kbhit()) _getch(); //키버퍼를 비움. Emptying key buffer.
	return 0;
}

void flipBlock(void) {//좌우 반전함수		Block flipping function
		/*

		Fliping blocks when the 'up' butten is pressed.
		위로 버튼이 눌렸을경우 블록을 바꿉니다.

		해결

		*/
	int temp;

	temp = block.num;
	block.num = block2.num;
	block2.num = temp;
}

void moveBlock(int direction) { // 좌,우,아래 입력시 움직임 함수	Moving blocks for left, reight, and down input
	switch (direction) {
	case LEFT:
		/*

		implement left action

		해결

		*/
		if (block.pos_y > 0 && gameScreen[block.pos_x][block.pos_y - 1] == 0) {
			gameScreen[block.pos_x][block.pos_y - 1] = block.num;
			gameScreen[block.pos_x][block.pos_y] = 0;
			block.pos_y--;
		}
		else {
			block.isactive = 1;
		}

		if (block2.pos_y > 0 && gameScreen[block2.pos_x][block2.pos_y - 1] == 0) {
			gameScreen[block2.pos_x][block2.pos_y - 1] = block2.num;
			gameScreen[block2.pos_x][block2.pos_y] = 0;
			block2.pos_y--;
		}
		else {
			block2.isactive = 1;
		}
		break;

	case RIGHT:
		/*

		implement right action

		해결

		*/

		if (block2.pos_y < Y - 1 && gameScreen[block2.pos_x][block2.pos_y + 1] == 0) {
			gameScreen[block2.pos_x][block2.pos_y + 1] = block2.num;
			gameScreen[block2.pos_x][block2.pos_y] = 0;
			block2.pos_y++;
		}
		else {
			block2.isactive = 1;
		}
		if (block.pos_y < Y - 1 && gameScreen[block.pos_x][block.pos_y + 1] == 0) {
			gameScreen[block.pos_x][block.pos_y + 1] = block.num;
			gameScreen[block.pos_x][block.pos_y] = 0;
			block.pos_y++;
		}
		else {
			block.isactive = 1;
		}
		break;

	case DOWN:
		if (block.pos_x < X - 1 && gameScreen[block.pos_x + 1][block.pos_y] == 0) {
			gameScreen[block.pos_x + 1][block.pos_y] = block.num;
			gameScreen[block.pos_x][block.pos_y] = 0;
			block.pos_x++;
		}
		else {
			block.isactive = 0;
		}

		if (block2.pos_x < X - 1 && gameScreen[block2.pos_x + 1][block2.pos_y] == 0) {
			gameScreen[block2.pos_x + 1][block2.pos_y] = block2.num;
			gameScreen[block2.pos_x][block2.pos_y] = 0;
			block2.pos_x++;
		}
		else {
			block2.isactive = 0;
		}
	}
}

int numORop(int x, int y) {
	static int n;

	if (gameScreen[x][y] == '+')		//현재 블록이 연산자라면 n==PLUS
		n = '+';
	else if (gameScreen[x][y] == '-')		//현재 블록이 연산자라면 n==MINUS
		n = '-';
	else if (gameScreen[x][y] != '+' && gameScreen[x][y] != '-' && gameScreen[x][y] != 0 && gameScreen[x][y] != 999)	//현재 블록이 999,0이 아닌 숫자라면 n==1
		n = 1;

	return n;

}



int checkAdjacentBlock(int x, int y) { //Merging 조건 확인 함수		Checking merging condition
	 /*

	 조건확인하여 연산하고 Merging
	  64 생성시 점수 계산

	 Implement checking condition of Merging.
	 If 64 is found, player earns points.

	 
	 */
	static int count64 = 0;

	//*모양 블록일때 연산X처리
	if (gameScreen[x][y] == 999)
		return 0;

	//현재 블록이 연산자라면, 이웃한 연산자 있을 때 연산X처리
	if (numORop(x, y) != 1) {
		if (gameScreen[x][y - 1] == '+' || gameScreen[x][y - 1] == '-')
			return 0;
		else if (gameScreen[x][y + 1] == '+' || gameScreen[x][y + 1] == '-')
			return 0;
	}
	
	//현재 매개변수 블록이 0이 아닌 숫자라면
	else if (numORop(x,y)) { 

		//오류 수정 위해 따로 항목 만듦
		if (y == 3) {
			if (gameScreen[x][y] == 0 && gameScreen[x - 1][y] != 0) {
				gameScreen[x][y] = gameScreen[x - 1][y];
				gameScreen[x - 1][y] = gameScreen[x - 2][y];
				gameScreen[x - 2][y] = gameScreen[x - 3][y];
				gameScreen[x - 3][y] = gameScreen[x - 4][y];
				gameScreen[x - 4][y] = gameScreen[x - 5][y];
				gameScreen[x - 5][y] = gameScreen[x - 6][y];
				gameScreen[x - 6][y] = gameScreen[x - 7][y];

				for (int i = 0; i < Y; i++) {
					gameScreen[0][i] = 0;
				}
				
			}
			return 0;
		}


		//우선순위1 가로 합성

		//매개변수 블록 오른쪽에 +연산자, 그 오른쪽에 0이 아닌 숫자가 위치했을 경우
		if (gameScreen[x][y + 1] == '+' && gameScreen[x][y + 2] != '+' && gameScreen[x][y + 2] != '-' && gameScreen[x][y + 2] != 0 && gameScreen[x][y + 2] != 999) {
			
			//중복 계산 처리
			//y==0일 때만 발생.
			if(y==0){
				// x + y + z 경우 처리
				if (gameScreen[x][3] == '+' && gameScreen[x][4] != '+' && gameScreen[x][4] != '-' && gameScreen[x][4] != 0 && gameScreen[x][4] != 999) {
					gameScreen[x][0] = gameScreen[x][0] + gameScreen[x][2] + gameScreen[x][4];
					gameScreen[x][1] = 0;
					gameScreen[x][2] = 0;
					gameScreen[x][3] = 0;
					gameScreen[x][4] = 0;
					return 1;
				}

				// x + y - z 경우 처리
				if (gameScreen[x][3] == '-' && gameScreen[x][4] != '+' && gameScreen[x][4] != '-' && gameScreen[x][4] != 0 && gameScreen[x][4] != 999){
					if (gameScreen[x][0] + gameScreen[x][2] - gameScreen[x][4] > 0) {
						gameScreen[x][0] = gameScreen[x][0] + gameScreen[x][2] - gameScreen[x][4];
					}

					else if (gameScreen[x][0] + gameScreen[x][2] - gameScreen[x][4] < 0) {
						gameScreen[x][0] = -(gameScreen[x][0] + gameScreen[x][2] - gameScreen[x][4]);
					}

					else {
						gameScreen[x][0] = 999;
					}

					gameScreen[x][1] = 0;
					gameScreen[x][2] = 0;
					gameScreen[x][3] = 0;
					gameScreen[x][4] = 0;
				}

			}//중복 계산 처리 끝


			//단일 계산 처리
			if (gameScreen[x][2] == 0)
				return 0;

			else {
				gameScreen[x][y] += gameScreen[x][y + 2];
				gameScreen[x][y + 1] = 0;
				gameScreen[x][y + 2] = 0;

				return 1;

			}

		}

		//매개변수 블록 오른쪽에 -연산자, 그 오른쪽에 0이 아닌 숫자가 위치했을 경우
		else if (gameScreen[x][y + 1] == '-' && gameScreen[x][y + 2] != '+' && gameScreen[x][y + 2] != '-' && gameScreen[x][y + 2] != 0 && gameScreen[x][y + 2] != 999) {
			
			//중복 계산 처리
			//y==0일 때만 발생.
			if (y == 0) {
				// x - y + z 경우 처리
				if (gameScreen[x][3] == '+' && gameScreen[x][4] != '+' && gameScreen[x][4] != '-' && gameScreen[x][4] != 0 && gameScreen[x][4] != 999) {
					if (gameScreen[x][0] - gameScreen[x][2] + gameScreen[x][4] > 0) {
						gameScreen[x][0] = gameScreen[x][0] - gameScreen[x][2] + gameScreen[x][4];
					}

					else if (gameScreen[x][0] - gameScreen[x][2] + gameScreen[x][4] < 0) {
						gameScreen[x][0] = -(gameScreen[x][0] - gameScreen[x][2] + gameScreen[x][4]);
					}

					else {
						gameScreen[x][0] = 999;
					}
					gameScreen[x][1] = 0;
					gameScreen[x][2] = 0;
					gameScreen[x][3] = 0;
					gameScreen[x][4] = 0;
					return 1;
				}

				// x - y - z 경우 처리
				if (gameScreen[x][3] == '-' && gameScreen[x][4] != '+' && gameScreen[x][4] != '-' && gameScreen[x][4] != 0 && gameScreen[x][4] != 999) {
					if (gameScreen[x][0] - gameScreen[x][2] - gameScreen[x][4] > 0) {
						gameScreen[x][0] = gameScreen[x][0] - gameScreen[x][2] - gameScreen[x][4];
					}

					else if (gameScreen[x][0] + gameScreen[x][2] - gameScreen[x][4] < 0) {
						gameScreen[x][0] = -(gameScreen[x][0] - gameScreen[x][2] - gameScreen[x][4]);
					}

					else {
						gameScreen[x][0] = 999;
					}

					gameScreen[x][1] = 0;
					gameScreen[x][2] = 0;
					gameScreen[x][3] = 0;
					gameScreen[x][4] = 0;
				}

			}//중복 계산 처리 끝


			//단일 계산 처리
			if (gameScreen[x][2] == 0)
				return 0;

			else {
			if (gameScreen[x][y] > gameScreen[x][y + 2])	    // 원래 블록의 숫자가 클 경우, 그대로 뺄셈
				gameScreen[x][y] -= gameScreen[x][y + 2];
			else if (gameScreen[x][y] < gameScreen[x][y + 2]) { // 원래 블록의 숫자가 작을 경우, 순서 바꾸어 뺄셈
				gameScreen[x][y] -= gameScreen[x][y + 2];
				gameScreen[x][y] *= -1;
			}
			else if (gameScreen[x][y] == gameScreen[x][y + 2])    // 두 블록의 숫자가 같을 경우, *처리
				gameScreen[x][y] = 999; 
			gameScreen[x][y + 1] = 0;
			gameScreen[x][y + 2] = 0;
			return 1;

			}
			
		}


	// 우선순위2 세로 합성 

		//매개변수 블록 위쪽에 +연산자, 그 위쪽에 0이 아닌 숫자가 위치했을 경우
		if (gameScreen[x - 1][y] == '+' && gameScreen[x - 2][y] != '+' && gameScreen[x - 2][y] != '-' && gameScreen[x - 2][y] != 0 && gameScreen[x - 2][y] != 999) {
			// 만약 
			gameScreen[x][y] += gameScreen[x-2][y];
			gameScreen[x-1][y] = 0;
			gameScreen[x-2][y] = 0;
			return 1;
		}

		//매개변수 블록 위쪽에 -연산자, 그 위쪽에 0이 아닌 숫자가 위치했을 경우
		else if (gameScreen[x-1][y] == '-' && gameScreen[x - 2][y] != '+' && gameScreen[x - 2][y] != '-' && gameScreen[x - 2][y] != 0 && gameScreen[x - 2][y] != 999) {
			if (gameScreen[x][y] > gameScreen[x - 2][y])	    // 원래 블록의 숫자가 클 경우, 그대로 뺄셈
				gameScreen[x][y] -= gameScreen[x - 2][y];
			else if (gameScreen[x][y] < gameScreen[x - 2][y]) { // 원래 블록의 숫자가 작을 경우, 순서 바꾸어 뺄셈
				gameScreen[x][y] -= gameScreen[x - 2][y];
				gameScreen[x][y] *= -1;
			}
			else if (gameScreen[x][y] == gameScreen[x - 2][y])    // 두 블록의 숫자가 같을 경우, *처리
				gameScreen[x][y] = 999; // 해결 : 42로 표현되는 문제 수정
			gameScreen[x - 1][y] = 0;
			gameScreen[x - 2][y] = 0;
			return 1;
		}


	} //현재 블록이 숫자라면 끝



	//블록이 64에 도달
	if (gameScreen[x][y] == 64) {
		gameScreen[x][y] = 0;
		count64++;		

		score += 10 * (count64 - 1);

	}

	//합성된 후 블록이 없어지면 윗 블록이 떨어지도록 하기
	if (gameScreen[x][y] == 0 && gameScreen[x - 1][y] != 0) {
		gameScreen[x][y] = gameScreen[x - 1][y];
		gameScreen[x - 1][y] = gameScreen[x - 2][y];
		gameScreen[x - 2][y] = gameScreen[x - 3][y];
		gameScreen[x - 3][y] = gameScreen[x - 4][y];
		gameScreen[x - 4][y] = gameScreen[x - 5][y];
		gameScreen[x - 5][y] = gameScreen[x - 6][y];
		gameScreen[x - 6][y] = gameScreen[x - 7][y];

		for (int i = 0; i < Y; i++) {
			gameScreen[0][i] = 0;
		}
	}




	return 0; //example value. 0 for none, 1 for mergable.  예시값. 0이면 합칠 블록이 없고, 1이면 있음.
}

void checkNumber(int x, int y) {

	checkAdjacentBlock(x, y);
	printGameScreen();
	Sleep(200);

	for (int i = X - 1; i > 0; i--) {
		for (int j = 0; j < Y; j++) {
			if (checkAdjacentBlock(i, j)) {
				printGameScreen();
				Sleep(200);
				i = X;
				break;
			}
		}
	}
}

void setGameFlow(int setGameFlowType) { // Implement this function to set conditions of stages.  스테이지의 조건들을 설정하기위해 이 함수를 완성하세요.
	switch (setGameFlowType) {
	case 0:  /* initialize game */
		if (best_score < score)
			best_score = score;

		currentStage = 0;
		score = 0;

	case 1: /* To next stage */
		currentStage++;

		switch (currentStage) {
		case 1:
			time_interval_moveBlockDown = 100;
			goal = 100;
			break;
		case 2:
			time_interval_moveBlockDown = 130;
			goal = 300;
			break;
		case 3:
			time_interval_moveBlockDown = 150;
			goal = 500;
			break;
		case 4:
			time_interval_moveBlockDown = 170;
			goal = 600;
			break;
		case 5:
			time_interval_moveBlockDown = 200;
			goal = 750;
			break;
		case 6:
			time_interval_moveBlockDown = 250;
			goal = 850;
			break;
		}
		clearScreen();
		newBlock();
		break;

	default:
		clearScreen();
		setCoordinate(MAP_ADJ_X + 2, MAP_ADJ_Y + 13); printf(" @System error: state end condition currupted.");
		exit(0);
	}
}

int isStageEnd() { // 스테이지가 끝났는지 체크      Checking if stage ended up or not.
	for (int i = X - 1; i > 1; i--) {
		for (int j = 0; j < Y; j++) {
			if (score >= goal) {
				currentStage < 6 ? printEndScreen(1) : printEndScreen(2);
				return 1;
			}
		}
	}
	for (int i = 0; i < Y; i++) {
		if (gameScreen[0][i] != 0) { //패배			Game over
			printEndScreen(0);
			return 1;
		}
	}
	return 0;
}


/* @ ↓↓↓'DO NOT MODIFY FROM HERE.'↓↓↓ starts		 이 아래로는 수정하지 마세요.	*/
void removeCursorFromCMD() { // cmd에서 깜빡이는 커서없앰 Concealing cursor from cmd.
	CONSOLE_CURSOR_INFO CurInfo;
	CurInfo.dwSize = 1;
	CurInfo.bVisible = FALSE;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CurInfo);
}

void setCoordinate(int x, int y) { //cmd에서 커서 위치를 x, y로 세팅   Setting curser to (x,y).
	COORD pos = { 2 * x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void printGameScreen() {
	int i, j, k;

	for (i = 0; i < X; i++) {
		for (j = 0; j < Y; j++) {
			setCoordinate(MAP_ADJ_X + (j * 6), MAP_ADJ_Y + (i * 3));
			if (gameScreen[i][j] == 0) {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
				if (i == 0) {
					setCoordinate(MAP_ADJ_X + (j * 6), MAP_ADJ_Y + (i * 3) - 1);
					printf("             ");
					setCoordinate(MAP_ADJ_X + (j * 6), MAP_ADJ_Y + (i * 3));
					printf("     ┿      ");
					setCoordinate(MAP_ADJ_X + (j * 6), MAP_ADJ_Y + (i * 3) + 1);
					printf("             ");
					setCoordinate(MAP_ADJ_X + (j * 6), MAP_ADJ_Y + (i * 3) + 2);
					printf("-------------");
				}
				else {
					setCoordinate(MAP_ADJ_X + (j * 6), MAP_ADJ_Y + (i * 3));
					printf("             ");
					setCoordinate(MAP_ADJ_X + (j * 6), MAP_ADJ_Y + (i * 3) + 1);
					printf("     ┿      ");
					setCoordinate(MAP_ADJ_X + (j * 6), MAP_ADJ_Y + (i * 3) + 2);
					printf("             ");
				}
			}

			else {
				for (k = 0; k < 12;) if (1 << (++k) == gameScreen[i][j]) break;
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), k + 1);
				if (gameScreen[i][j] == 45 || gameScreen[i][j] == 43)
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);

				if (i == 0) {
					setCoordinate(MAP_ADJ_X + (j * 6), MAP_ADJ_Y + (i * 3) - 1);
					printf("┌─────────┐ ");
					setCoordinate(MAP_ADJ_X + (j * 6), MAP_ADJ_Y + (i * 3));
					if (gameScreen[i][j] == 45)
						printf("   %4c", gameScreen[i][j]);
					else if (gameScreen[i][j] == 43)
						printf("   %4c", gameScreen[i][j]);
					else if (gameScreen[i][j] == 999)
						printf("      *", gameScreen[i][j]);
					else
						printf("   %4d", gameScreen[i][j]);
					setCoordinate(MAP_ADJ_X + (j * 6), MAP_ADJ_Y + (i * 3) + 1);
					printf("└─────────┘ ");
					setCoordinate(MAP_ADJ_X + (j * 6), MAP_ADJ_Y + (i * 3) + 2);

				}
				else {
					setCoordinate(MAP_ADJ_X + (j * 6), MAP_ADJ_Y + (i * 3));
					printf("┌─────────┐  ");
					setCoordinate(MAP_ADJ_X + (j * 6), MAP_ADJ_Y + (i * 3) + 1);
					if (gameScreen[i][j] == 45)
						printf("   %4c", gameScreen[i][j]);
					else if (gameScreen[i][j] == 43)
						printf("   %4c", gameScreen[i][j]);
					else if (gameScreen[i][j] == 999)
						printf("      *", gameScreen[i][j]);
					else
						printf("   %4d", gameScreen[i][j]);
					setCoordinate(MAP_ADJ_X + (j * 6), MAP_ADJ_Y + (i * 3) + 2);
					printf("└─────────┘  ");
				}

			}
		}
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y); printf(" Stage : %4d", currentStage);
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 1); printf(" GOAL  : %4d", goal);
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 2); printf(" SPEED  : %.1f", 100.0 / (float)time_interval_moveBlockDown);
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 3); printf("+-  N E X T  -+ ");
	for (k = 0; k < 12;) if (1 << (++k) == block.nextnum) break;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), k + 1);
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 4); printf("  ┌─────────┐    ");
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 5); printf("     %4d    ", block.nextnum);
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 6); printf("  └─────────┘    ");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 7); printf("  ┌─────────┐    ");
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 8); printf("     %4c    ", block2.nextnum);
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 9); printf("  └─────────┘    ");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 10); printf("+-- -  -  - --+ ");
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 11); printf(" YOUR SCORE :");
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 12); printf("        %6d", score);
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 13); printf(" BEST SCORE :");
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 14); printf("        %6d", best_score);

	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 15); printf("  △   : Flip");
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 16); printf("◁  ▷ : Left / Right");
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 17); printf("  ▽   : Soft Drop ");
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 18); printf(" SPACE : Hard Drop");
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 19); printf("   P   : Pause");
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 20); printf("  ESC  : Quit");

}

void goMainMenuScreen() { //시작화면 출력 함수. Printing out main menu.
	int x = 19;
	int y = 9;
	int input_command = 0;

	setCoordinate(x, y + 2); printf("──────────────────────────────────");
	Sleep(30);
	setCoordinate(x, y + 2); printf("☆ ────────────────────────────");
	Sleep(30);
	setCoordinate(x, y + 2); printf("o ☆ ─────────────────────────");
	Sleep(30);
	setCoordinate(x, y + 2); printf("yo ☆ ────────────────────────");
	Sleep(30);
	setCoordinate(x, y + 2); printf("yoPuyo ☆ ─────────────────────");
	Sleep(30);
	setCoordinate(x, y + 2); printf("PuyoPuyo ☆ ────────────────────");
	Sleep(30);
	setCoordinate(x, y + 2); printf("─ 64 PuyoPuyo ☆─────────────────");
	Sleep(30);
	setCoordinate(x, y + 2); printf("─── 64 PuyoPuyo ☆ ───────────────");
	Sleep(30);
	setCoordinate(x, y + 2); printf("──── 64 PuyoPuyo ☆ ──────────────");
	Sleep(30);
	setCoordinate(x, y + 2); printf("─────  64 PuyoPuyo ☆ ────────────");
	Sleep(30);
	setCoordinate(x, y + 2); printf("───────  64 PuyoPuyo ☆ ──────────");
	Sleep(30);
	setCoordinate(x, y + 2); printf("────────  64 PuyoPuyo ☆ ─────────");
	Sleep(30);
	setCoordinate(x, y + 2); printf("─────────  64 PuyoPuyo ☆ ────────");
	Sleep(30);
	setCoordinate(x, y + 2); printf("──────────  64 PuyoPuyo ☆ ───────");
	Sleep(30);
	setCoordinate(x, y + 2); printf("───────────  64 PuyoPuyo ☆ ──────");
	Sleep(30);
	setCoordinate(x, y + 2); printf("────────────  64 PuyoPuyo ☆ ─────");
	Sleep(30);
	setCoordinate(x, y + 2); printf("─────────────  64 PuyoPuyo ☆ ────");
	Sleep(30);
	setCoordinate(x, y + 2); printf("──────────────  64 PuyoPuyo ☆ ───");
	Sleep(30);
	setCoordinate(x, y + 2); printf("───────────────  64 PuyoPuyo ☆ ──");
	Sleep(30);
	setCoordinate(x, y + 2); printf("────────────────  64 PuyoPuyo ☆ ─");


	setCoordinate(x + 2, y + 4); printf(" Press any key to start");

	setCoordinate(x, y + 6);  printf(" ▤▤▤▤ HOW TO CONTROL ▤▤▤▤");
	setCoordinate(x, y + 7);  printf(" ▤                            ▤");
	setCoordinate(x, y + 8);  printf(" ▤      △   : Flip           ▤");
	setCoordinate(x, y + 9); printf(" ▤    ◁  ▷ : Left / Right   ▤");
	setCoordinate(x, y + 10); printf(" ▤      ▽   : Soft Drop      ▤");
	setCoordinate(x, y + 11); printf(" ▤    SPACE  : Hard Drop      ▤");
	setCoordinate(x, y + 12); printf(" ▤      P    : Pause          ▤");
	setCoordinate(x, y + 13); printf(" ▤     ESC   : Quit           ▤");
	setCoordinate(x, y + 14); printf(" ▤                            ▤");
	setCoordinate(x, y + 15); printf(" ▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤");

	setCoordinate(x, y + 16); printf("          ESC: Exit game.     ");



	while (_kbhit())
		_getch();
	input_command = _getch();


	if (input_command == ESC) {
		clearScreen();
		exit(0);
	}
	else
		setGameFlow(0); /* Reset the game */

}

void printPauseScreen() { // 게임 일시정지		Pause game.
	int x = MAP_ADJ_X + 8;
	int y = MAP_ADJ_Y + 5;

	setCoordinate(x, y + 0); printf("▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤");
	setCoordinate(x, y + 1); printf("▤                              ▤");
	setCoordinate(x, y + 2); printf("▤  +-----------------------+   ▤");
	setCoordinate(x, y + 3); printf("▤  |       P A U S E       |   ▤");
	setCoordinate(x, y + 4); printf("▤  +-----------------------+   ▤");
	setCoordinate(x, y + 5); printf("▤   Press any key to resume.   ▤");
	setCoordinate(x, y + 6); printf("▤                              ▤");
	setCoordinate(x, y + 7); printf("▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤");

	_getch();

	system("cls");
	printGameScreen();
}



void printEndScreen(int endType) {
	int input_command = 0;
	int x = MAP_ADJ_X + 8;

	switch (endType) {
	case 0:
		setCoordinate(x, MAP_ADJ_Y + 5);  printf("▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤");
		setCoordinate(x, MAP_ADJ_Y + 6);  printf("▤                              ▤");
		setCoordinate(x, MAP_ADJ_Y + 7);  printf("▤  +-----------------------+   ▤");
		setCoordinate(x, MAP_ADJ_Y + 8);  printf("▤  |  G A M E  O V E R..   |   ▤");
		setCoordinate(x, MAP_ADJ_Y + 9);  printf("▤  +-----------------------+   ▤");
		setCoordinate(x, MAP_ADJ_Y + 10); printf("▤   YOUR SCORE: %6d         ▤", score);
		setCoordinate(x, MAP_ADJ_Y + 11); printf("▤                              ▤");
		setCoordinate(x, MAP_ADJ_Y + 12); printf("▤   Press any key to restart.  ▤");
		setCoordinate(x, MAP_ADJ_Y + 13); printf("▤                              ▤");
		setCoordinate(x, MAP_ADJ_Y + 14); printf("▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤");
		setCoordinate(x, MAP_ADJ_Y + 16); printf("      ESC: Back to main menu.     ");

		while (_kbhit()) _getch();
		input_command = _getch();

		if (input_command == ESC) {
			while (_kbhit()) _getch();
			clearScreen();
			goMainMenuScreen();
			return;
		}

		setGameFlow(0); /* Reset the game */
		break;

	case 1:
		setCoordinate(x, MAP_ADJ_Y + 5);  printf("▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤");
		setCoordinate(x, MAP_ADJ_Y + 6);  printf("▤                              ▤");
		setCoordinate(x, MAP_ADJ_Y + 7);  printf("▤  +-----------------------+   ▤");
		setCoordinate(x, MAP_ADJ_Y + 8);  printf("▤  |      STAGE CLEAR      |   ▤");
		setCoordinate(x, MAP_ADJ_Y + 9);  printf("▤  +-----------------------+   ▤");
		setCoordinate(x, MAP_ADJ_Y + 10); printf("▤   YOUR SCORE: %6d         ▤", score);
		setCoordinate(x, MAP_ADJ_Y + 11); printf("▤                              ▤");
		setCoordinate(x, MAP_ADJ_Y + 12); printf("▤ Press any key to next stage. ▤");
		setCoordinate(x, MAP_ADJ_Y + 13); printf("▤                              ▤");
		setCoordinate(x, MAP_ADJ_Y + 14); printf("▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤");

		while (_kbhit()) _getch();
		input_command = _getch();

		setGameFlow(1); /* To next stage */
		break;

	case 2:
		setCoordinate(x, MAP_ADJ_Y + 5);  printf("▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤");
		setCoordinate(x, MAP_ADJ_Y + 6);  printf("▤                                      ▤");
		setCoordinate(x, MAP_ADJ_Y + 7);  printf("▤  +-------------------------------+   ▤");
		setCoordinate(x, MAP_ADJ_Y + 8);  printf("▤  | WINNER WINNER CHICKEN DINNER! |   ▤");
		setCoordinate(x, MAP_ADJ_Y + 9);  printf("▤  +-------------------------------+   ▤");
		setCoordinate(x, MAP_ADJ_Y + 10); printf("▤         YOUR SCORE: %6d           ▤", score);
		setCoordinate(x, MAP_ADJ_Y + 11); printf("▤                                      ▤");
		setCoordinate(x, MAP_ADJ_Y + 12); printf("▤         Cleared final stage.         ▤");
		setCoordinate(x, MAP_ADJ_Y + 13); printf("▤                                      ▤");
		setCoordinate(x, MAP_ADJ_Y + 14); printf("▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤");

		while (_kbhit()) _getch();
		input_command = _getch();
		setGameFlow(0); /* Reset the game */
		break;

	default:
		clearScreen();
		setCoordinate(x, MAP_ADJ_Y + 13); printf(" @System error: state end condition currupted.");
		exit(0);
	}
}
void clearScreen() {
	for (int i = 0; i < X; i++) {
		for (int j = 0; j < Y; j++) {
			gameScreen[i][j] = 0;
		}
	}
	block.pos_x = 0;
	block.pos_x = 0;
	block.num = 0;
	block.nextnum = 0;
	block.isactive = 0;

	block2.pos_x = 0;
	block2.pos_x = 0;
	block2.num = 0;
	block2.nextnum = 0;
	block2.isactive = 0;

	system("cls");
}
/* @ '↑↑↑DO NOT MODIFY.↑↑↑'ends. */