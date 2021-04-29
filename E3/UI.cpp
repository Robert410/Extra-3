#pragma warning(disable : 4996)

#include "UI.h"
#include "colormod.h"
#include <windows.h>
#include <ctime>
#include <time.h>
#include <random>

void setcursor(short int x, short int y) {
    auto console = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(console, { x, y });
}

Color::Modifier red(Color::FG_RED);
Color::Modifier green(Color::FG_GREEN);
Color::Modifier def(Color::FG_DEFAULT);

Color::Modifier red_bg(Color::BG_GREEN);
Color::Modifier def_bg(Color::BG_DEFAULT);

void UI::LoadDictionary() {
    wordsA.reserve(3775);
    ifstream bigFile("dictionary.txt");
    constexpr size_t bufferSize = 1024;
    unique_ptr<char[]> buffer(new char[bufferSize]);
    while (bigFile)
    {
        bigFile.read(buffer.get(), bufferSize);
        string word = "";
        string a = buffer.get();
        for (auto x : a) {
            if (x == ' ' or x == '\n') {
                wordsA.push_back(word);
                word = "";
            }
            else {
                word = word + x;
            }
        }
    }
}

void UI::createGraph() {
    ob.createMap(wordsA);
}

string UI::checkreading(string a, string b) {
    if (a.size() != b.size())
        return "The words has to have the same size";
    if (find(wordsA.begin(), wordsA.end(), a) == wordsA.end())
        return "The word " + a + " does not exist";
    if (find(wordsA.begin(), wordsA.end(), b) == wordsA.end())
        return "The word " + b + " does not exist";
    return "";
}

string UI::checkNext(string a, string b, int pos, string word) {
    if (a.size() != word.size()) {
        pos = pos - 1;
        return "The words must have the same size";
    }

    bool ok = 0;
    for (auto j:sol)
        if (word == j[pos]) {
            ok = 1;
            break;
        }

    if (ok == 0){
        pos = pos - 1;
        return "You cannot continue with this word";
    }
    cout << "| Ok, "<<green<<"good"<<def<<"\n";
    setcursor(143, ++LineofConsole); cout << "|\n";

    return "";
}

string UI::hint(int pos, string last) {

    for (auto i : sol) 
        if (i[pos - 1] == last){
            hints++;
            cout << "|       The word added is: ->  "; 
            //red << i[pos] << def 
            for (int j = 0; j < i[pos].size(); j++) {
                if (i[pos][j] == last[j])
                    cout << i[pos][j];
                else
                    cout << red << i[pos][j] << def;
            }
            cout << " <- and you will continue from this one\n";
            setcursor(143, ++LineofConsole); cout << "|\n";
            return i[pos];
        }
    
    return "";
}
int UI::get_rand() {
    
    return rand() % wordsA.size();
}

string UI::randomWord(int sizeW) {
    int sizeContainer = wordsA.size();
    
    int lucky = rand() % sizeContainer;
    while(wordsA[lucky].size() != sizeW)
        lucky = rand() % sizeContainer;
    return wordsA[lucky];
}

void UI::findWords(int letters, string& a, string& b) {
  
    a = randomWord(letters);
    b = randomWord(letters);
    
}

void UI::BeginTheGame(string a, string b) {
  
    bool ok = 0, win = 0;
    sol = ob.findSolutionPath(a, b, wordsA);
    if (sol == vector<vecStr>()) {
        cout << "|                                          It is not possible to reach this target                                                             |\n";
        cout << "|                                                                                                                                              |\n";
        /*if(dif1(a,b) == 1){
        cout << "|                                   "<<a<<" -> "<<b<<"                                                                                         |\n";
        LineofConsole++;
        }*/
        LineofConsole += 2;
        ok = 1;
    }
    
    string test, checkRead, last;
    int pos = 0;
    last = a;
    while (ok == 0) {
        pos++;
        cout << "|      What do you think is the next word?";
        setcursor(143, ++LineofConsole); cout << "|\n";
        cout << "|  -> ";
        cin >> test;
        setcursor(143, ++LineofConsole); cout << "|\n";
        if (test == "-1") break;
        if (test == "F1") {
            if (last == b){
                pos--;
                cout << "->You already finished!!\n";
                setcursor(143, ++LineofConsole); cout << "|\n";
                continue;
            }
            if (pos == sol[0].size() - 1){
                pos--;
                cout << "->Do you really need help for the last word?\n";
                setcursor(143, ++LineofConsole); cout << "|\n";
                continue;
            }
            last=hint(pos, last);

        }
        else{
        AllWords.push_back(test);
        checkRead = checkNext(a, b, pos, test);
        if (checkRead != "") {
            if (dif1(last, test) == 1) {
                if (find(wordsA.begin(), wordsA.end(), test) != wordsA.end()){
                    Words.push_back(test);
                    BeginTheGame(test, b);
                    return ;
                }
            }

            pos--;
            cout << red << "| !!! " << def << checkRead << red << "!!!                                                                                             \n" << def ;
            setcursor(143, ++LineofConsole); cout << "|\n";
            /*cout << "\nWanna continue?\n->(y/n): ";
            char PlayAgain;
            cin >> PlayAgain;
            if (PlayAgain == 'n')
                break;*/
            cout << "|      Wanna continue?                                                                                                                         |";
            cout << "\n|               ->(y/n): ";

            LineofConsole ++;
            char PlayAgain;
            cin >> PlayAgain;
            setcursor(143, ++LineofConsole); cout << "|\n";
            if (PlayAgain == 'n')
                break;
        }
        else {
            last = test;
            Words.push_back(last);

        }
        if (test == b) ok = 1, win = 1;
        }
    }
    if(win == 1){
        cout << "|                                                                                                                                              |\n";
        cout << "|                                                           "<<green<<"WELL DOOONEEE!!!"<<def<<"\n\n";
        LineofConsole++;
        setcursor(143, ++LineofConsole); cout << "|\n";
    }
    return ;
}

