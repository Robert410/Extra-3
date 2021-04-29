#include "UI.h"

#include <set>

using namespace std;

void Analytic::getWords(string name,string surname) {
	set < string >s;
	ifstream fin;
	fin.open("date.txt");
	if (fin.eof())
		return;
	bool ok;
	while(true){
		string dont, word, nameUsed, surnameUsed,test;
		nameUsed = ""; surnameUsed = ""; test = "";
		ok = 0;
		fin >> test;
		if (test != "Player")break;
		getline(fin, dont);
		fin >> nameUsed >> surnameUsed;
		if (nameUsed == name && surnameUsed == surname)ok = 1;
		if(ok==1)
		cout << "| PLAYER: " << nameUsed << " " << surnameUsed << " ";
		getline(fin, dont); 
		getline(fin, dont);
		getline(fin, dont); 
		getline(fin, dont); 
		fin >> word;
		while (word != "Number") {
			s.insert(word);
			fin >> word;
		}
		if (ok == 1)
		cout << " has used: " << s.size() << " unique words and used: ";
		getline(fin, dont); 
		int a;
		fin >> a;
		if (ok == 1)
		cout << a << " hints. ";
		getline(fin, dont);
		getline(fin, dont); 
		if(ok==1)cout << dont << endl;
		s = set<string>();
		if (ok == 1)
		cout << "\n";
	}
}