#pragma once

#include "GameModes.h"

GameModes::GameModes()
{
}

string GameModes::Caesar()
{
	string result;

	fstream file;
	file.open("DataBank/WORDS.txt");

	int word = (rand() * 12913) % 377072;

	cout << word << endl;

	int n = 0;

	while (n < word) {
		getline(file, result);
		n++;
	}

	getline(file, result);

	for (char& c : result)
		c = c & ~' ';

	cout << result << endl;

	file.close();

	return result;
}

string GameModes::Caesar_E(int key, string c) {


	for (int i = 0; i < c.length(); i++) {

		c[i] = (c[i] + key - 65) % 26;
		c[i] = c[i] + 65;
	}

	return c;
}