void UI::readWordsPlayer() {
    
    string a, b;
    cout << "|                                        Hello, whats your name?(Last name & First name) -> ";
    cin >> name>>surname;
    setcursor(143, ++LineofConsole); cout << "|\n";
    cout << "|    Hello " << name<<" "<<surname<<"!!! And welcome! Happy to have you here"  << "\n";
    setcursor(143, ++LineofConsole); cout << "|\n";
    while (true) {

        int randLetter, letters;
        
        cout << "| Soo, adventure, your words will have how many letters?";
        setcursor(143, ++LineofConsole); cout << "|\n";
        cout << "|  -> ";
        cin >> letters;
        setcursor(143, ++LineofConsole); cout << "|\n";


        a = ""; b = "";
        while ((a.size() != letters || b.size() != letters)) {
            

            randLetter = (rand()*RAND_MAX + rand()) % wordsA.size();
            a = wordsA[randLetter];

            randLetter = (rand() * RAND_MAX + rand()) % wordsA.size();
            b = wordsA[randLetter];

        }

        cout <<"| The words you got are: "<<red<< a<<def<<" and you have to reach: " <<red<< b<<def;
        setcursor(143, ++LineofConsole); cout << "|\n";

        string checkRead;
        checkRead = checkreading(a, b);
        if (checkRead != ""){
            cout << red << "| !!! " << def << checkRead << red << "!!!                                                                                                          |\n" << def;

            LineofConsole++;
            
        }else{
        sol = ob.findSolutionPath(a, b, wordsA);

        cout << "|\n| LET THE GAMES BEGIN:\n\n";
        setcursor(143, ++LineofConsole); cout << "|\n";
        setcursor(143, ++LineofConsole); cout << "|\n";
        Words = vecStr();
        Words.push_back(a);
        
        BeginTheGame(a,b);
        WordsUsed.push_back(Words);
        }

        cout << "|      Wanna play again?                                                                                                                       |";
        LineofConsole++;
       
        cout << "\n|               ->(y/n): ";

        char PlayAgain;
        cin >> PlayAgain;
        setcursor(143, ++LineofConsole); cout << "|\n";
        if (PlayAgain == 'n')
            break;
    }
}

void UI::readWordsAutomatic() {
    string a, b;
    while (true) {
       
        cout << "|  Start word: "; cin >> a;
        
        setcursor(143, ++LineofConsole); cout << "|\n";
        cout << "|  Target word: "; cin >> b;
        setcursor(143, ++LineofConsole); cout << "|\n";
        string checkRead;
        checkRead = checkreading(a, b);
        if (checkRead != "") {
            cout <<red<< "| !!! "<<def << checkRead << red << "!!!                                                                                                          |\n" << def;

            LineofConsole++;
        }
        else {
            
            sol = ob.findSolutionPath(a, b, wordsA);
            if (sol == vector<vecStr>()) {
                cout << "|                                          It is not possible to reach this target                                                             |\n";
                cout << "|                                                                                                                                              |\n";
                /*if(dif1(a,b) == 1){
                cout << "|                                   "<<a<<" -> "<<b<<"                                                                                         |\n";
                LineofConsole++;
                }*/
                LineofConsole += 2;
                break;
            }
            else
                printSolution(a, b);
        }
        cout << "|                                                                                                                                              |\n";

        cout << "|      Wanna play again?                                                                                                                       |";
            cout<<"\n|               ->(y/n): ";
        
        LineofConsole+=2;
        char PlayAgain;
        cin >> PlayAgain;
        setcursor(143, ++LineofConsole); cout << "|\n";
        if (PlayAgain == 'n')
            break;
    }
}

bool UI::dif1(string a, string b) {
    int count = 0;
    for (int i = 0; i < a.size(); i++)
        if (a[i] != b[i]) count++;
    return count == 1;
}

