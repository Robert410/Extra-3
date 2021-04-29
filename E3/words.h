#pragma once

#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>

using namespace std;

template <typename T>
class Solution {
public:
    void createMap(vector<string>& wordList);

    T findSolutionPath(string beginWord, string endWord, vector<string>& wordList);

private:
    unordered_set<string> dict, cop;

    void findChildren(string word, unordered_set<string>& next, unordered_set<string>& dict, unordered_map<string, vector<string>>& PathTarget);

    void genSolutionPath(string beginWord, string endWord, unordered_map<string, vector<string>>& PathTarget, vector<string>& ChildrenWords, T& finalSol);
};


template <typename T>
void Solution<T>::createMap(vector<string>& wordList) {
    dict = unordered_set<string>(wordList.begin(), wordList.end());
    cop = dict;
}



template <typename T>
T Solution<T>::findSolutionPath(string beginWord, string endWord, vector<string>& wordList) {
    unordered_set<string>  current, next;

    unordered_map<string, vector<string>> PathTarget;
    T finalSol;
    vector<string> ChildrenWords;
    current.insert(beginWord);
    ChildrenWords.push_back(beginWord);
    while (true) {
        for (string word : current) {
            dict.erase(word);
        }
        for (string word : current) {
            findChildren(word, next, dict, PathTarget);
        }
        if (next.empty()) {
            break;
        }
        if (next.find(endWord) != next.end()) {
            genSolutionPath(beginWord, endWord, PathTarget, ChildrenWords, finalSol);
            break;
        }
        current.clear();
        swap(current, next);
    }
    dict = cop;
    /*for (auto i : PathTarget) {
        cout << i.first << " ";
    }*/
    return finalSol;
}


template <typename T>
void Solution<T>::findChildren(string word, unordered_set<string>& next, unordered_set<string>& dict, unordered_map<string, vector<string>>& PathTarget) {
    string parent = word;
    for (int i = 0; i < word.size(); i++) {
        char t = word[i];
        for (int j = 0; j < 26; j++) {
            word[i] = 'a' + j;
            if (dict.find(word) != dict.end()) {
                next.insert(word);
                PathTarget[parent].push_back(word);
            }
        }
        word[i] = t;
    }
}


template <typename T>
void Solution<T>::genSolutionPath(string beginWord, string endWord, unordered_map<string, vector<string>>& PathTarget, vector<string>& ChildrenWords, T& finalSol) {
    if (beginWord == endWord) {
        finalSol.push_back(ChildrenWords);
    }
    else {
        for (string child : PathTarget[beginWord]) {
            ChildrenWords.push_back(child);
            genSolutionPath(child, endWord, PathTarget, ChildrenWords, finalSol);
            ChildrenWords.pop_back();
        }
    }
}
