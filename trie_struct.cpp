#include <iostream>
//#include <bits/stdc++.h>
#include "trie_struct.h"

using namespace std;

// Возвращет новый узел с пустыми детьми
TrieNode* getNewNode()
{
    // Выделяем память по новый узел
    struct TrieNode* pNode = new TrieNode;

    // устанавливаем флаг конца слова в false
    pNode->isEndOfWord = false;

    // инициализируем детей нулевым уазателем
    for (int i = 0; i < ALPHABET_SIZE; i++)
    {
        pNode->children[i] = nullptr;
    } 
    return pNode;
}

// Вставляет ключ в дерево, если его нет, 
// иначе если ключ явлется префксом узла дерева 
// помечает вкачестве литового т.е. конец слова
void insert(TrieNode* root, std::string key)
{
    TrieNode* node = root;

    for (int i = 0; i < key.length(); i++)
    {
        // вычисляем индекс в алфите через смещение относительно первой буквы
        int index = key[i] - 'a';

        // если указатель пустрой, т.е. детей с таким префиксом нет
        // создаем новый узел
        if (!node->children[index])
        {
            node->children[index] = getNewNode();
        }
        node = node->children[index];
    }

    // помечаем последний узлел как лист, т.е. конец слова
    node->isEndOfWord = true;
}

// Рекурсивная функция удаления ключа из дерева
TrieNode* remove(TrieNode* root, std::string key, int depth)
{
    // Если дерево пустое
    if (!root)
        return nullptr;

    // Если дошли до конца ключа
    if (depth == key.size())
    {
        // Этот узел больше не конец слова
        // поэтому снимаем метку
        if (root->isEndOfWord)
        {
            root->isEndOfWord = false;
        }

        // Если ключ не является префиксом, удаляем его
        if (isEmpty(root))
        {
            delete (root);
            root = nullptr;
        }

        return root;
    }

    // Пока не дошли до конца ключа или определили, 
    // что его нет в дереве, рекурсивно вызываем для ребенка
    // соответствующего символа
    int index = key[depth] - 'a';
    root->children[index] = remove(root->children[index], key, depth + 1);

    // Если у корня нет дочернего слова
    // (удален только один его дочерний элемент),
    // и он не заканчивается другим словом.
    if (isEmpty(root) && root->isEndOfWord == false)
    {
        delete (root);
        root = nullptr;
    }

    // возвращаем новый корень
    return root;
}

// Возвращает true если root имеет лист, иначе false 
bool isEmpty(TrieNode* root)
{
    for (int i = 0; i < ALPHABET_SIZE; i++)
    {
        if (root->children[i])
        {
            return false;
        }
    }

    return true;
}

// Возвращает true если ключ есть в дереве, иначе false 
bool search(struct TrieNode* root, std::string key)
{
    TrieNode* node = searchKey(root, key);
    return (node != nullptr && node->isEndOfWord);
}

TrieNode* searchKey(TrieNode* root, std::string key)
{
    struct TrieNode* node = root;

    for (int i = 0; i < key.length(); i++)
    {
        int index = key[i] - 'a';
        if (!node->children[index])
        {
            return nullptr;
        }
        node = node->children[index];
    }
    return node;
}

void searchAllStrings(TrieNode* root, std::string& word, std::vector<std::string>& predictiveList)
{
    if (!root)
    {
        return;
    }

    if (root->isEndOfWord)
    {
        // cout << word << "\n";
        predictiveList.push_back(word);
    }

    for (int i = 0; i < ALPHABET_SIZE; ++i)
    {
        if (root->children[i] != nullptr)
        {
            word.push_back('a' + i);
            searchAllStrings(root->children[i], word, predictiveList);
            word.pop_back();
        }
    }
}

void predictiveDictionary(TrieNode* node, std::string& key, std::vector<std::string>& predictiveList)
{
    TrieNode* root = searchKey(node, key);
    searchAllStrings(root, key, predictiveList);    
}

