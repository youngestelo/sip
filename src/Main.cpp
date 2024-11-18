#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <chrono>
#include <sstream>
#include <fstream>
using namespace std;

#include "Art.h"
#include "Objects.h"

void displayFrame(Game& _game)
{
	cout << " SCORE\t ELIMS\n " << _game.GAME_INSTANCE_POINTS << "\t " << _game.GAME_INSTANCE_ELIMINATIONS << "\n\n";
	_game.GAME_INSTANCE_GRID.displayGrid();
	_game.GAME_INSTANCE_PLAYER.displayMovementLayer();
}
#define DISPLAYFRAME displayFrame(GAME_INSTANCE);

int main()
{
	SetConsoleTitleA("Space Invaders Plus");
	system("cls");
	_art_sip_small();												// Display Logo-Art & Menu
	unsigned int MENU_CHOICE, INT_LAYERS, INT_WIDTH;
	try
	{
		cout << "Your choice: "; cin >> MENU_CHOICE;
		switch (MENU_CHOICE)
		{
		case 1: { INT_LAYERS = 3; INT_WIDTH = 17; break; }
		case 2: { INT_LAYERS = 4; INT_WIDTH = 22; break; }
		case 3: { INT_LAYERS = 5; INT_WIDTH = 27; break; }
		case 4: 
		{ 
			cout << "Layers: "; cin >> INT_LAYERS; 
			cout << "Width: "; cin >> INT_WIDTH; 

			if (INT_LAYERS < 1) throw "Error: There must be at least 1 layer";
			if (INT_WIDTH < 1)	throw "Error: The width must be at least 1";
			break; 
		}
		case 0: return 0;
		default: throw 0;
		}
		system("cls");
	}
	catch (const char* err) { system("cls"); cerr << err; Sleep(2000); system("cls"); main(); }
	catch (...)				{ system("cls"); cerr << "Fatal Error! Please, restart the game.\n"; return 0; }

	Game GAME_INSTANCE(INT_LAYERS, INT_WIDTH);	// INITIALIZE GAME (*)

	auto GAME_INSTANCE_TIMER_START = chrono::high_resolution_clock::now(); // Timer start

	DISPLAYFRAME;

	while (GAME_INSTANCE.GAME_INSTANCE_SHOT_POSSIBLE)
	{
		GAME_INSTANCE.GAME_INSTANCE_SHOT_POSSIBLE = FALSE;
		if (_kbhit())												// Register player's keyboard input
		{
			int KEYWORD = static_cast<int>(_getch()); KEYWORD = _getch();
			if (KEYWORD == 75 || KEYWORD == 77)
			{
				bool CLEAR_AFTER_MOVEMENT = (KEYWORD == 75) ? GAME_INSTANCE.GAME_INSTANCE_PLAYER.movePlayer(TRUE) : GAME_INSTANCE.GAME_INSTANCE_PLAYER.movePlayer(FALSE);
				if (CLEAR_AFTER_MOVEMENT) { system("cls"); DISPLAYFRAME; }
			}
			else if (KEYWORD == 72)
			{
				GAME_INSTANCE.GAME_INSTANCE_PLAYER.shotRaycast(&GAME_INSTANCE.GAME_INSTANCE_POINTS, &GAME_INSTANCE.GAME_INSTANCE_ELIMINATIONS); GAME_INSTANCE.GAME_INSTANCE_TOTAL_SHOTS++;
				srand(time(NULL)); short MirrorShotSuccess = 0 + rand() % 2;
				if (MirrorShotSuccess == 0) GAME_INSTANCE.GAME_INSTANCE_PLAYER.MirrorShot(&GAME_INSTANCE.GAME_INSTANCE_POINTS, &GAME_INSTANCE.GAME_INSTANCE_ELIMINATIONS);
			}
		}
		GAME_INSTANCE.GAME_INSTANCE_SHOT_POSSIBLE = TRUE;

		if (GAME_INSTANCE.GAME_INSTANCE_ELIMINATIONS >= INT_LAYERS * INT_WIDTH) break;		// End //
	}

	auto GAME_INSTANCE_TIMER_END = chrono::high_resolution_clock::now();		// Count elapsed time
	chrono::duration<double> GAME_INSTANCE_TIMER = GAME_INSTANCE_TIMER_END - GAME_INSTANCE_TIMER_START;

	Sleep(1000); system("cls");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
	cout << "[ Congratulations! You have destroyed all targets! ]\n";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
	cout << "[ Highscore: " << GAME_INSTANCE.GAME_INSTANCE_POINTS << " Eliminations: " << GAME_INSTANCE.GAME_INSTANCE_ELIMINATIONS << " ]\n";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
	cout << "[ Elapsed Time: " << GAME_INSTANCE_TIMER.count() << "s ]\n";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	Sleep(1750);

	try
	{
		int FINAL_MENU_CHOICE;
		cout << "\nWrite your statistic to a file?\n  [1] Yes \n  [2] No, exit\nYour choice: "; cin >> FINAL_MENU_CHOICE;

		switch (FINAL_MENU_CHOICE)
		{
		case 1:
		{
			cout << "\nWorking with a file...\n  [1] Write to a new file\n  [2] Append to the file\nYour choice: "; cin >> FINAL_MENU_CHOICE;
			string FILE_NAME; 
			cout << "Enter text-file name: "; cin.ignore(); getline(cin, FILE_NAME);

			stringstream SAMPLE_STREAM;
			string MODE = (MENU_CHOICE == 1) ? "EASY" : (MENU_CHOICE == 2) ? "MEDIUM" : (MENU_CHOICE == 3) ? "MASTER" : "CUSTOM";
			SAMPLE_STREAM << "HIGHSCORE : " << GAME_INSTANCE.GAME_INSTANCE_POINTS << endl
						  << "ELIMINATIONS : " << GAME_INSTANCE.GAME_INSTANCE_ELIMINATIONS << endl
						  << "ELAPSED TIME : " << GAME_INSTANCE_TIMER.count() << "s" << endl
						  << "MODE : " << MODE;
			ofstream FILE;
			if (FINAL_MENU_CHOICE == 1)
			{
				FILE.open(FILE_NAME + ".txt");
				FILE << SAMPLE_STREAM.rdbuf() << endl << endl;
				cout << "[ Successfully wrote statistic to a file! ]\n";
			}
			else if (FINAL_MENU_CHOICE == 2)
			{
				FILE.open(FILE_NAME + ".txt", ios::app);
				FILE << SAMPLE_STREAM.rdbuf() << endl << endl;
				cout << "[ Successfully added statistic to a file! ]\n";
			}
			else throw 0;
			FILE.close();
			break;
		}
		case 2: { break; }
		default: throw 0;
		}

		Sleep(2000);
		main(); // Return

	}
	catch (...) { system("cls"); cerr << "Fatal Error! Please, restart the program.\n"; return 0; }
}