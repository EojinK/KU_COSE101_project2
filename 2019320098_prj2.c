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

// ���� �Լ�
int numORop(int x, int y);	//����� �������� ���������� �Ǵ��ϴ� �Լ�

/* 'DO NOT MODIFY THESE FUNCTIONS.'		'�� �Լ����� ��ġ�� ������.'		 */
void removeCursorFromCMD();
void setCoordinate(int x, int y); //cmd���� Ŀ���� x, y ��ġ�� ����

								  /* display fuctions */
void goMainMenuScreen(); // ���θ޴��� �̵� Moving to main menu.
void printGameScreen();  // ����ȭ�� ���	 Printing game screen.
void printPauseScreen(); // �Ͻ� ���� ȭ��	 Prining pause screen.
void printEndScreen(int endType); //  // �������� ���� ȭ�� ���. ��������Ŭ����, ��罺������Ŭ����, �й�.
									//Printing result of end of stages including 'cleared the stage', 'cleared whole stages', and 'lost'.
void clearScreen();
/* 'DO NOT MODIFY THESE FUNCTIONS.' ends	'�� �Լ����� ��ġ�� ������' ��.	*/

		/* @ --- You can modify following functions ----  �� �Ʒ����ִ� �Լ����� ��ġ����.  */
//Implement these functions below to complete your project.
//�� �Լ����� �����ϼż� ������Ʈ�� �ϼ��ϼ���.

		/* Block generating functions */
int getRandomNumber();
void newBlock();

/* block control and merging functions  */
int takeBlockControl();				//��� ���� ���� �Է� ����			Taking input for moving blocks
void flipBlock();					// Fliping function.
void moveBlock(int direction);		// �Է� ���� �������� ��� �̵�		Moving blocks to corresponding direction of input key.
int checkAdjacentBlock(int, int);	//Checking adjacent blocks for merging conditions
void checkNumber(int, int);			// ��� ���� ��ϼ��� Ȯ��			Checking number of adjacent block

		/* game flow control functions */
int isStageEnd();  // ���������� �������� üũ. (�������� Ŭ����, ��� �������� Ŭ����, �й�)         Checking condition for end of the stage. (stage clear, all stage clear, lost)
void setGameFlow(int type); // �� ���������� �´� ���� ȯ�� ����.   Setting up status values for each stages.




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
		if (!block.isactive && !block2.isactive) { // �� �κ��� �������� ����� �ٴ��̳� �ٸ���Ͽ� ��Ҵ��� üũ�մϴ�. This statement is cheking that wether the falling blocks got touched by floor or other blocks.
			checkNumber(block.pos_x, block.pos_y);
			if (isStageEnd() != 1)
				newBlock();
		}

	}
}

int getRandomNumber() {
	/*

		���� Ȯ���� 2, 4, 8 �� �ϳ��� �����Ͽ� �� ���� ������.
		Generate 2, 4, or 8 with certain percentage and return the number.

		�ذ�

		*/

	int prob = 1 + rand() % 100;	// 1~100������ ���� ����

	if (prob <= 50)
		return 2;

	else if (prob > 50 && prob <= 80)
		return 4;

	else if (prob > 80 && prob <= 100)
		return 8;
}

char getRandomOper() {
	/*

	���� Ȯ���� +, - �� �ϳ��� �����Ͽ� �� ���� ������.
	Generate +, or - with certain percentage and return the number.

	�ذ�

	*/

	int prob = rand() % 2;

	if (currentStage >= 4) {  //  currentStage>=4 �� �����ϱ�. test������ �ٲپ����
		if (prob == 0)
			return '+';
		if (prob == 1)
			return '-'; 
	}

	else
		return '+';

	// You should modify example value for complete this function. ���ð��� ��ġ�ż� �Լ��� �ϼ��ϼ���.
}