void findMinPrefixes(TrieNode* root, char buffer[], int index, std::string& result)
{    
    if (!root)
    {
        return;
    }    

    if (!index)
    {
        buffer[index] = 0;
    }

    int childCount = 0; // количество детей узла root
    for (int i = 0; i < ALPHABET_SIZE; ++i)
    {
        if (root->children[i] != nullptr)
        {
            ++childCount;
        }
    }       

    if (childCount > 1)
    {
        buffer[0] = -index;
    }
    else 
    {
        if (buffer[0] <= 0)
        {
            buffer[0] = index;
        }
    }

    if (root->isEndOfWord && childCount)
    {
        buffer[0] = index;
    }

    if (root->isEndOfWord)
    {
        if (!childCount)
        {
            for (int i = 1; i <= buffer[0]; i++)
            {
                result.push_back(buffer[i]);
            }
            result.push_back(' ');
            buffer[0] = 0;
        }
        else
        {
            buffer[0] = index + 1;
        }
    }    

    if (childCount == 0)
    {
        return;
    }
    
    int childRemained{ childCount };

    for (int i = 0; i < ALPHABET_SIZE; ++i)
    {
        if (root->children[i] != nullptr)
        {
            buffer[index + 1] = i + 'a';                
            findMinPrefixes(root->children[i], buffer, index + 1, result);
            childRemained--;
        }
        if (!childRemained) break;
    }
    
    buffer[index] = 0;
    buffer[0] = -(index - 1);    
}

