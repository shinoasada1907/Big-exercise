#pragma GCC optimize("Ofast")//used to optimize the program
#pragma comment(lib, "lwinmm.lib")//libraby link
#include <iostream>
#include <fstream>//the library write information to file .txt
#include <chrono>//the time libraby
#include <conio.h>
#include <windows.h>
using namespace std;

#define PAINT1 char(219)
#define PAINT2 PAINT1 << PAINT1
#define PAINT3 PAINT2 << PAINT1
#define PAINT4 PAINT3 << PAINT1
#define PAINT5 PAINT4 << PAINT1
#define KB_ENTER 13
#define KB_ESC 27
#define KB_UP 72
#define KB_LEFT 75
#define KB_RIGHT 77
#define KB_DOWN 80
#define X (j - 1) * 20 + 1
#define Y (i - 1) * 10
#define endl '\n'

// function to create color of font
void colorFont(int n){
	HANDLE color;
	color = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(color, n);
}

// function to resize console
void resizeConsole(int width, int height){
	HWND console = GetConsoleWindow();
	RECT r;
	GetWindowRect(console, &r);
	MoveWindow(console, r.left, r.top, width, height, TRUE);
}

// function teleport to one position to write in console
void gotoxy(short x, short y){
	HANDLE color;
	color = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD cal = {x, y};
	SetConsoleCursorPosition(color, cal);
}

//	function write new high score
void writeDataToFile(string file, long long score){
	ofstream coutFile(file);
	if(coutFile.fail()){
		colorFont(12);	cout << "Warning: Can't find file HighScore.txt. Your high score won't be save !";
		colorFont(15);	Sleep(2000);		return;
	}
	coutFile << score;
}

//	function receive high score to game
void readDataFromFile(string file, long long &fileScore){
	ifstream cinFile(file);
	if(cinFile.fail()){
		colorFont(12);	cout << "Warning: Can't find file HighScore.txt. Your high score won't be save !";
		colorFont(15);	Sleep(2000);		return;
	}
	cinFile >> fileScore;
}

//	oop design
class GAME{
	private:
		string filePath = "HighScore.txt";
		long long score;
		int n, _2048[100][100], turn, add, accept, choice, gameOver, select;
	public:
		long long highScore = -1;
		GAME(){
			n = 4;	turn = 1;	score = 0;	gameOver = 0;	select = 1;
			readDataFromFile(filePath, highScore);
		}
		void menu();
		void main();
		void guide();
		void makeBoard();
		void generate();
		int random();
		void display();
		void writeScore();
		void move();
		void checkGameOver();
		void endGame();
};

