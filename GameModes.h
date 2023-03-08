#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

class GameModes
{
public:
	GameModes();
	static string get_Word();
	static string Caesar_E(int key, string p);
	static string Vignere_K();
	static string Vignere_E(string k, string r);
};

#pragma once
