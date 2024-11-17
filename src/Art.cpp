#include <iostream>
#include <Windows.h>
using namespace std;

void _art_sip_small()
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
	cout << "                           _                 _                     _         " << endl <<
			"  ____ __  __ _ __ ___    (_)_ ___ ____ _ __| |___ _ _ ___    _ __| |_  _ ___" << endl <<
			" (_-< '_ \\/ _` / _/ -_)   | | ' \\ V / _` / _` / -_) '_(_-<   | '_ \\ | || (_-<" << endl <<
			" /__/ .__/\\__,_\\__\\___|   |_|_||_\\_/\\__,_\\__,_\\___|_| /__/   | .__/_|\\_,_/__/" << endl <<
			"    |_|                                                      |_|             " << endl ;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	cout << "\nSelect the size of playing field\n";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
	cout << "  [1] Easy\n";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
	cout << "  [2] Medium\n";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
	cout << "  [3] Master\n";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 3);
	cout << "  [4] Custom\n";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
	cout << "  [0] Exit\n";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
}