void GAME::menu(){
	MAIN:;
	system("cls");
	colorFont(10);
	gotoxy(98, 15);		cout << "  ______    _______       __     _______     ";
	gotoxy(98, 16);		cout << " /  __  \\  /  ___  \\     /  |   /  ___  \\ ";
	gotoxy(98, 17);		cout << " |_|  \\  | | /   \\ |    /   |   | /   \\ | ";
	gotoxy(98, 18);		cout << "      |  | | |   | |   / /| |   | \\___/ |   ";
	gotoxy(98, 19);		cout << "     /  /  | |   | |  / /_| |_   \\     /    ";
	gotoxy(98, 20);		cout << "    /  /   | |   | | |____   _|  / ___ \\    ";
	gotoxy(98, 21);		cout << "   /  /    | |   | |      | |   | /   \\ |   ";
	gotoxy(98, 22);		cout << "  /  /___  | \\___/ |      | |   | \\___/ |  ";
	gotoxy(98, 23);		cout << " |_______| \\_______/      |_|   \\_______/  ";
	gotoxy(99, 25);		cout << "             _*-*_MENU_*-*_                  ";
	gotoxy(99, 27);		cout << "               Play Game                     ";
	gotoxy(99, 29);		cout << "               Quit                          ";
	colorFont(15);
	MENU:;
	switch(select){
		case 1:
			gotoxy(111, 27);	colorFont(14);		cout << ">> Play Game";		colorFont(10);		gotoxy(0, 0);
			switch(_getch()){
				case KB_ENTER:	colorFont(14);
					for (auto i = 1; i <= 13; i ++){
						gotoxy(106 + i, 27);	cout << "     >>";
						gotoxy(0, 0);		Sleep(50);
					}
					gotoxy(114, 27);	colorFont(10);		cout << "Play Game     ";
					colorFont(10);		gameOver = 0;		PlaySound(NULL , NULL, SND_ASYNC);		main();			goto MAIN;
				case KB_DOWN :		select = 2;		break;
				default:		goto MENU;
			}
			gotoxy(109, 27);		cout << "     Play Game     ";		goto MENU;
		case 2:
			gotoxy(111, 29);	colorFont(14);		cout << ">> Quit";			colorFont(10);		gotoxy(0, 0);
			switch(_getch()){
				case KB_ENTER:	colorFont(14);
					for (auto i = 1; i <= 13; i ++){
						gotoxy(106 + i, 29);	cout << "     >>";
						gotoxy(0, 0);		Sleep(50);
					}
					gotoxy(111, 29);		cout << ">> Quit          ";		colorFont(10);		gotoxy(0, 0);	goto END;
				case KB_UP:			select = 1;		break;
				default:		goto MENU;
			}
			gotoxy(109, 29);		cout << "     Quit     ";			goto MENU;
	}
	END:;
}

//	function main game
void GAME::main(){
	makeBoard();
//	_2048[1][1] = 2;	_2048[1][2] = 4;	_2048[1][3] = 8;	_2048[1][4] = 16;	_2048[2][1] = 32;	_2048[2][2] = 64;	_2048[2][3] = 128;	_2048[2][4] = 256;	_2048[3][1] = 512;	_2048[3][2] = 1024;	_2048[3][3] = 2048;	_2048[3][4] = 4096;	_2048[4][1] = 8192;
	generate();
	guide();
	TURN:;
		generate();
		display();
		writeScore();
		checkGameOver();
		if(gameOver){	endGame();	goto END;	}
		REINPUT:;
			accept = 0;
			switch(_getch()){
				case KB_UP:		choice = 1;		move();
					if(accept)	goto TURN;
					else		goto REINPUT;
				case KB_RIGHT:	choice = 2;		move();
					if(accept)	goto TURN;
					else		goto REINPUT;
				case KB_DOWN:	choice = 3;		move();
					if(accept)	goto TURN;
					else		goto REINPUT;
				case KB_LEFT:	choice = 4;		move();
					if(accept)	goto TURN;
					else		goto REINPUT;
				case KB_ESC:	score = 0;		goto END;
				default:		goto REINPUT;
			}
	END:;
	PlaySound(NULL , NULL, SND_ASYNC);
}

void GAME::guide(){
	gotoxy(83, 1);		cout << "ESC: Exit Game";
	gotoxy(0, 0);
}

//	function make board
void GAME::makeBoard(){
	colorFont(8);		gotoxy(0, 0);
	for(auto i = 1; i <= n; i ++)
		for(auto j = 1; j <= n; j ++)
			_2048[i][j] = 0;
	for(auto i = 1; i <= n * 10 + 1; i ++){
		for(auto j = 1; j <= n * 20 + 1; j ++)
			if(!((i - 1) % 10) || ((i - 1) % 10 && !((j - 1) % 20)))
				cout << PAINT1;
			else cout << ' ';
		cout << endl;
	}
}

//	function generate point random on map
void GAME::generate(){
	int seed, i ,j;
	!(turn % 5) ? add = 4 : add = 2;
	turn ++;
	while(true){
		seed = random();
		i = seed / n + 1;	j = seed % n + 1;
		if(!(_2048[i][j])){		_2048[i][j] = add;		break;		}
	}
}

//	function return random seed
int GAME::random(){
	return rand() % (n * n);
}