void newBlock() {
	/*

	�� �Լ��� ������ ���� �������� �����ϴ� ��ġ�� �Է��ؼ� �� ����� ����ϴ�.
	�׸��� ����, ���� ��Ͽ� ���� �� ���� �����մϴ�.
	This fuction makes new blocks with setting generated value and the initial position where starts falling down.
	After that, generate new number for value of the next block.

	*/
	int random = rand() % 4;	// 0~3������ ���� ����
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

	block.pos_x = 0; // ����
	block.pos_y = random; // 0~3������ ����
	block.isactive = 1;
	gameScreen[block.pos_x][block.pos_y] = block.num;

	block2.pos_x = 0; // ����
	block2.pos_y = random + 1; // 1~4������ ����
	block2.isactive = 1;
	gameScreen[block2.pos_x][block2.pos_y] = block2.num;
}

int takeBlockControl() {
	int input_blockControl = 0;

	if (_kbhit()) {
		input_blockControl = _getch();
		if (input_blockControl == 224 && block.isactive && block2.isactive) { //����Ű�ΰ��  Cases for direction keys
			do { input_blockControl = _getch(); } while (input_blockControl == 224);//����Ű���ð��� ����  dropping the value of direction
			switch (input_blockControl) {

				/*

				Implement cases of each dilection key
				�� ����Ű�� ���� ������ �����ϼ���

				�ذ�

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
		else { //����Ű�� �ƴѰ��  Cases for other keys
			switch (input_blockControl) {
				/*

				Implement hard drop
				�ϵ����� �����ϼ���

				�ذ�

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

					if (!block.isactive && !block2.isactive) // ����� �ٸ���Ͽ� ��Ҵ��� �Ǵ�
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
	while (_kbhit()) _getch(); //Ű���۸� ���. Emptying key buffer.
	return 0;
}

void flipBlock(void) {//�¿� �����Լ�		Block flipping function
		/*

		Fliping blocks when the 'up' butten is pressed.
		���� ��ư�� ��������� ����� �ٲߴϴ�.

		�ذ�

		*/
	int temp;

	temp = block.num;
	block.num = block2.num;
	block2.num = temp;
}

void moveBlock(int direction) { // ��,��,�Ʒ� �Է½� ������ �Լ�	Moving blocks for left, reight, and down input
	switch (direction) {
	case LEFT:
		/*

		implement left action

		�ذ�

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

		�ذ�

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

	if (gameScreen[x][y] == '+')		//���� ����� �����ڶ�� n==PLUS
		n = '+';
	else if (gameScreen[x][y] == '-')		//���� ����� �����ڶ�� n==MINUS
		n = '-';
	else if (gameScreen[x][y] != '+' && gameScreen[x][y] != '-' && gameScreen[x][y] != 0 && gameScreen[x][y] != 999)	//���� ����� 999,0�� �ƴ� ���ڶ�� n==1
		n = 1;

	return n;

}



int checkAdjacentBlock(int x, int y) { //Merging ���� Ȯ�� �Լ�		Checking merging condition
	 /*

	 ����Ȯ���Ͽ� �����ϰ� Merging
	  64 ������ ���� ���

	 Implement checking condition of Merging.
	 If 64 is found, player earns points.

	 
	 */
	static int count64 = 0;

	//*��� ����϶� ����Xó��
	if (gameScreen[x][y] == 999)
		return 0;

	//���� ����� �����ڶ��, �̿��� ������ ���� �� ����Xó��
	if (numORop(x, y) != 1) {
		if (gameScreen[x][y - 1] == '+' || gameScreen[x][y - 1] == '-')
			return 0;
		else if (gameScreen[x][y + 1] == '+' || gameScreen[x][y + 1] == '-')
			return 0;
	}
	
	//���� �Ű����� ����� 0�� �ƴ� ���ڶ��
	else if (numORop(x,y)) { 

		//���� ���� ���� ���� �׸� ����
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


		//�켱����1 ���� �ռ�

		//�Ű����� ��� �����ʿ� +������, �� �����ʿ� 0�� �ƴ� ���ڰ� ��ġ���� ���
		if (gameScreen[x][y + 1] == '+' && gameScreen[x][y + 2] != '+' && gameScreen[x][y + 2] != '-' && gameScreen[x][y + 2] != 0 && gameScreen[x][y + 2] != 999) {
			
			//�ߺ� ��� ó��
			//y==0�� ���� �߻�.
			if(y==0){
				// x + y + z ��� ó��
				if (gameScreen[x][3] == '+' && gameScreen[x][4] != '+' && gameScreen[x][4] != '-' && gameScreen[x][4] != 0 && gameScreen[x][4] != 999) {
					gameScreen[x][0] = gameScreen[x][0] + gameScreen[x][2] + gameScreen[x][4];
					gameScreen[x][1] = 0;
					gameScreen[x][2] = 0;
					gameScreen[x][3] = 0;
					gameScreen[x][4] = 0;
					return 1;
				}

				// x + y - z ��� ó��
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

			}//�ߺ� ��� ó�� ��


			//���� ��� ó��
			if (gameScreen[x][2] == 0)
				return 0;

			else {
				gameScreen[x][y] += gameScreen[x][y + 2];
				gameScreen[x][y + 1] = 0;
				gameScreen[x][y + 2] = 0;

				return 1;

			}

		}

		//�Ű����� ��� �����ʿ� -������, �� �����ʿ� 0�� �ƴ� ���ڰ� ��ġ���� ���
		else if (gameScreen[x][y + 1] == '-' && gameScreen[x][y + 2] != '+' && gameScreen[x][y + 2] != '-' && gameScreen[x][y + 2] != 0 && gameScreen[x][y + 2] != 999) {
			
			//�ߺ� ��� ó��
			//y==0�� ���� �߻�.
			if (y == 0) {
				// x - y + z ��� ó��
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

				// x - y - z ��� ó��
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

			}//�ߺ� ��� ó�� ��


			//���� ��� ó��
			if (gameScreen[x][2] == 0)
				return 0;

			else {
			if (gameScreen[x][y] > gameScreen[x][y + 2])	    // ���� ����� ���ڰ� Ŭ ���, �״�� ����
				gameScreen[x][y] -= gameScreen[x][y + 2];
			else if (gameScreen[x][y] < gameScreen[x][y + 2]) { // ���� ����� ���ڰ� ���� ���, ���� �ٲپ� ����
				gameScreen[x][y] -= gameScreen[x][y + 2];
				gameScreen[x][y] *= -1;
			}
			else if (gameScreen[x][y] == gameScreen[x][y + 2])    // �� ����� ���ڰ� ���� ���, *ó��
				gameScreen[x][y] = 999; 
			gameScreen[x][y + 1] = 0;
			gameScreen[x][y + 2] = 0;
			return 1;

			}
			
		}


	// �켱����2 ���� �ռ� 

		//�Ű����� ��� ���ʿ� +������, �� ���ʿ� 0�� �ƴ� ���ڰ� ��ġ���� ���
		if (gameScreen[x - 1][y] == '+' && gameScreen[x - 2][y] != '+' && gameScreen[x - 2][y] != '-' && gameScreen[x - 2][y] != 0 && gameScreen[x - 2][y] != 999) {
			// ���� 
			gameScreen[x][y] += gameScreen[x-2][y];
			gameScreen[x-1][y] = 0;
			gameScreen[x-2][y] = 0;
			return 1;
		}

		//�Ű����� ��� ���ʿ� -������, �� ���ʿ� 0�� �ƴ� ���ڰ� ��ġ���� ���
		else if (gameScreen[x-1][y] == '-' && gameScreen[x - 2][y] != '+' && gameScreen[x - 2][y] != '-' && gameScreen[x - 2][y] != 0 && gameScreen[x - 2][y] != 999) {
			if (gameScreen[x][y] > gameScreen[x - 2][y])	    // ���� ����� ���ڰ� Ŭ ���, �״�� ����
				gameScreen[x][y] -= gameScreen[x - 2][y];
			else if (gameScreen[x][y] < gameScreen[x - 2][y]) { // ���� ����� ���ڰ� ���� ���, ���� �ٲپ� ����
				gameScreen[x][y] -= gameScreen[x - 2][y];
				gameScreen[x][y] *= -1;
			}
			else if (gameScreen[x][y] == gameScreen[x - 2][y])    // �� ����� ���ڰ� ���� ���, *ó��
				gameScreen[x][y] = 999; // �ذ� : 42�� ǥ���Ǵ� ���� ����
			gameScreen[x - 1][y] = 0;
			gameScreen[x - 2][y] = 0;
			return 1;
		}


	} //���� ����� ���ڶ�� ��



	//����� 64�� ����
	if (gameScreen[x][y] == 64) {
		gameScreen[x][y] = 0;
		count64++;		

		score += 10 * (count64 - 1);

	}

	//�ռ��� �� ����� �������� �� ����� ���������� �ϱ�
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




	return 0; //example value. 0 for none, 1 for mergable.  ���ð�. 0�̸� ��ĥ ����� ����, 1�̸� ����.
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

void setGameFlow(int setGameFlowType) { // Implement this function to set conditions of stages.  ���������� ���ǵ��� �����ϱ����� �� �Լ��� �ϼ��ϼ���.
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

int isStageEnd() { // ���������� �������� üũ      Checking if stage ended up or not.
	for (int i = X - 1; i > 1; i--) {
		for (int j = 0; j < Y; j++) {
			if (score >= goal) {
				currentStage < 6 ? printEndScreen(1) : printEndScreen(2);
				return 1;
			}
		}
	}
	for (int i = 0; i < Y; i++) {
		if (gameScreen[0][i] != 0) { //�й�			Game over
			printEndScreen(0);
			return 1;
		}
	}
	return 0;
}


/* @ ����'DO NOT MODIFY FROM HERE.'���� starts		 �� �Ʒ��δ� �������� ������.	*/
void removeCursorFromCMD() { // cmd���� �����̴� Ŀ������ Concealing cursor from cmd.
	CONSOLE_CURSOR_INFO CurInfo;
	CurInfo.dwSize = 1;
	CurInfo.bVisible = FALSE;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CurInfo);
}

void setCoordinate(int x, int y) { //cmd���� Ŀ�� ��ġ�� x, y�� ����   Setting curser to (x,y).
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
					printf("     ��      ");
					setCoordinate(MAP_ADJ_X + (j * 6), MAP_ADJ_Y + (i * 3) + 1);
					printf("             ");
					setCoordinate(MAP_ADJ_X + (j * 6), MAP_ADJ_Y + (i * 3) + 2);
					printf("-------------");
				}
				else {
					setCoordinate(MAP_ADJ_X + (j * 6), MAP_ADJ_Y + (i * 3));
					printf("             ");
					setCoordinate(MAP_ADJ_X + (j * 6), MAP_ADJ_Y + (i * 3) + 1);
					printf("     ��      ");
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
					printf("���������������������� ");
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
					printf("���������������������� ");
					setCoordinate(MAP_ADJ_X + (j * 6), MAP_ADJ_Y + (i * 3) + 2);

				}
				else {
					setCoordinate(MAP_ADJ_X + (j * 6), MAP_ADJ_Y + (i * 3));
					printf("����������������������  ");
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
					printf("����������������������  ");
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
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 4); printf("  ����������������������    ");
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 5); printf("     %4d    ", block.nextnum);
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 6); printf("  ����������������������    ");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 7); printf("  ����������������������    ");
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 8); printf("     %4c    ", block2.nextnum);
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 9); printf("  ����������������������    ");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 10); printf("+-- -  -  - --+ ");
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 11); printf(" YOUR SCORE :");
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 12); printf("        %6d", score);
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 13); printf(" BEST SCORE :");
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 14); printf("        %6d", best_score);

	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 15); printf("  ��   : Flip");
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 16); printf("��  �� : Left / Right");
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 17); printf("  ��   : Soft Drop ");
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 18); printf(" SPACE : Hard Drop");
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 19); printf("   P   : Pause");
	setCoordinate(MAP_ADJ_X + 32, MAP_ADJ_Y + 20); printf("  ESC  : Quit");

}

