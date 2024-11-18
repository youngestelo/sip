#pragma once
#ifndef Objects_h
#define Objects_h

#define CC(COLOR) SetConsoleTextAttribute(HANDLE_CONSOLE, COLOR)
HANDLE HANDLE_CONSOLE = GetStdHandle(STD_OUTPUT_HANDLE);

class Grid
{
	const unsigned int LAYERS;
	const unsigned int WIDTH;
protected:
	unsigned int FREE_SPACE;
	char** GRID;
public:
	Grid() : Grid(1, 1) {}
	Grid(unsigned int _L, unsigned int _W) : LAYERS{ _L + 5 }, WIDTH{ _W }, FREE_SPACE {5}
	{
		GRID = new char* [LAYERS];
		for (short I = 0; I < LAYERS; I++)	// Initialize grid layout
		{
			GRID[I] = new char[WIDTH];
			for (short J = 0; J < WIDTH; J++)
			{
				if (I < LAYERS - FREE_SPACE) GRID[I][J] = (char)248;
				else GRID[I][J] = '.';
			}
		}
	}

	void displayGrid()
	{
		CC(0);
		for (short I = 0; I < LAYERS - FREE_SPACE; I++)
		{
			for (short J = 0; J < WIDTH; J++)
			{
				if (GRID[I][J] != '.') { CC(15); cout << GRID[I][J]; CC(0); }	// Alien's color
				else cout << GRID[I][J];
			}
			cout << endl;
		}

		CC(0);	// Trajectory color
		for (short I = LAYERS - FREE_SPACE; I < LAYERS; I++)
		{
			for (short J = 0; J < WIDTH; J++)
			{
				if (GRID[I][J] == (char)219) { CC(7); cout << GRID[I][J]; CC(0); }
				else if (GRID[I][J] == (char)186) { CC(3); cout << GRID[I][J]; CC(0); }
				else if (GRID[I][J] == (char)248) { CC(15); cout << GRID[I][J]; CC(0); }
				else cout << GRID[I][J];
			}
			cout << endl;
		}
	}

	~Grid()
	{
		for (auto i = 0; i < LAYERS; i++)
		{delete[] GRID[i];}
		delete[] GRID;
	}

	const unsigned int getLayers() { return LAYERS; }
	const unsigned int getWidth() { return WIDTH; }
	const unsigned int getFreeSpace() { return FREE_SPACE; }
	char** getGrid() { return GRID; }
}; 

void displayFrame(unsigned int gip, unsigned int gie, Grid& gig)
{
	cout << " SCORE\t ELIMS\n " << gip << "\t " << gie << "\n\n";
	gig.displayGrid();
}

class Player
{
	Grid& GridHandle;
	unsigned int PositionX;
	char PlayerSymbol = (char)254;
	char* MovementLayer;
public:
	Player() : Player(*new Grid(1, 1)) {}
	Player(Grid& Handler) : GridHandle{Handler}, PositionX{Handler.getWidth()/2}, MovementLayer{new char[Handler.getWidth()]}
	{
		for (short I = 0; I < Handler.getWidth(); I++)	// Initialize movement layer
		{MovementLayer[I] = (char)'.'; }
		MovementLayer[PositionX] = PlayerSymbol;
	}
	
	void displayMovementLayer()
	{
		CC(0);
		for (short I = 0; I < GridHandle.getWidth(); I++)
		{
			if (MovementLayer[I] != '.') { CC(11); cout << MovementLayer[I]; CC(0);}
			else cout << MovementLayer[I];
		}
		cout << endl;
		CC(7);
	}

	bool movePlayer(bool TO_LEFT)				// Player's movement
	{
		if (TO_LEFT && PositionX > 0)
		{
			swap(MovementLayer[PositionX], MovementLayer[PositionX - 1]); 
			PositionX--;
			return TRUE;
		}
		else if (!TO_LEFT && PositionX < GridHandle.getWidth() - 1)
		{
			swap(MovementLayer[PositionX], MovementLayer[PositionX + 1]);
			PositionX++;
			return TRUE;
		}
		return FALSE;
	}