//	function write point in board
void GAME::display(){
	for(auto i = 1; i <= n; i ++)
		for(auto j = 1; j <= n; j ++)
			switch(_2048[i][j]){
				case 0:
					gotoxy(X, Y + 1);		cout << "                   ";
					gotoxy(X, Y + 2);		cout << "                   ";
					gotoxy(X, Y + 3);		cout << "                   ";
					gotoxy(X, Y + 4);		cout << "                   ";
					gotoxy(X, Y + 5);		cout << "                   ";
					gotoxy(X, Y + 6);		cout << "                   ";
					gotoxy(X, Y + 7);		cout << "                   ";
					gotoxy(X, Y + 8);		cout << "                   ";
					gotoxy(X, Y + 9);		cout << "                   ";
					break;
				case 2:
					colorFont(240);
					gotoxy(X, Y + 1);		cout << "                   ";
					gotoxy(X, Y + 2);		cout << "                   ";
					gotoxy(X, Y + 3);		cout << "       " << PAINT5 << "       ";
					gotoxy(X, Y + 4);		cout << "           " << PAINT1 << "       ";
					gotoxy(X, Y + 5);		cout << "       " << PAINT5 << "       ";
					gotoxy(X, Y + 6);		cout << "       " << PAINT1 << "           ";
					gotoxy(X, Y + 7);		cout << "       " << PAINT5 << "       ";
					gotoxy(X, Y + 8);		cout << "                   ";
					gotoxy(X, Y + 9);		cout << "                   ";
					colorFont(15);			break;
				case 4:
					colorFont(112);
					gotoxy(X, Y + 1);		cout << "                   ";
					gotoxy(X, Y + 2);		cout << "                   ";
					gotoxy(X, Y + 3);		cout << "       " << PAINT1 << "   " << PAINT1 << "       ";
					gotoxy(X, Y + 4);		cout << "       " << PAINT1 << "   " << PAINT1 << "       ";
					gotoxy(X, Y + 5);		cout << "       " << PAINT5 << "       ";
					gotoxy(X, Y + 6);		cout << "           " << PAINT1 << "       ";
					gotoxy(X, Y + 7);		cout << "           " << PAINT1 << "       ";
					gotoxy(X, Y + 8);		cout << "                   ";
					gotoxy(X, Y + 9);		cout << "                   ";
					colorFont(15);			break;
				case 8:
					colorFont(48);
					gotoxy(X, Y + 1);		cout << "                   ";
					gotoxy(X, Y + 2);		cout << "                   ";
					gotoxy(X, Y + 3);		cout << "       " << PAINT5 << "       ";
					gotoxy(X, Y + 4);		cout << "       " << PAINT1 << "   " << PAINT1 << "       ";
					gotoxy(X, Y + 5);		cout << "       " << PAINT5 << "       ";
					gotoxy(X, Y + 6);		cout << "       " << PAINT1 << "   " << PAINT1 << "       ";
					gotoxy(X, Y + 7);		cout << "       " << PAINT5 << "       ";
					gotoxy(X, Y + 8);		cout << "                   ";
					gotoxy(X, Y + 9);		cout << "                   ";
					colorFont(15);			break;
				case 16:
					colorFont(176);
					gotoxy(X, Y + 1);		cout << "                   ";
					gotoxy(X, Y + 2);		cout << "                   ";
					gotoxy(X, Y + 3);		cout << "      " << PAINT1 << " " << PAINT5 << "      ";
					gotoxy(X, Y + 4);		cout << "      " << PAINT1 << " " << PAINT1 << "          ";
					gotoxy(X, Y + 5);		cout << "      " << PAINT1 << " " << PAINT5 << "      ";
					gotoxy(X, Y + 6);		cout << "      " << PAINT1 << " " << PAINT1 << "   " << PAINT1 << "      ";
					gotoxy(X, Y + 7);		cout << "      " << PAINT1 << " " << PAINT5 << "      ";
					gotoxy(X, Y + 8);		cout << "                   ";
					gotoxy(X, Y + 9);		cout << "                   ";
					colorFont(15);			break;
				case 32:
					colorFont(32);
					gotoxy(X, Y + 1);		cout << "                   ";
					gotoxy(X, Y + 2);		cout << "                   ";
					gotoxy(X, Y + 3);		cout << "    " << PAINT5 << " " << PAINT5 << "    ";
					gotoxy(X, Y + 4);		cout << "        " << PAINT1 << "     " << PAINT1 << "    ";
					gotoxy(X, Y + 5);		cout << "    " << PAINT5 << " " << PAINT5 << "    ";
					gotoxy(X, Y + 6);		cout << "        " << PAINT1 << " " << PAINT1 << "        ";
					gotoxy(X, Y + 7);		cout << "    " << PAINT5 << " " << PAINT5 << "    ";
					gotoxy(X, Y + 8);		cout << "                   ";
					gotoxy(X, Y + 9);		cout << "                   ";
					colorFont(15);			break;
				case 64:
					colorFont(160);
					gotoxy(X, Y + 1);		cout << "                   ";
					gotoxy(X, Y + 2);		cout << "                   ";
					gotoxy(X, Y + 3);		cout << "    " << PAINT5 << " " << PAINT1 << "   " << PAINT1 << "    ";
					gotoxy(X, Y + 4);		cout << "    " << PAINT1 << "     " << PAINT1 << "   " << PAINT1 << "    ";
					gotoxy(X, Y + 5);		cout << "    " << PAINT5 << " " << PAINT5 << "    ";
					gotoxy(X, Y + 6);		cout << "    " << PAINT1 << "   " << PAINT1 << "     " << PAINT1 << "    ";
					gotoxy(X, Y + 7);		cout << "    " << PAINT5 << "     " << PAINT1 << "    ";
					gotoxy(X, Y + 8);		cout << "                   ";
					gotoxy(X, Y + 9);		cout << "                   ";
					colorFont(15);			break;
				case 128:
					colorFont(16);
					gotoxy(X, Y + 1);		cout << "                   ";
					gotoxy(X, Y + 2);		cout << "                   ";
					gotoxy(X, Y + 3);		cout << "   " << PAINT1 << " " << PAINT5 << " " << PAINT5 << "   ";
					gotoxy(X, Y + 4);		cout << "   " << PAINT1 << "     " << PAINT1 << " " << PAINT1 << "   " << PAINT1 << "   ";
					gotoxy(X, Y + 5);		cout << "   " << PAINT1 << " " << PAINT5 << " " << PAINT5 << "   ";
					gotoxy(X, Y + 6);		cout << "   " << PAINT1 << " " << PAINT1 << "     " << PAINT1 << "   " << PAINT1 << "   ";
					gotoxy(X, Y + 7);		cout << "   " << PAINT1 << " " << PAINT5 << " " << PAINT5 << "   ";
					gotoxy(X, Y + 8);		cout << "                   ";
					gotoxy(X, Y + 9);		cout << "                   ";
					colorFont(15);			break;
				case 256:
					colorFont(144);
					gotoxy(X, Y + 1);		cout << "                   ";
					gotoxy(X, Y + 2);		cout << "                   ";
					gotoxy(X, Y + 3);		cout << " " << PAINT5 << " " << PAINT5 << " " << PAINT5 << " ";
					gotoxy(X, Y + 4);		cout << "     " << PAINT1 << " " << PAINT1 << "     " << PAINT1 << "     ";
					gotoxy(X, Y + 5);		cout << " " << PAINT5 << " " << PAINT5 << " " << PAINT5 << " ";
					gotoxy(X, Y + 6);		cout << " " << PAINT1 << "         " << PAINT1 << " " << PAINT1 << "   " << PAINT1 << " ";
					gotoxy(X, Y + 7);		cout << " " << PAINT5 << " " << PAINT5 << " " << PAINT5 << " ";
					gotoxy(X, Y + 8);		cout << "                   ";
					gotoxy(X, Y + 9);		cout << "                   ";
					colorFont(15);			break;
				case 512:
					colorFont(96);
					gotoxy(X, Y + 1);		cout << "                   ";
					gotoxy(X, Y + 2);		cout << "                   ";
					gotoxy(X, Y + 3);		cout << "   " << PAINT5 << " " << PAINT1 << " " << PAINT5 << "   ";
					gotoxy(X, Y + 4);		cout << "   " << PAINT1 << "     " << PAINT1 << "     " << PAINT1 << "   ";
					gotoxy(X, Y + 5);		cout << "   " << PAINT5 << " " << PAINT1 << " " << PAINT5 << "   ";
					gotoxy(X, Y + 6);		cout << "       " << PAINT1 << " " << PAINT1 << " " << PAINT1 << "       ";
					gotoxy(X, Y + 7);		cout << "   " << PAINT5 << " " << PAINT1 << " " << PAINT5 << "   ";
					gotoxy(X, Y + 8);		cout << "                   ";
					gotoxy(X, Y + 9);		cout << "                   ";
					colorFont(15);			break;
				case 1024:
					colorFont(224);
					gotoxy(X, Y + 1);		cout << "                   ";
					gotoxy(X, Y + 2);		cout << "                   ";
					gotoxy(X, Y + 3);		cout << PAINT1 << " " << PAINT5 << " " << PAINT5 << " " << PAINT1 << "   " << PAINT1;
					gotoxy(X, Y + 4);		cout << PAINT1 << " " << PAINT1 << "   " << PAINT1 << "     " << PAINT1 << " "  << PAINT1 << "   " << PAINT1;
					gotoxy(X, Y + 5);		cout << PAINT1 << " " << PAINT1 << "   " << PAINT1 << " " << PAINT5 << " " << PAINT5;
					gotoxy(X, Y + 6);		cout << PAINT1 << " " << PAINT1 << "   " << PAINT1 << " " << PAINT1 << "         " << PAINT1;
					gotoxy(X, Y + 7);		cout << PAINT1 << " " << PAINT5 << " " << PAINT5 << "     " << PAINT1;
					gotoxy(X, Y + 8);		cout << "                   ";
					gotoxy(X, Y + 9);		cout << "                   ";
					colorFont(15);			break;
				case 2048:
					colorFont(64);
					gotoxy(X, Y + 1);		cout << "                   ";
					gotoxy(X, Y + 2);		cout << "                   ";
					gotoxy(X, Y + 3);		cout << PAINT4 << " " << PAINT4 << " " << PAINT1 << "  " << PAINT1 << " "  << PAINT4;
					gotoxy(X, Y + 4);		cout << "   " << PAINT1 << " " << PAINT1 << "  " << PAINT1 << " " << PAINT1 << "  " << PAINT1 << " "  << PAINT1 << "  " << PAINT1;
					gotoxy(X, Y + 5);		cout << PAINT4 << " " << PAINT1 << "  " << PAINT1 << " " << PAINT4 << " "  << PAINT4;
					gotoxy(X, Y + 6);		cout << PAINT1 << "    " << PAINT1 << "  " << PAINT1 << "    " << PAINT1 << " "  << PAINT1 << "  " << PAINT1;
					gotoxy(X, Y + 7);		cout << PAINT4 << " " << PAINT4 << "    " << PAINT1 << " "  << PAINT4;
					gotoxy(X, Y + 8);		cout << "                   ";
					gotoxy(X, Y + 9);		cout << "                   ";
					colorFont(15);			break;
				case 4096:
					colorFont(192);
					gotoxy(X, Y + 1);		cout << "                   ";
					gotoxy(X, Y + 2);		cout << "                   ";
					gotoxy(X, Y + 3);		cout << PAINT1 << "  " << PAINT1 << " " << PAINT4 << " " << PAINT4 << " " << PAINT4;
					gotoxy(X, Y + 4);		cout << PAINT1 << "  " << PAINT1 << " " << PAINT1 << "  " << PAINT1 << " " << PAINT1 << "  " << PAINT1 << " "  << PAINT1 << "   ";
					gotoxy(X, Y + 5);		cout << PAINT4 << " " << PAINT1 << "  " << PAINT1 << " " << PAINT4 << " " << PAINT4;
					gotoxy(X, Y + 6);		cout << "   " << PAINT1 << " " << PAINT1 << "  " << PAINT1 << "    " << PAINT1 << " "  << PAINT1 << "  " << PAINT1;
					gotoxy(X, Y + 7);		cout << "   " << PAINT1 << " " << PAINT4 << " " << PAINT4 << " " << PAINT4;
					gotoxy(X, Y + 8);		cout << "                   ";
					gotoxy(X, Y + 9);		cout << "                   ";
					colorFont(15);			break;
				case 8192:
					colorFont(80);
					gotoxy(X, Y + 1);		cout << "                   ";
					gotoxy(X, Y + 2);		cout << "                   ";
					gotoxy(X, Y + 3);		cout << PAINT5 << " " << PAINT1 << " " << PAINT5 << " " << PAINT5;
					gotoxy(X, Y + 4);		cout << PAINT1 << "   " << PAINT1 << " " << PAINT1 << " " << PAINT1 << "   " << PAINT1 << "     "  << PAINT1;
					gotoxy(X, Y + 5);		cout << PAINT5 << " " << PAINT1 << " " << PAINT5 << " " << PAINT5;
					gotoxy(X, Y + 6);		cout << PAINT1 << "   " << PAINT1 << " " << PAINT1 << "     " << PAINT1 << " "  << PAINT1 << "    ";
					gotoxy(X, Y + 7);		cout << PAINT5 << " " << PAINT1 << " " << PAINT5 << " " << PAINT5;
					gotoxy(X, Y + 8);		cout << "                   ";
					gotoxy(X, Y + 9);		cout << "                   ";
					colorFont(15);			break;
				case 16384:		break;
				case 32768:		break;
				case 65536:		break;
				case 131072:	break;
				default:		return;
			}
}

