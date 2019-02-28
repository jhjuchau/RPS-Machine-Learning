#include <iostream>		//mainly for cout
#include <conio.h>		//for _getch()
#include <Windows.h>	//mainly for system("CLS"), among other functions
#include <iomanip>		//for setprecision() on the results screen
#include <string>		//for display() to work with coutPastMoves() more easily
#include "MenuInterface.h"
#include "RPSCPU.h"



using namespace std;

//These are the numerical values returned when the labelled key is pressed while _getch() is active.
//For example, if c=_getch(), and the next keystroke entered is the down arrow key, c is assigned the value of 80.
//All these keys are used within normal program function.

#define UPKEY 72
#define DOWNKEY 80
#define LEFTKEY 75
#define RIGHTKEY 77
#define ENTER 13
#define SPACE 32
#define ESC 27

#define AKEY 97
#define SKEY 115
#define DKEY 100
#define JKEY 106
#define KKEY 107
#define LKEY 108

class GameLogic{
public:
	void gameLoop(double& p1wins, double& p2wins, int lives, string playerNames[2], bool CPU)
	{
		RPSCPU computer;
		boolean ongoing = true;	//boolean sentinel to detect when the game is over
		int p1move = 0, p2move = 0;			  //1 - rock; 2 - paper; 3 - scissors
		int p1life = lives, p2life = lives;     //keeps track of the life the players have
		int p1PastMoves[2] = { 0, 0 }, p2PastMoves[2] = { 0, 0 };	//two small arrays that track each players' previous two moves
		if (CPU){ playerNames[1] = "THE MACHINE"; }


		//also used in display() to show each players previous move
		//initialized to {0,0} to simplify how display() reads these arrays

		while (ongoing) //the following function calls loop while the game hasn't ended
		{
			display(p1life, p2life, p1PastMoves, p2PastMoves, lives, playerNames);	//displays controls and current stats
			collectInput(p1move, p2move, p1PastMoves, p2PastMoves, CPU); //loops until both players have entered valid input
			ongoing = analyzePlay(p1move, p2move, p1life, p2life);	//calculates the results of each play and sets 'ongoing' to false if the game is over, ending the loop

		}//close while(ongoing) loop
		if (p1life == 0){ p2wins++; }	//only one point of damage is possible per turn in fireball. therefore, it is impossible that both players reach 0 life simultaneously. therefore, the order of placement of these
		//win conditions is irrelevant, as the proper player will win in normal play.
		else { p1wins++; }
		resultsAndRetry(p1wins, p2wins, lives, playerNames, CPU);
	}
private:
	void display(int p1life, int p2life, int p1PastMoves[], int p2PastMoves[], int lives, string playerNames[])
	{
		cout << playerNames[0] << ":\t\t\t\t\t" << playerNames[1] << ":" << endl;
		cout << "A=Rock\t\t\t\t\t\tJ=Rock" << endl;
		cout << "S=Paper\t\t\t\t\t\tK=Paper" << endl;
		cout << "D=Scissors\t\t\t\t\tL=Scissors" << endl << endl;
		cout << "Life: " << p1life << "/" << lives;
		cout << "\t\t\t\t\tLife: " << p2life << "/" << lives << endl;
		/*
		if (p1PastMoves[0] != 0)	//this if statement is here to prevent the game from reading this message on the first turn
		//on every following turn, p1PastMoves[0] will always contain a nonzero value
		{
		cout << "On the previous turn..." << endl;
		cout << "Player 1 ";
		cout << coutPastMove(p1PastMoves[0]) << endl;	//p1PastMoves[0] is Player 1's previous move
		//coutPastMove() translates the integer value of the move into an in-game message for the players
		cout << "Player 2 ";
		cout << coutPastMove(p2PastMoves[0]) << endl;
		}
		*/
		cout << "TAP A KEY TWICE TO LOCK IN YOUR SELECTION!" << endl;
	}

	string coutPastMove(int in)
	{
		switch (in)
		{
		case 1:
			return "threw rock!";
			break;
		case 2:
			return "tossed paper!";
			break;
		case 3:
			return "drew scissors!";
			break;
		default:
			return "did nothing! (this should never happen)";
			break;
		}

	}

