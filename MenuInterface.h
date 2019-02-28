#ifndef MENU_H
#define MENU_H

#include <iostream>
using namespace std;

#define UKEY 72
#define DKEY 80
#define LKEY 75
#define RKEY 77
#define ENTER 13

#include <iostream>
#include <conio.h>
#include <Windows.h>

using namespace std;

#define UKEY 72
#define DKEY 80
#define LKEY 75
#define RKEY 77
#define ENTER 13

void displayList(boolean[4]);
void setBooleanArray(int, int&, int, boolean[4]);
void initArray(boolean[4]);
void alternateList(boolean[4], int);
void setName(string[]);
void banner();

int mMenu(string playerNames[2], bool CPU)
{
	int c = 0, currentSelection = 1;
	boolean pos[4], altList = false;
	initArray(pos);
	int  y = 0;

	displayList(pos);
	while (1)
	{

		c = 0;

		switch (c = _getch())
		{

		case UKEY:
			system("CLS");
			if (altList == false){ setBooleanArray(c, y, 3, pos); }
			else{ setBooleanArray(c, y, 2, pos); }
			if (altList){ alternateList(pos, 1); }	//NOTE: THIS COULD BECOME AN ISSUE IF MORE MENUS WITH ARROW KEYS ARE IMPLEMENTED
			else{ displayList(pos); }
			break;

		case DKEY:
			system("CLS");
			if (altList == false){ setBooleanArray(c, y, 3, pos); }
			else{ setBooleanArray(c, y, 2, pos); }
			if (altList){ alternateList(pos, 1); }	//NOTE: THIS COULD BECOME AN ISSUE IF MORE MENUS WITH ARROW KEYS ARE IMPLEMENTED
			else{ displayList(pos); }
			break;

		case LKEY:
			if (altList = true)
			{
				altList = false;
				initArray(pos);
				y = 0;
				system("CLS");
				if (altList){ alternateList(pos, 1); }	//NOTE: THIS COULD BECOME AN ISSUE IF MORE MENUS WITH ARROW KEYS ARE IMPLEMENTED
				else{ displayList(pos); }
			}
			break;




		case ENTER:
			if (altList == true)
			{
				if (pos[0]){ return 3; }
				if (pos[1]){ return 5; }
				if (pos[2]){ return 20; }
			}

			if (altList == false && pos[0])//"Play against another player"
			{
				alternateList(pos, 1);
				altList = true;
			}

			if (altList == false && pos[1])//"Play against the computer"
			{
				alternateList(pos, 1);
				altList = true;
				CPU = true;
			}

			if (altList == false && pos[2])
			{
				setName(playerNames);
				y = 0;
				initArray(pos);
				displayList(pos);
			}

			if (altList == false && pos[3])
			{
				return 0;
			}


			break;

		case RKEY: //copy paste from enter function when finished
			break;


		}
	}
}



void setBooleanArray(int c, int& y, int ymax, boolean pos[4]){
	pos[y] = false;
	switch (c)
	{
	case UKEY: //on a press of the up key
		y--;
		break;

	case DKEY: //on a press of the down key
		y++;
		break;


	}


	if (y < 0){ y = (ymax); } //loop around to the other side.
	if (y > ymax){ y = 0; }

	pos[y] = true;


}


void displayList(boolean pos[4]){

	//banner();

	if (pos[0]){ cout << "->"; }
	else{ cout << "  "; }
	cout << "1. P1 vs P2!" << endl;

	if (pos[1]){ cout << "->"; }
	else{ cout << "  "; }
	cout << "2. P1 vs CPU!" << endl;

	if (pos[2]){ cout << "->"; }
	else{ cout << "  "; }
	cout << "3. Set Names" << endl;

	if (pos[3]){ cout << "->"; }
	else{ cout << "  "; }
	cout << "4. Exit" << endl;

}

void initArray(boolean pos[4]){	//this function simply initializes all booleans in pos[] to false, then sets pos[0] to true
	for (int i = 0; i < 4; i++)
	{
		pos[i] = false;
	}

	pos[0] = true;
}

void alternateList(boolean pos[4], int num){	//'num' is a modular indicator of which alt menu to display
	//if num==1, it means enter or right arrow was pressed from "Play Fireball!" on the first menu...
	//so it will display a submenu with the different amounts of lives you can have.
	//if num==2, it means the user chose 'Set Names', which will display the appropriate prompts

	//banner();
	system("CLS");
	cout << "  ";
	cout << "1. P1 vs P2!";

	if (num == 1 || num == 2)
	{
		cout << "\t\t";	//tab formatting intentionally placed within the if block to prevent formatting of the other alt menus
		if (pos[0]){ cout << "->"; }
		else{ cout << "  "; }
		cout << "1. 3 Lives" << endl;
	}

	cout << "  ";
	cout << "2. P1 vs CPU!";

	if (num == 1 || num == 2)
	{
		cout << "\t\t";
		if (pos[1]){ cout << "->"; }
		else{ cout << "  "; }
		cout << "2. 5 Lives" << endl;
	}


	cout << "  ";
	cout << "3. Set Names";

	if (num == 1 || num == 2)
	{
		cout << "\t\t";
		if (pos[2]){ cout << "->"; }
		else{ cout << "  "; }
		cout << "3. 20 Lives" << endl;
	}


	cout << "  ";
	cout << "4. Exit" << endl << endl;


}

void setName(string playerName[])
{
	cout << "Enter a name for Player 1: ";
	getline(cin, playerName[0]);
	cout << "Enter a name for Player 2: ";
	getline(cin, playerName[1]);
	cout << "Names collected!" << endl;
	Sleep(1000);
}


#endif