//	function write score gain after 1 turn
void GAME::writeScore(){
	gotoxy(105, 5);		cout << "High Score: " << highScore;
	gotoxy(110, 7);		cout << "Score: " << score;
}

//	function do the input command
void GAME::move(){
	int location, checkSpace;
	switch(choice){
		case 1:		// Up
			for(auto j = 1; j <= n; j ++){
				checkSpace = 0;
				for(auto i = 1; i <= n; i ++){
					if(!checkSpace && _2048[i][j])		continue;
					if(!checkSpace && !_2048[i][j]){	checkSpace = 1;		continue;	}
					if(checkSpace && _2048[i][j]){		accept = 1;			break;		}
				}
				if(accept)		break;
			}
			for(auto j = 1; j <= n; j ++){
				location = 1;
				for(auto i = 1; i <= n; i ++){
					if(!_2048[i][j])	continue;
					for(auto k = i + 1; k <= n; k ++){
						if(!_2048[k][j])	continue;
						if(_2048[i][j] == _2048[k][j]){		_2048[i][j] *= 2;	_2048[k][j] = 0;	score += _2048[i][j];	accept = 1;	}
						break;
					}
					if(location != i){	_2048[location][j] = _2048[i][j];	_2048[i][j] = 0;	}
					location ++;
				}
			}
			break;
		case 2:		//	Right
			for(auto i = 1; i <= n; i ++){
				checkSpace = 0;
				for(auto j = n; j >= 1; j --){
					if(!checkSpace && _2048[i][j])		continue;
					if(!checkSpace && !_2048[i][j]){	checkSpace = 1;		continue;	}
					if(checkSpace && _2048[i][j]){		accept = 1;			break;		}
				}
				if(accept)		break;
			}
			for(auto i = 1; i <= n; i ++){
				location = n;
				for(auto j = n; j >= 1; j --){
					if(!_2048[i][j])	continue;
					for(auto k = j - 1; k >= 1; k --){
						if(!_2048[i][k])	continue;
						if(_2048[i][j] == _2048[i][k]){		_2048[i][j] *= 2;	_2048[i][k] = 0;	score += _2048[i][j];	accept = 1;	}
						break;
					}
					if(location != j){	_2048[i][location] = _2048[i][j];	_2048[i][j] = 0;	}
					location --;
				}
			}
			break;
		case 3:		//	Down
			for(auto j = 1; j <= n; j ++){
				checkSpace = 0;
				for(auto i = n; i >= 1; i --){
					if(!checkSpace && _2048[i][j])		continue;
					if(!checkSpace && !_2048[i][j]){	checkSpace = 1;		continue;	}
					if(checkSpace && _2048[i][j]){		accept = 1;			break;		}
				}
				if(accept)		break;
			}
			for(auto j = 1; j <= n; j ++){
				location = n;
				for(auto i = n; i >= 1; i --){
					if(!_2048[i][j])	continue;
					for(auto k = i - 1; k >= 1; k --){
						if(!_2048[k][j])	continue;
						if(_2048[i][j] == _2048[k][j]){		_2048[i][j] *= 2;	_2048[k][j] = 0;	score += _2048[i][j];	accept = 1;	}
						break;
					}
					if(location != i){	_2048[location][j] = _2048[i][j];	_2048[i][j] = 0;	}
					location --;
				}
			}
			break;
		case 4:		//	Left
			for(auto i = 1; i <= n; i ++){
				checkSpace = 0;
				for(auto j = 1; j <= n; j ++){
					if(!checkSpace && _2048[i][j])		continue;
					if(!checkSpace && !_2048[i][j]){	checkSpace = 1;		continue;	}
					if(checkSpace && _2048[i][j]){		accept = 1;			break;		}
				}
				if(accept)		break;
			}
			for(auto i = 1; i <= n; i ++){
				location = 1;
				for(auto j = 1; j <= n; j ++){
					if(!_2048[i][j])	continue;
					for(auto k = j + 1; k <= n; k ++){
						if(!_2048[i][k])	continue;
						if(_2048[i][j] == _2048[i][k]){		_2048[i][j] *= 2;	_2048[i][k] = 0;	score += _2048[i][j];	accept = 1;	}
						break;
					}
					if(location != j){	_2048[i][location] = _2048[i][j];	_2048[i][j] = 0;	}
					location ++;
				}
			}
			break;
		default:	break;
	}
}

