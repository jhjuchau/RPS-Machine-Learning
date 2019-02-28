//Written by James Juchau
//Between Sunday, August 30th and


#include <iostream>		//mainly for cout
#include <conio.h>		//for _getch()
#include <Windows.h>	//mainly for system("CLS"), among other functions
#include <iomanip>		//for setprecision() on the results screen
#include <string>		//for display() to work with coutPastMoves() more easily
#include "MenuInterface.h"
#include "GameLogic.h"



using namespace std;

//These are the numerical values returned when the labelled key is pressed while _getch() is active.
//For example, if c=_getch(), and the next keystroke entered is the down arrow key, c is assigned the value of 80.
//All these keys are used within normal program function.



int main()
{

	string playerNames[2] = { "Player 1", "Player 2" };
	bool CPU = false;
	int lives = mMenu(playerNames, CPU);	//[0] is player 1's name, [1] is player 2's name, mMenu returns # of lives
	if (lives == 0){ return 0; }
	double p1wins = 0, p2wins = 0;		//for keeping score on the results screen
	cout << "Loading..." << endl;
	system("CLS");
	GameLogic game;
	game.gameLoop(p1wins, p2wins, lives, playerNames, CPU);
	main();

	return 0;
}
