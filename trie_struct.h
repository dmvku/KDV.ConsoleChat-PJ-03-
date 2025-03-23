#pragma once
#include "console.h"

#include <string>
#include <vector>
#include <iomanip>
#include <conio.h>
#include <cstring>
#include <fstream>

const int ALPHABET_SIZE = 26;

struct TrieNode
{
    struct TrieNode* children[ALPHABET_SIZE];    
    bool isEndOfWord;
};

TrieNode* getNewNode();
void insert(TrieNode* root, std::string word);
TrieNode* remove(TrieNode* root, std::string word, int depth = 0);
bool isEmpty(TrieNode* root);
bool search(TrieNode* root, std::string key);
TrieNode* searchKey(TrieNode* root, std::string word);
void searchAllStrings(TrieNode* root, std::string& word, std::vector<std::string>& predictiveList);
void predictiveDictionary(TrieNode* node, std::string& key, std::vector<std::string>& predictiveList);
void findMinPrefixes(TrieNode* root, char buffer[] , int index, std::string& word);
std::string creatingMessage(TrieNode* root, int& coordX, int& coordY);
void read_dict(std::string& filename, TrieNode* root);
void write_dict(std::string& filename, TrieNode* root);
bool isContainsNonLetters(std::string prefix);