	void collectInput(int& p1move, int& p2move, int p1PastMoves[], int p2PastMoves[], bool CPU)
	{
		p1move = 0; p2move = 0; //wiping move variables clean for a new turn
		RPSCPU computerPlayer;


		while (p1move <= 0 || p2move <= 0) //this while loop will continue until both players have entered valid input, as explained below
		{
			//The way I programmed this section of the game is a little confusing if you just read it, so I'll break it down.
			//p1move and p2move are integers.	1 corresponds to the move "rock"
			//									2 corresponds to the move "paper"
			//									3 corresponds to the move "scissors"
			//I wanted to have some kind of confirmation system to prevent players from accidentally selecting the wrong choice. My initial thought was to have both players input a move, have a timer tick down from three,
			//and if either player hit the spacebar during this countdown, the moves would be reset and input would be collected again. Unfortunately, I couldn't find a way to collect input while a timer was ticking down,
			//as the only real-time input collection methods I'm familiar with (cin, getline(),  and _getch()) all stop the program in progress and wait specifically for input.

			//Therefore, to help ensure players get the input they intend, I implemented a double-tapping confirmation method. You have to press the same key twice in a row to lock in your action.
			//To detect if the previous player input was the same key, I made the first tap assign the negative value of the option they're considering.
			//									-1 corresponds to 'considering' rock
			//									-2 corresponds to 'considering' paper
			//									-3 corresponds to 'considering' scissors

			//Therefore, if player one hits 'S' for the first time, the p1move value will change to -2, considering block.
			//If player one hits 'D" after that, the p1move value will change to -3, considering fireball.
			//If player one hits 'D' again after that, the p1move value will change to 3, and the player will be locked into performing fireball.

			//I chose to use negatives to simplify verification of locking in a move. I can simply check if(p1move>0) to see if they've locked in a move, and prevent further change.
			//It also help with the loop that governs this whole process: while (p1move<= 0 || p2move<= 0)
			//		because the default value (0), and the 'considering' values are negative, the loop will continue while both players are either considering or have not made any decision.
			//		The loop will only end if both players have double-tapped an option.

			//Currently, even if a player has double-tapped an option, they have the option of switching. I'm trying to decide whether that functionality is conducive to competition or unnecessary and limiting.
			//Once both players have double-tapped an option, however, the loop ends and both players' moves will be passed into analyzePlay()


			int c; // integer input variable

			switch (c = _getch())		  //a switch block pivoting on user input
			{
			case AKEY:			//If the A key is pressed, meaning Player 1 selected 'Rock'
				if (p1move == -1) //if p1move was already -1, which could only happen if the immediately prior p1 input was also 'A'...
				{
					cout << "Player 1 move selected!" << endl; //confirm to the players that player 1 has double-tapped their input
					p1move = 1;	//assign p1move the value of positive 1. the input collection loop only breaks when p1move and p2move are both positive, meaning both players have double-tapped to confirm their move.
				}
				else //if p1move was anything other than -1, meaning the user 
					//NOTE: since, in this build, you CAN switch moves after double tapping, this case could still occur if this is the third, fifth, seventh (etc etc etc) input of the same key.
					//if a player double taps a selection, then presses the same key again, their 'confirmation' is undone and they have to press the key one more time to confirm again
					//I could, and probably will, fix this so that subsequent presses of the same key after confirmation don't change the value of p1move, but it's not high on my list of priorities 
				{
					cout << "Player 1 is thinking...!" << endl;


					p1move = -1; // p1move is assigned -1. if the next player one input (A, S or D) is 'A', p1move will be assigned 1 (charge)
				}
				break;


			case SKEY:	//Player 1 selects 'paper'
				if (p1move == -2)
				{
					cout << "Player 1 move selected!" << endl;
					p1move = 2;
				}
				else
				{
					cout << "Player 1 is thinking...!" << endl;
					p1move = -2;
				}
				break;

			case DKEY:		//player 1 selects 'scissors'
				if (p1move == -3) //if player 2 double-taps 'scissors' 
				{
					cout << "Player 1 move selected!" << endl;
					p1move = 3;
				}
				else
				{
					cout << "Player 1 is thinking...!" << endl;
					p1move = -3;
				}
				break;

				if (CPU == false){
					//player 2 options
			case JKEY:
				if (p2move == -1)	//Player 2 selects 'Rock'
				{
					cout << "\t\t\t\t\t\tPlayer 2 move selected!" << endl;
					p2move = 1;
				}
				else
				{
					cout << "\t\t\t\t\t\tPlayer 2 is thinking...!" << endl;
					p2move = -1;
				}

				break;

			case KKEY:	//if Player 2 selects 'Paper'...
				if (p2move == -2)
				{
					cout << "\t\t\t\t\t\tPlayer 2 move selected!" << endl;
					p2move = 2;
				}
				else
				{
					cout << "\t\t\t\t\t\tPlayer 2 is thinking...!" << endl;
					p2move = -2;
				}

				break;

			case LKEY:		//player 2 selects 'Scissors'
				if (p2move == -3)
				{
					cout << "\t\t\t\t\t\tPlayer 2 move selected!" << endl;
					p2move = 3;
				}
				else
				{
					cout << "\t\t\t\t\t\tPlayer 2 is thinking...!" << endl;
					p2move = -3;
				}
				break;

				}


			}//close switch

			p2move = computerPlayer.randomInput();
		}//close while(p1move == 0 && p2move == 0) loop
		push(p1move, p1PastMoves);
		push(p2move, p2PastMoves);
		system("CLS");
	}