void goMainMenuScreen() { //����ȭ�� ��� �Լ�. Printing out main menu.
	int x = 19;
	int y = 9;
	int input_command = 0;

	setCoordinate(x, y + 2); printf("��������������������������������������������������������������������");
	Sleep(30);
	setCoordinate(x, y + 2); printf("�� ��������������������������������������������������������");
	Sleep(30);
	setCoordinate(x, y + 2); printf("o �� ��������������������������������������������������");
	Sleep(30);
	setCoordinate(x, y + 2); printf("yo �� ������������������������������������������������");
	Sleep(30);
	setCoordinate(x, y + 2); printf("yoPuyo �� ������������������������������������������");
	Sleep(30);
	setCoordinate(x, y + 2); printf("PuyoPuyo �� ����������������������������������������");
	Sleep(30);
	setCoordinate(x, y + 2); printf("�� 64 PuyoPuyo �٦���������������������������������");
	Sleep(30);
	setCoordinate(x, y + 2); printf("������ 64 PuyoPuyo �� ������������������������������");
	Sleep(30);
	setCoordinate(x, y + 2); printf("�������� 64 PuyoPuyo �� ����������������������������");
	Sleep(30);
	setCoordinate(x, y + 2); printf("����������  64 PuyoPuyo �� ������������������������");
	Sleep(30);
	setCoordinate(x, y + 2); printf("��������������  64 PuyoPuyo �� ��������������������");
	Sleep(30);
	setCoordinate(x, y + 2); printf("����������������  64 PuyoPuyo �� ������������������");
	Sleep(30);
	setCoordinate(x, y + 2); printf("������������������  64 PuyoPuyo �� ����������������");
	Sleep(30);
	setCoordinate(x, y + 2); printf("��������������������  64 PuyoPuyo �� ��������������");
	Sleep(30);
	setCoordinate(x, y + 2); printf("����������������������  64 PuyoPuyo �� ������������");
	Sleep(30);
	setCoordinate(x, y + 2); printf("������������������������  64 PuyoPuyo �� ����������");
	Sleep(30);
	setCoordinate(x, y + 2); printf("��������������������������  64 PuyoPuyo �� ��������");
	Sleep(30);
	setCoordinate(x, y + 2); printf("����������������������������  64 PuyoPuyo �� ������");
	Sleep(30);
	setCoordinate(x, y + 2); printf("������������������������������  64 PuyoPuyo �� ����");
	Sleep(30);
	setCoordinate(x, y + 2); printf("��������������������������������  64 PuyoPuyo �� ��");


	setCoordinate(x + 2, y + 4); printf(" Press any key to start");

	setCoordinate(x, y + 6);  printf(" �ǢǢǢ� HOW TO CONTROL �ǢǢǢ�");
	setCoordinate(x, y + 7);  printf(" ��                            ��");
	setCoordinate(x, y + 8);  printf(" ��      ��   : Flip           ��");
	setCoordinate(x, y + 9); printf(" ��    ��  �� : Left / Right   ��");
	setCoordinate(x, y + 10); printf(" ��      ��   : Soft Drop      ��");
	setCoordinate(x, y + 11); printf(" ��    SPACE  : Hard Drop      ��");
	setCoordinate(x, y + 12); printf(" ��      P    : Pause          ��");
	setCoordinate(x, y + 13); printf(" ��     ESC   : Quit           ��");
	setCoordinate(x, y + 14); printf(" ��                            ��");
	setCoordinate(x, y + 15); printf(" �ǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢ�");

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

void printPauseScreen() { // ���� �Ͻ�����		Pause game.
	int x = MAP_ADJ_X + 8;
	int y = MAP_ADJ_Y + 5;

	setCoordinate(x, y + 0); printf("�ǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢ�");
	setCoordinate(x, y + 1); printf("��                              ��");
	setCoordinate(x, y + 2); printf("��  +-----------------------+   ��");
	setCoordinate(x, y + 3); printf("��  |       P A U S E       |   ��");
	setCoordinate(x, y + 4); printf("��  +-----------------------+   ��");
	setCoordinate(x, y + 5); printf("��   Press any key to resume.   ��");
	setCoordinate(x, y + 6); printf("��                              ��");
	setCoordinate(x, y + 7); printf("�ǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢ�");

	_getch();

	system("cls");
	printGameScreen();
}



void printEndScreen(int endType) {
	int input_command = 0;
	int x = MAP_ADJ_X + 8;

	switch (endType) {
	case 0:
		setCoordinate(x, MAP_ADJ_Y + 5);  printf("�ǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢ�");
		setCoordinate(x, MAP_ADJ_Y + 6);  printf("��                              ��");
		setCoordinate(x, MAP_ADJ_Y + 7);  printf("��  +-----------------------+   ��");
		setCoordinate(x, MAP_ADJ_Y + 8);  printf("��  |  G A M E  O V E R..   |   ��");
		setCoordinate(x, MAP_ADJ_Y + 9);  printf("��  +-----------------------+   ��");
		setCoordinate(x, MAP_ADJ_Y + 10); printf("��   YOUR SCORE: %6d         ��", score);
		setCoordinate(x, MAP_ADJ_Y + 11); printf("��                              ��");
		setCoordinate(x, MAP_ADJ_Y + 12); printf("��   Press any key to restart.  ��");
		setCoordinate(x, MAP_ADJ_Y + 13); printf("��                              ��");
		setCoordinate(x, MAP_ADJ_Y + 14); printf("�ǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢ�");
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
		setCoordinate(x, MAP_ADJ_Y + 5);  printf("�ǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢ�");
		setCoordinate(x, MAP_ADJ_Y + 6);  printf("��                              ��");
		setCoordinate(x, MAP_ADJ_Y + 7);  printf("��  +-----------------------+   ��");
		setCoordinate(x, MAP_ADJ_Y + 8);  printf("��  |      STAGE CLEAR      |   ��");
		setCoordinate(x, MAP_ADJ_Y + 9);  printf("��  +-----------------------+   ��");
		setCoordinate(x, MAP_ADJ_Y + 10); printf("��   YOUR SCORE: %6d         ��", score);
		setCoordinate(x, MAP_ADJ_Y + 11); printf("��                              ��");
		setCoordinate(x, MAP_ADJ_Y + 12); printf("�� Press any key to next stage. ��");
		setCoordinate(x, MAP_ADJ_Y + 13); printf("��                              ��");
		setCoordinate(x, MAP_ADJ_Y + 14); printf("�ǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢ�");

		while (_kbhit()) _getch();
		input_command = _getch();

		setGameFlow(1); /* To next stage */
		break;

	case 2:
		setCoordinate(x, MAP_ADJ_Y + 5);  printf("�ǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢ�");
		setCoordinate(x, MAP_ADJ_Y + 6);  printf("��                                      ��");
		setCoordinate(x, MAP_ADJ_Y + 7);  printf("��  +-------------------------------+   ��");
		setCoordinate(x, MAP_ADJ_Y + 8);  printf("��  | WINNER WINNER CHICKEN DINNER! |   ��");
		setCoordinate(x, MAP_ADJ_Y + 9);  printf("��  +-------------------------------+   ��");
		setCoordinate(x, MAP_ADJ_Y + 10); printf("��         YOUR SCORE: %6d           ��", score);
		setCoordinate(x, MAP_ADJ_Y + 11); printf("��                                      ��");
		setCoordinate(x, MAP_ADJ_Y + 12); printf("��         Cleared final stage.         ��");
		setCoordinate(x, MAP_ADJ_Y + 13); printf("��                                      ��");
		setCoordinate(x, MAP_ADJ_Y + 14); printf("�ǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢ�");

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
/* @ '����DO NOT MODIFY.����'ends. */