void UI::printSolution(string a, string b) {
   
        cout << "|                                          Sow we will have:                                                                                   |\n";
        cout << "|                                                                                                                                              |\n";
        /*if(dif1(a,b) == 1){
        cout << "|                                   "<<a<<" -> "<<b<<"                                                                                         |\n";
        LineofConsole++;
        }*/
        LineofConsole += 2;
        for (int j = 0; j < sol.size(); j++) {
            cout << "|             ";
            for (int i = 0; i < sol[j].size(); i++) {
                if (i == sol[j].size() - 1)
                    cout << sol[j][i];
                else
                    cout << sol[j][i] << " -> ";

            }
            LineofConsole++; setcursor(143, LineofConsole); cout << "|";
            cout << endl;
        }
    
    
}

void UI::printOut() {
    accounts++;
    ofstream fout;
    fout.open("date.txt", ios_base::app);
    fout << "Player -> \n" << name << " " << surname;

    fout << "\nWords used to get to the target/targets: \n";
    for (int i = 0; i < WordsUsed.size(); i++)
        for (int j = 1; j < WordsUsed[i].size(); j++)
                fout << WordsUsed[i][j] << " ";
    fout << "\nAll words used by " << name << " " << surname<<'\n';
    for (auto i : AllWords) {
        fout << i << " ";
    }
    fout << '\n';
    fout << "Number of hints used -> \n" << hints;
    time_t now = time(0);
    char* dt = ctime(&now);
    fout << "\nAnd he played at: " << dt << '\n';


}

void UI::StartAll() {
    time_t begin, end;
    cout << "------------------------------------------------------------------------------------------------------------------------------------------------\n";
    cout << "|                                             Hello brave adventure, and welcome to the world of letters.                                      |\n";
    cout << "|                                                      Maybe you are thinking why are you here?                                                |\n";
    cout << "|                                                     Let's play a little simple game.                                                         |\n";
    cout << "|                                          And don't worry. I will be here with you to help at any time <3                                     |\n";
    cout << "|                                                                                                                                              |\n";
    cout << "|  The "<<red<<"rules"<<def<<" of this game are simple:                                                                                                          |\n";
    cout << "|       " << green << "1." << def << " You will have two words ( the start and the end) and you have to reach that end by changing 1 letter from the start word            |\n";
    cout << "|       " << green << "2." << def << " When you are changing a letter, that new word has to exist in the dictionary!!                                                      |\n";
    cout << "|       " << green << "3." << def << " At every moment you can type 'F1' and I will help you with 1 word                                                                   |\n";
    cout << "|       " << green << "4." << def << " At every moment you can type '-1' and that game will end                                                                            |\n";
    cout << "|                                                                                                                                              |\n";
    cout << "------------------------------------------------------------------------------------------------------------------------------------------------\n";
    cout << "|                                                                                                                                              |\n";
    cout << "|   From now on, let's begin our travel:                                                                                                       |\n";
    cout << "|   First of all, how would you like to play? (" << red << "Automatic" << def << "/" << red << "Playing" << def << " mode).                                                                        |\n";
    cout << "|      " << green << "Automatic mode" << def << ": you select the starting and the target words and I will display the transformations you need to perform                 |\n";
    cout << "|      " << green << "Playing mode" << def << ": you select the length and I will give you the words. You have to find the path                                            |\n";
    cout << "|      " << green << "MASTER mode" << def << ": you are in the analytic mode and you can see all the stats from all the users                                              |\n";
    cout << "|      So, how will be?(a/p/m): ";
    string type; cin >> type;
    setcursor(143,19); cout << "|\n";
    LineofConsole = 19;
    time(&begin);// ->>> START TIME
    
    if (type == "a" or type == "automatic") {
        cout << "|   Let me load for you the dictionary                                                                                                         |\n";
        LineofConsole++;
        LoadDictionary();
        cout << "|   OK, good good, a lot of words, not gonna lie                                                                                               |\n";
        cout << "|                                                                                                                                              |\n";
        LineofConsole += 2;
        createGraph();
        readWordsAutomatic();
    }
    else if (type == "p" or type == "player") {
        cout << "|   Let me load for you the dictionary                                                                                                         |\n";
        LineofConsole++;
        LoadDictionary();
        cout << "|   OK, good good, a lot of words, not gonna lie                                                                                               |\n";
        cout << "|                                                                                                                                              |\n";
        LineofConsole += 2;
        createGraph();
        readWordsPlayer();
    }
    else if (type == "m" or type == "master") {
        Analytic an;
        cout << "| Give me the user exactly how it is (with full name and First name): ";
        string nm, surnm; 
        cin >> nm >> surnm;
        an.getWords(nm,surnm);
    }
    else
    cout << "| bye bye ...                                                                                                                                   |";
    cout << "------------------------------------------------------------------------------------------------------------------------------------------------\n";
    if(name != "")
    printOut();
    time(&end);// ->>> END TIME

    double difference = difftime(end, begin);
    cout << name<<" "<<surname<<". You've played for " << difference<<" seconds and used: "<<hints<<" hints.\n";
    //printf("You've played for %.2lf seconds.\n", difference);
}