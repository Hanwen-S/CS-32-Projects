#include <iostream>
#include <stack>
#include <string>
#include <cassert>

using namespace std;

bool validSyntax(string infix);
bool conversion(string infix, string& postfix);
bool evaluation(const bool values[], string postfix);

int evaluate(string infix, const bool values[], string& postfix, bool& result) {
    bool valid = conversion(infix, postfix); //set postfix
    if (!valid) {     //if the syntax is invalid
        return 1;
    }
    result = evaluation(values, postfix);   //if valid, set result
    return 0;
}

bool validSyntax(string infix) {

    if (infix.size() == 0) { //if the string is empty
        return false;
    }

    string withoutSpace = "";
    for (unsigned int i = 0; i < infix.size(); i++) { //delete the spaces
        if (infix.at(i) != ' ') {
            withoutSpace += infix.at(i);
        }
    }

    if (withoutSpace.size() == 0) { //if the string contains only spaces
        return false;
    }

    if (withoutSpace.size() == 1) { //if the string contains only one char
        if (withoutSpace.at(0) > '9' || withoutSpace.at(0) < '0') {
            return false;
        }
    }

    for (unsigned int i = 0; i < withoutSpace.size(); i++) { //check all the chars
        if (withoutSpace.at(i) != '(' && withoutSpace.at(i) != ')' && withoutSpace.at(i) != '!' && withoutSpace.at(i) != '&' && withoutSpace.at(i) != '|') {
            //if neither numbers nor operators
            if (withoutSpace.at(i) > '9' || withoutSpace.at(i) < '0') {
                return false;
            }
        }
    }

    if (withoutSpace.at(0) != '(' && withoutSpace.at(0) != '!') { //check the beginning
        if (withoutSpace.at(0) > '9' || withoutSpace.at(0) < '0') {
            return false;
        }
    }

    if (withoutSpace.at(withoutSpace.size() - 1) != ')') { //check the ending
        if (withoutSpace.at(withoutSpace.size() - 1) > '9' || withoutSpace.at(withoutSpace.size() - 1) < '0') {
            return false;
        }
    }

    int numOpeningBracket(0), numClosingBracket(0); //count brackets

    if (withoutSpace.at(0) == '(') {
        numOpeningBracket++;
    }

    for (unsigned int i = 1; i < withoutSpace.size(); i++) {
        char ch = withoutSpace.at(i);
        switch (ch) {
        case ')':
            numClosingBracket++;
        case '|':
        case '&':
            if (withoutSpace.at(i - 1) != ')') {
                if (withoutSpace.at(i - 1) > '9' || withoutSpace.at(i - 1) < '0') {
                    return false;
                }
            }
            break;
        case '(':
            numOpeningBracket++;
        case '!':
            if (withoutSpace.at(i - 1) != '|' && withoutSpace.at(i - 1) != '&' && withoutSpace.at(i - 1) != '(' && withoutSpace.at(i - 1) != '!') {
                return false;
            }
            break;
        default: //numbers
            if (withoutSpace.at(i - 1) != '|' && withoutSpace.at(i - 1) != '&' && withoutSpace.at(i - 1) != '(' && withoutSpace.at(i - 1) != '!') {
                return false;
            }
            break;
        }
    }
    //parantheses check
    if (numOpeningBracket != numClosingBracket) {
        return false;
    }
    return true;
}

