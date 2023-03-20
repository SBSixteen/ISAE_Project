#pragma once

#include "GameModes.h"

GameModes::GameModes()
{
}

string GameModes::get_Word()
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

string GameModes::Vignere_K()
{
	string A = "";

	int m = rand() % 26;
	int len = (rand() % 10) + 1;
	char g = 'A';

	for (int i = 0; i < len; i++) {
		g = 'A' + m;
		A += g;
		g = 'A';
		m = rand() % 26;
	}

	cout << A << endl;

	return A;
}

string GameModes::Vignere_E(string K, string R)
{
	string A = "";
	int j = 0;

	for (int i = 0; i < R.length(); i++) {
		A += ((K[i % K.length()] - 65) + (R[i] - 65)) % 26;
		A[i] = A[i] + 65;
	}

	cout << A << endl;

	return A;
}