	boolean analyzePlay(int p1move, int p2move, int& p1life, int& p2life)
	{
		//									1 corresponds to the move "Rock"
		//									2 corresponds to the move "Paper"
		//									3 corresponds to the move "Scissors"

		boolean p1safe = false, p2safe = false;	//this will simplify damage calculation later down the line

		if (p1move == p2move)
		{
			cout << "Both players threw ";
			if (p1move == 1){ cout << "rock!" << endl; }
			if (p1move == 2){ cout << "paper!" << endl; }
			if (p1move == 3){ cout << "scissors!" << endl; }
			cout << "Both players are safe!" << endl;
		}

		//end of tie logic


		if (p1move == 1 && p2move == 2) //P1 rock vs P2 paper
		{
			cout << "Player 1's ROCK is deftly covered by Player 2's PAPER!" << endl;
			p1life--;
		}

		if (p1move == 2 && p2move == 1){
			cout << "Player 2's ROCK is deftly covered by Player 1's PAPER!" << endl;
			p2life--;
		}

		//end of rock vs paper logic


		if (p1move == 2 && p2move == 3) //P1 rock vs P2 paper
		{
			cout << "Player 2's SCISSORS slice straight through Player 1's PAPER!" << endl;
			p1life--;
		}

		if (p1move == 3 && p2move == 2){
			cout << "Player 1's SCISSORS slice straight through Player 2's PAPER!" << endl;
			p2life--;
		}

		//end of paper vs scissors logic

		if (p1move == 3 && p2move == 1) //P1 rock vs P2 paper
		{
			cout << "Player 2's ROCK smashes Player 1's SCISSORS!" << endl;
			p1life--;
		}

		if (p1move == 1 && p2move == 3){
			cout << "Player 1's ROCK smashes Player 2's SCISSORS!" << endl;
			p2life--;
		}

		//end of rock vs scissors logic



		system("pause");
		cout << endl;

		if (p1life <= 0){ cout << "Player 2 wins!" << endl; return false; }
		if (p2life <= 0){ cout << "Player 1 wins!" << endl; return false; }
		return true;
	}

	void resultsAndRetry(double p1wins, double p2wins, int lives, string playerNames[2], bool CPU)
	{
		int total = p1wins + p2wins;	//there are no tie games in fireball, so adding how many each player has won gives you the total number of games played
		cout << "Player 1: " << p1wins << "/" << total;
		cout << "  (" << ((p1wins / total) * 100.00) << "%)" << endl;
		cout << "Player 2: " << p2wins << "/" << total;
		cout << "  (" << ((p2wins / total) * 100.00) << "%)" << endl;

		cout << "Press ESC to quit, SPACEBAR to return to the main menu, or another key to retry." << endl;
		int c = _getch();
		if (c == ESC){ return; }
		else if (c == SPACE){ return; }
		else { gameLoop(p1wins, p2wins, lives, playerNames, CPU); }
	}

	void push(int currentMove, int pastMoves[2])
	{
		//This function maintains the pastMoves queue. This queue is used to prevent players from blocking three times in a row
		//pastMoves[] also is used in the display() function to inform both players of their last two moves
		if (pastMoves[0] == 0){ pastMoves[0] = currentMove; return; }
		else
		{
			pastMoves[1] = pastMoves[0];
			pastMoves[0] = currentMove;
		}
	}

};