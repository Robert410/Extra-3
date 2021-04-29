#pragma once
#include "words.h"

#define vecStr vector<string>

 /*struct output {
	string name,surname;
	string begin, target;
	vecStr allUsed;
	vecStr UsedTarget;
	int hints;
};*/

class UI
{
	//friend class Analytic;
private:
	vecStr wordsA;
	Solution<vector<vecStr>> ob;
	vector<vecStr> sol;
	string name,surname;

	vecStr Words;
	vector<vecStr> WordsUsed;
	vecStr AllWords;

	int accounts = 0;

	int LineofConsole;
	int hints = 0;

	void readWordsPlayer();
	void readWordsAutomatic();
	void createGraph();
	void LoadDictionary();
	void printSolution(string, string);
	string checkreading(string, string);
	void BeginTheGame(string, string);
	string checkNext(string, string, int, string);
	string hint(int, string);
	void findWords(int, string&, string&);
	bool dif1(string, string);
	void printOut();
	int get_rand();
	string randomWord(int sizeW);
public:
	void StartAll();
	int getAccoutns() { return accounts; }
};

class Analytic : public UI {
public:
	void getWords(string, string);
};