//	function check game over condition
void GAME::checkGameOver(){
	auto checkFull = 1;
	for(auto i = 1; i <= n; i ++)
		for(auto j = 1; j <= n; j ++)
			if(!_2048[i][j])	checkFull = 0;
	if(checkFull){
		gameOver = 1;
		for(auto i = 1; i <= n; i ++)
			for(auto j = 1; j <= n; j ++)
				if((_2048[i][j] == _2048[i-1][j] && i != 1) || (_2048[i][j] == _2048[i+1][j] && i != n) || (_2048[i][j] == _2048[i][j-1] && j != 1) || (_2048[i][j] == _2048[i][j+1] && j != n))	gameOver = 0;
	}
}

//	function result and write new high score
void GAME::endGame(){
	if(score > highScore){
		gotoxy(97, 10);		cout << "Congratulation ! You achieve new high score !";
		writeDataToFile(filePath, score);
	}
	gotoxy(105, 12);	cout << "Game over. Your score is: " << score;
	gotoxy(100, 13);	cout << "Press any key to return to main menu...";
	score = 0;			_getch();
}

//	main function code start running
int main(int argc, char *argv[]){
	cin.tie(NULL);		cout.tie(NULL);	//	for fast input, ouput
	srand(time(0));						//	for random number in library <chrono>
	cout.setf(ios_base::boolalpha);		//	output boolean 'true' and 'false' instead of '1' and '0'
	resizeConsole(1280,720);			//	resize game screen to 720p
	GAME play;			play.menu();
	return 0;
}