std::string creatingMessage(TrieNode* root, int& coordX, int& coordY)
{
    int lastCoordinateX{};
    int lastCoordinateY{};
    std::string newMessage{};                         // create message
    std::string prefix{};
    std::string postfix{};   
    bool isCapitalLetter{ false };
    std::vector<std::string> arrayOfMessageWords{};    
    std::vector<std::string> predictiveList{};
    char pressKey{ '\0' };

    clearingTheInputWindow();
    gotoCoordinates(4, 24);
    std::cout << "\033[1;33;44m\033[36m\x11\033[33m previous word   \033[36m\x10"
        << "\033[33m next word   \033[36m\x1E\033[33m page up   \033[36m\x1F"
        << "\033[33m page down   \033[36mEnter\033[33m - confirm\033[37;0m";
    gotoCoordinates(0, 19);

    do
    {        
        if (pressKey == '\0')
        {
            pressKey = _getche();
        }
        
        if (pressKey == 8)    // key "Backspace"
        {
            if (!prefix.empty())
            {              
                prefix.pop_back();
                if (prefix.empty())
                {
                    postfix.clear();
                }
                std::cout << "\033[0K";
                predictiveList.clear();
                pressKey = '\0';
                lastCoordinateX = getXcoord();
                lastCoordinateY = getYcoord();
                coordX = 0;
                coordY = 0;
                gotoCoordinates(79, 17);
                std::cout << "\033[1J";
                gotoCoordinates(lastCoordinateX, lastCoordinateY);
            }
            else
            {   
                if (isContainsNonLetters(arrayOfMessageWords.back()))
                {
                    arrayOfMessageWords.pop_back();
                }
                
                if (!isContainsNonLetters(arrayOfMessageWords.back()))
                {
                    prefix = arrayOfMessageWords.back();
                    arrayOfMessageWords.pop_back();
                    if (prefix.front() >= 'A' && prefix.front() <= 'Z')
                    {
                        isCapitalLetter = true;
                        prefix.front() += 32;
                    }
                }                   

                std::cout << "\033[0K";
                pressKey = '\0';
                lastCoordinateX = getXcoord();
                lastCoordinateY = getYcoord();
                continue;
            }
        }
        else if (pressKey == 13)   // key "Enter"
        {
            if (arrayOfMessageWords.empty())
            {
                outInformationLines();
                outSelectAction();
                return "";
            }

            if (postfix.size())
            {
                gotoCoordinates(79, 17);
                std::cout << "\033[1J";
                gotoCoordinates(lastCoordinateX, lastCoordinateY); 
                if (isCapitalLetter)
                {
                    prefix.front() -= 32;
                }
                arrayOfMessageWords.push_back(prefix + postfix);
                std::cout << "\033[0K" << postfix;
                isCapitalLetter = false;
                prefix.clear();
                postfix.clear();
                pressKey = '\0';
                lastCoordinateX = getXcoord();
                lastCoordinateY = getYcoord();
                continue;
            }
            else
            {
                insert(root, prefix);

                if (isCapitalLetter)
                {
                    prefix.front() -= 32;
                }
               
                arrayOfMessageWords.push_back(prefix);
                
                gotoCoordinates(lastCoordinateX, lastCoordinateY);

                for (auto words : arrayOfMessageWords)
                {
                    newMessage += words;
                }
                
                return newMessage;
            }            
        }
        else if (pressKey >= 32 && pressKey <= 126)
        {
            if (pressKey >= 'A' && pressKey <= 'Z')
            {
                isCapitalLetter = true;
                pressKey += 32;
            }

            if (pressKey >= 'a' && pressKey <= 'z')
            {
                predictiveList.clear();
                prefix.push_back(pressKey);                
            }    
            else
            {
                if (pressKey != 32)
                {
                    if (!prefix.empty() && !isContainsNonLetters(prefix))
                    {
                        insert(root, prefix);

                        if (isCapitalLetter)
                        {
                            prefix.front() -= 32;
                        }
                        arrayOfMessageWords.push_back(prefix);

                        isCapitalLetter = false;
                        prefix.clear();
                        postfix.clear();
                        std::cout << "\033[0K";
                    }
                    
                    std::string addChar{ pressKey };
                   
                    if (prefix.empty())
                    {
                        arrayOfMessageWords.push_back(addChar);
                    }               
                }
                else
                {
                    std::cout << "\033[0K"; 
                    if (!prefix.empty())
                    {
                        insert(root, prefix);
                        if (isCapitalLetter)
                        {
                            prefix.front() -= 32;
                        }
                        arrayOfMessageWords.push_back(prefix);
                        arrayOfMessageWords.push_back(" ");
                        isCapitalLetter = false;
                        prefix.clear();
                        postfix.clear();
                    }
                    else
                    {
                        arrayOfMessageWords.push_back(" ");
                    }
                   
                    std::cout << "\033[0K";                       
                }
                
                predictiveList.clear();
            }
            pressKey = '\0';
            lastCoordinateX = getXcoord();
            lastCoordinateY = getYcoord();
        }        

        if (prefix.size() && !isContainsNonLetters(prefix))
        {
            predictiveList.clear();
            predictiveDictionary(root, prefix, predictiveList);
        }
        
        if (predictiveList.empty() && !isContainsNonLetters(prefix))
        {
            predictiveList.push_back(prefix);
        }

        if (prefix.size() && !isContainsNonLetters(prefix))
        {
            postfix = predictiveList.at(0).substr(prefix.size(), predictiveList.at(0).size() - prefix.size());
            std::cout << "\033[0K";
            std::cout << "\033[36m" << postfix << "\033[0m";

            gotoCoordinates(lastCoordinateX, lastCoordinateY); 
        }
        
        if (pressKey == -32)     // key "Esc"
        {           
            int element{ 0 };
            const size_t wordsOfTheScreen{ 85 };
            int predictPage{ 0 };
            int predictLine{ 0 };
            size_t pagesInTheDictionary{ predictiveList.size() % wordsOfTheScreen
                ? predictiveList.size() / wordsOfTheScreen + 1
                : predictiveList.size() / wordsOfTheScreen };
            int wordPositionX{};
            int wordPositionY{};

            if (!prefix.size())
            {
                pressKey = _getche();
                pressKey = '\0';
                std::cout << "\b";
                std::cout << "\033[0K";               
            }
            else
            {
                do
                {
                    if (pressKey != 13)   // key "Enter"
                    {
                        pressKey = _getche();
                    }

                    if (predictiveList.empty())
                    {
                        pressKey = '\0';
                        break;
                    }

                    switch (pressKey)
                    {
                    case 72:              // up
                        predictPage--;
                        if (predictPage < 0)
                        {
                            predictPage = pagesInTheDictionary - 1;
                        }
                        break;                        
                    case 75:              // left
                        element--;
                        if (element < 0)
                        {
                            element = predictiveList.size() - 1;
                        }
                        break;
                    case 77:              // right
                        element++;
                        if (element > predictiveList.size() - 1)
                        {
                            element = 0;
                        }
                        break;
                    case 80:              // down                        
                        predictPage++;
                        if (predictPage > pagesInTheDictionary - 1)
                        {
                            predictPage = 0;
                        }
                        break;                                          
                    default:
                        break;
                    }

                    if (pressKey == 75 || pressKey == 77 || pressKey == 72 || pressKey == 80) //  
                    {
                        postfix = predictiveList.at(element).substr(prefix.size(), predictiveList.at(element).size() - prefix.size());
                        std::cout << "\033[0K";
                        std::cout << "\033[36m" << postfix << "\033[0m";
                        
                        if (pressKey == 72 || pressKey == 80)
                        {
                            gotoCoordinates(79, 17);
                            std::cout << "\033[1J";
                            gotoCoordinates(0, 0);
                            size_t startList{ wordsOfTheScreen * predictPage };
                            size_t finishList{ wordsOfTheScreen * (predictPage + 1) > predictiveList.size()
                                ? predictiveList.size() : wordsOfTheScreen * (predictPage + 1) };
                                    
                            for (size_t wordNumber{ startList }; wordNumber < finishList; wordNumber++)
                            {
                                wordPositionX = wordNumber % 5;
                                wordPositionY = (wordNumber % wordsOfTheScreen) / 5;
                                gotoCoordinates(wordPositionX * 15, wordPositionY);
                                std::cout << predictiveList.at(wordNumber);
                                if (wordPositionX == 4 && wordPositionY == 18)
                                {
                                    wordNumber = predictiveList.size();
                                    break;
                                }
                            }                            
                        }
                        gotoCoordinates(lastCoordinateX, lastCoordinateY);
                        pressKey = _getche();
                        if (pressKey == -32)
                        {
                            std::cout << "\b";
                        }
                    }          
                } while (pressKey == -32);     // key "Esc"

                if (pressKey == '\0')
                {
                    predictiveList.clear();
                }
            }            
        }               
    } while (true);
}