	void shotRaycast(unsigned int* _gip, unsigned int* _gie)	// Shot
	{
		unsigned short CURRENT_LAYER = GridHandle.getLayers() - 1;
		GridHandle.getGrid()[CURRENT_LAYER][PositionX] = (char)219;
		while (TRUE)
		{
			system("cls");
			displayFrame(*_gip, *_gie, GridHandle);
			displayMovementLayer();
			if (CURRENT_LAYER - 1 > 0)
			{
				swap(GridHandle.getGrid()[CURRENT_LAYER][PositionX], GridHandle.getGrid()[CURRENT_LAYER - 1][PositionX]);
				CURRENT_LAYER--;
				if (GridHandle.getGrid()[CURRENT_LAYER - 1][PositionX] == (char)248)	// Alien has been found
				{
					Sleep(250);
					swap(GridHandle.getGrid()[CURRENT_LAYER][PositionX], GridHandle.getGrid()[CURRENT_LAYER - 1][PositionX]);
					GridHandle.getGrid()[CURRENT_LAYER - 1][PositionX] = '.';
					GridHandle.getGrid()[CURRENT_LAYER][PositionX] = '.';
					*_gip += 10; *_gie += 1;
					break;
				}
			}
			else // If alien wasn't found
			{ GridHandle.getGrid()[CURRENT_LAYER][PositionX] = '.'; break; } 

			Sleep(150);
		}
		system("cls");
		displayFrame(*_gip, *_gie, GridHandle);
		displayMovementLayer();
	}

	void MirrorShot(unsigned int* _gip, unsigned int* _gie)	  // Shot from aliens
	{
		srand(time(NULL));
		int ShotPosition = 0 + rand() % GridHandle.getWidth();

		unsigned short CURRENT_LAYER = GridHandle.getLayers() - GridHandle.getFreeSpace();
		GridHandle.getGrid()[CURRENT_LAYER][ShotPosition] = (char)186;

		Sleep(150);
		for (auto I = 0; I < GridHandle.getFreeSpace() - 1; I++)
		{
			system("cls");
			displayFrame(*_gip, *_gie, GridHandle);
			displayMovementLayer();
			Sleep(150);
			swap(GridHandle.getGrid()[CURRENT_LAYER][ShotPosition], GridHandle.getGrid()[CURRENT_LAYER + 1][ShotPosition]);
			CURRENT_LAYER++;
		}
		system("cls");
		displayFrame(*_gip, *_gie, GridHandle);
		displayMovementLayer();

		if (MovementLayer[ShotPosition] == (char)254) { (*_gip >= 50) ? *_gip -= 50 : *_gip = 0; } // Player has been found

		GridHandle.getGrid()[CURRENT_LAYER][ShotPosition] = '.';
		Sleep(150);
		system("cls");
		displayFrame(*_gip, *_gie, GridHandle);
		displayMovementLayer();
	}

	~Player() 
	{delete[] MovementLayer;}

};

class Game : public Grid, public Player
{
public:
	unsigned int GAME_INSTANCE_POINTS = 0;
	unsigned int GAME_INSTANCE_ELIMINATIONS = 0;
	unsigned int GAME_INSTANCE_TOTAL_SHOTS = 0;

	bool		 GAME_INSTANCE_SHOT_POSSIBLE = TRUE;

	Grid		 GAME_INSTANCE_GRID;			
	Player		 GAME_INSTANCE_PLAYER;			

	Game(unsigned int _layers, unsigned int _width) :
		GAME_INSTANCE_POINTS {0},
		GAME_INSTANCE_ELIMINATIONS {0},
		GAME_INSTANCE_TOTAL_SHOTS {0},

		GAME_INSTANCE_SHOT_POSSIBLE {TRUE},

		GAME_INSTANCE_GRID {*new Grid(_layers, _width)},
		GAME_INSTANCE_PLAYER {*new Player(GAME_INSTANCE_GRID)}
	{}
};

#endif