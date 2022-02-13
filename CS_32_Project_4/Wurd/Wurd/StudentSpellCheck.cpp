#include "StudentSpellCheck.h"
#include <string>
#include <vector>

using namespace std;

SpellCheck* createSpellCheck()
{
    return new StudentSpellCheck;
}

StudentSpellCheck::StudentSpellCheck() {
    //initialize the data members
    m_root = new TrieNode;
    for (int i = 0; i < 28; i++) {
        m_root->m_children[i] = nullptr;
    }
}

StudentSpellCheck::~StudentSpellCheck() {
    //call the cleanup function
    TrieNode* p = m_root;
    cleanUp(p);
}

void StudentSpellCheck::cleanUp(TrieNode* p) {
    //use recursion to delete every child in the trie
    if (p == nullptr) {
        return;
    }
    for (int i = 0; i < 28; i++) {
        cleanUp(p->m_children[i]);
    }
    delete p;
}

bool StudentSpellCheck::load(std::string dictionaryFile) {
    //input file
    ifstream infile(dictionaryFile);

    if (!infile) {
        return false;
    }

    string curline;
    //loop to put every word in the dictionary into the trie
    while (getline(infile, curline)) {
        for (int i = 0; i < curline.size(); i++) {
            if (curline.at(i) != '\'' && !isalpha(curline.at(i))) {
                //if a char in the line is not a ''' or a letter
                curline.erase(i);
                break;
            }
            //convert every word to lower case
            curline.at(i) = tolower(curline.at(i));
        }

        //adding characters to the trie
        TrieNode* p = m_root;
        bool included = true;
        int i = 0;
        while (i < curline.size()) {
            TrieNode* temp = p;
            if (curline.at(i) == '\'') {
                p = p->m_children[1];
            }
            else { //if the char is a letter, store it in an a-z order
                p = p->m_children[(curline.at(i) - 'a') + 2];
            }
            if (p == nullptr) {
                p = temp;
                included = false; //tell the trie there is no such letter combination
                break;
            }
            i++;
        }

        //if has characters that are not included
        if (!included) {
            for (int j = i; j < curline.size(); j++) {
                if (curline.at(j) == '\'') {
                    //set values and nullptrs
                    p->m_children[1] = new TrieNode;
                    p->m_children[1]->value = '\'';
                    for (int k = 0; k < 28; k++) {
                        p->m_children[1]->m_children[k] = nullptr;
                    }
                    //update p's position
                    p = p->m_children[1];
                }
                else {
                    p->m_children[curline.at(j) - 'a' + 2] = new TrieNode;
                    p->m_children[curline.at(j) - 'a' + 2]->value = curline.at(j);
                    for (int k = 0; k < 28; k++) {
                        p->m_children[curline.at(j) - 'a' + 2]->m_children[k] = nullptr;
                    }
                    p = p->m_children[curline.at(j) - 'a' + 2];
                }
            }
        }

        //add . to the end
        p->m_children[0] = new TrieNode;
        p->m_children[0]->value = '.';
        for (int k = 0; k < 28; k++) {
            p->m_children[0]->m_children[k] = nullptr;
        }
    }
    return true;
}

bool StudentSpellCheck::spellCheck(std::string word, int max_suggestions, std::vector<std::string>& suggestions) {

    //convert every word to lower case
    for (int i = 0; i < word.size(); i++) {
        word[i] = tolower(word[i]);
    }

    int flag = 0;
    TrieNode* p = m_root;
    //loop through to look for words
    for (int i = 0; i < word.size(); i++) {
        if (word[i] == '\'') {
            p = p->m_children[1];
        }
        else {
            p = p->m_children[tolower(word[i] - 'a' + 2)];
        }
        //not a word in dic
        if (p == nullptr) {
            //clear suggestion vector
            suggestions.clear();
            flag = -1;
            break;
        }
    }

    //see if ends with '.'
    if (p != nullptr) {
        if (p->m_children[0] != nullptr && flag != -1) {
            return true;
        }

        //not end with '.'
        else if (p->m_children[0] == nullptr && flag != -1) {
            suggestions.clear();
        }
    }
    //loop to get all words that are one char different from the original word 
    //and check if the word is in the dictionary
    for (unsigned int i = word.size() - 1; suggestions.size() < max_suggestions && i > 0; i--)
    {
        string suggestion = word;
        for (char j = 'a'; suggestions.size() < max_suggestions && j <= 'z'; j++)
        {
            //modify one character 
            suggestion.at(i) = j;
            if (checkDict(suggestion)) {
                //if yes, push it into suggestions
                suggestions.push_back(suggestion);
            }
        }
    }

    return false;
}

bool StudentSpellCheck::checkDict(const std::string s)
{
    TrieNode* p = m_root;
    for (int j = 0; j < s.size(); j++) {
        if (s.at(j) == '\'') {
            p = p->m_children[1];
        }
        else {
            if (p != nullptr) {
                p = p->m_children[tolower(s[j]) - 'a' + 2];
            }
        }
        //not in the dic
        if (p == nullptr) {
            return false;
        }
    }
    //just part of a word
    if (p != nullptr) {
        if (p->m_children[0] == nullptr) {
            return false;
        }
    }

    //else return true
    return true;
}

void StudentSpellCheck::spellCheckLine(const std::string& line, std::vector<SpellCheck::Position>& problems) {
    problems.clear();
    //slice into words
    vector<SpellCheck::Position> vec;
    int startPos = 0;
    int endPos = 0;
    int count = 0;
    int flag = -1;
    for (int i = 0; i < line.size(); i++) {
        //if not letters or '\''
        if (line.at(i) != '\'' && !isalpha(line.at(i))) {
            //get the pos of each word and push to the vector
            if (i != 0 && flag != -1) {
                endPos = i - 1;
                startPos = endPos - count + 1;
                SpellCheck::Position pos;
                pos.start = startPos;
                pos.end = endPos;
                vec.push_back(pos);
            }
            count = 0;
            flag = -1;
        }
        //end of line
        else if (i == line.size() - 1 && (isalpha(line.at(i)) || line.at(i) == '\'')) {
            endPos = i;
            startPos = endPos - count;
            SpellCheck::Position pos;
            pos.start = startPos;
            pos.end = endPos;
            vec.push_back(pos);
        }
        //if it's letter or '\''
        else if (isalpha(line.at(i)) || line.at(i) == '\'') {
            flag = 0;
            count++;
        }
    }

    //loop through to look for words in dic
    for (int i = 0; i < vec.size(); i++) {
        if (!checkDict(line.substr(vec[i].start, vec[i].end - vec[i].start + 1))) {
            problems.push_back(vec[i]);
        }
    }
}