void read_dict(std::string& filename, TrieNode* root)
{
    system("CLS");
    gotoCoordinates(7, 11);

    std::cout << "Do you want to connect a dictionary (y - yes, other key - no)? ";
    char action = _getche();
    if (action != 'y' && action != 'Y')
    {
        return;
    }    

    system("CLS");
    gotoCoordinates(23, 10);
    std::cout << "Default dictionary - \"dict.txt\".";
    gotoCoordinates(7, 11);
    std::cout << "Do you want to use your dictionary (y - yes, other key - no) ? ";
    action = _getche();

    if (action == 'y' || action == 'Y')
    {
        gotoCoordinates(23, 12);
        std::cout << "File name dictionary: ";
        std::cin >> filename;        
    }

    std::fstream fs;
    std::string word{};
    fs.open(filename, std::fstream::in);
    if (fs.is_open()) 
    {        
        while (!fs.eof())
        {
            getline(fs, word, ' ');
            insert(root, word);            
        }

        fs.close();
    }
}

void write_dict(std::string& filename, TrieNode* root)
{
    system("CLS");
    gotoCoordinates(8, 11);

    std::cout << "Do you want to save the dictionary (y - yes, other key - no)? ";
    char action = _getche();
    if (action != 'y' && action != 'Y')
    {
        return;
    }

    system("CLS");
    gotoCoordinates(23, 10);
    std::cout << "Default dictionary - \"" << filename << "\".";
    gotoCoordinates(1, 11);
    std::cout << "Do you want to save the dictionary to your file (y - yes, other key - no) ? ";
    action = _getche();

    if (action == 'y' || action == 'Y')
    {
        gotoCoordinates(23, 12);
        std::cout << "File name dictionary: ";
        std::cin >> filename;
    }

    std::fstream fs;    
    std::string word{};
    std::vector<std::string> dictionary{};

    fs.open(filename, std::fstream::out);
    if (fs.is_open()) 
    {
        searchAllStrings(root, word, dictionary);
        for (auto element : dictionary)
        {
            fs << element << ' ';
        }             
        fs << '\n';

        fs.close(); 
    }
}

bool isContainsNonLetters(std::string prefix)
{
    if (!prefix.size())
    {
        return true;
    }

    for (auto simbol : prefix)
    {
        if ((simbol >= 'A' && simbol <= 'Z') || (simbol >= 'a' && simbol <= 'z'))
        {
            continue;
        }
        else
        {
            return true;
        }
    }   

    return false;
}