bool conversion(string infix, string& postfix) {
    if (!validSyntax(infix)) { //check syntax
        return false;
    }

    string withoutSpace = "";
    for (unsigned int i = 0; i < infix.size(); i++) { //delete the spaces
        if (infix.at(i) != ' ') {
            withoutSpace += infix.at(i);
        }
    }

    postfix = "";
    stack<char> charStack;

    for (unsigned int i = 0; i < withoutSpace.size(); i++) {
        char ch = withoutSpace.at(i);
        switch (ch) {
        case '(':
            charStack.push(ch);
            break;

        case ')':
            while (charStack.top() != '(') {
                postfix += charStack.top();
                charStack.pop();
            }
            charStack.pop();
            break;

        case '!':
            while (!charStack.empty() && charStack.top() != '(' && (charStack.top() == '!')) {
                postfix += charStack.top();
                charStack.pop();
            }
            charStack.push(ch);
            break;

        case '&':
            while (!charStack.empty() && charStack.top() != '(' && (charStack.top() == '!' || charStack.top() == '&')) {
                postfix += charStack.top();
                charStack.pop();
            }
            charStack.push(ch);
            break;

        case '|':
            while (!charStack.empty() && charStack.top() != '(' && (charStack.top() == '!' || charStack.top() == '&' || charStack.top() == '|')) {
                postfix += charStack.top();
                charStack.pop();
            }
            charStack.push(ch);
            break;

        default: //case of operands
            postfix += ch;
            break;
        }
    }
    while (!charStack.empty()) {
        postfix += charStack.top();
        charStack.pop();
    }

    return true;
}


bool evaluation(const bool values[], string postfix) {
    stack<bool> boolStack;

    for (unsigned int i = 0; i < postfix.size(); i++) {
        if (postfix.at(i) != '!' && postfix.at(i) != '&' && postfix.at(i) != '|') { //case of operands
            int index = postfix.at(i);
            index -= '0';
            boolStack.push(values[index]);
        }

        else if (postfix.at(i) == '!') { //case of '!'
            bool temp = boolStack.top();
            boolStack.pop();
            boolStack.push(!temp);
        }

        else {  //the rest of the cases
            bool operand_2 = boolStack.top();
            boolStack.pop();
            bool operand_1 = boolStack.top();
            boolStack.pop();

            if (postfix.at(i) == '&') { //case of '&'
                boolStack.push(operand_1 && operand_2);
            }

            else { //case of '|'
                boolStack.push(operand_1 || operand_2);
            }
        }
    }
    //return the only element (result)
    return boolStack.top();
}


int main()
{
    //bool ba[10] = {
    //    //  0      1      2      3      4      5      6      7      8      9
    //      true,  true,  true,  false, false, false, true,  false, true,  false
    //};
    //string pf;
    //bool answer;
    //assert(evaluate("2| 3", ba, pf, answer) == 0 && pf == "23|" && answer);
    //assert(evaluate("8|", ba, pf, answer) == 1);
    //assert(evaluate(" &6", ba, pf, answer) == 1);
    //assert(evaluate("4 5", ba, pf, answer) == 1);
    //assert(evaluate("01", ba, pf, answer) == 1);
    //assert(evaluate("()", ba, pf, answer) == 1);
    //assert(evaluate("()4", ba, pf, answer) == 1);
    //assert(evaluate("2(9|8)", ba, pf, answer) == 1);
    //assert(evaluate("2(&8)", ba, pf, answer) == 1);
    //assert(evaluate("(6&(7|7)", ba, pf, answer) == 1);
    //assert(evaluate("x+5", ba, pf, answer) == 1);
    //assert(evaluate("", ba, pf, answer) == 1);
    //assert(evaluate("2|3|4", ba, pf, answer) == 0
    //    && pf == "23|4|" && answer);
    //assert(evaluate("2|(3|4)", ba, pf, answer) == 0
    //    && pf == "234||" && answer);
    //assert(evaluate("4  |  !3 & (0&3) ", ba, pf, answer) == 0
    //    && pf == "43!03&&|" && !answer);
    //assert(evaluate(" 9  ", ba, pf, answer) == 0 && pf == "9" && !answer);
    //ba[2] = false;
    //ba[9] = true;
    //assert(evaluate("((9))", ba, pf, answer) == 0 && pf == "9" && answer);
    //assert(evaluate("2| 3", ba, pf, answer) == 0 && pf == "23|" && !answer);

    return 0;
}