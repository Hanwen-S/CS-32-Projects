#include "StudentUndo.h"
#include <algorithm>

using namespace std;

Undo* createUndo()
{
    return new StudentUndo;
}

void StudentUndo::submit(const Action action, int row, int col, char ch) {
    //create a new Act and put values in
    Act a;
    a.m_act = action;
    a.m_value = ch;
    a.m_row = row;
    a.m_col = col;
    a.m_count = 1;
    a.delTogether = false;
    a.isDelete = false;
    //if not empty and the act equals to the previous act
    if (!ActStack.empty()) {
        //if same row and insert right to the previous or it's a tab
        if (a.m_act == INSERT && ActStack.top().m_act == INSERT && (a.m_col == ActStack.top().m_col + 1 || a.m_value == '\t')) {
            a.m_count = ActStack.top().m_count + 1;
        }
        //if delete together, not separately
        else if (a.m_act == DELETE && ActStack.top().m_act == DELETE && (a.m_col == ActStack.top().m_col - 1 || a.m_col == ActStack.top().m_col) && a.m_row == ActStack.top().m_row) {
            //if it's deleted not backspaced
            if (a.m_col == ActStack.top().m_col) {
                a.isDelete = true;
                ActStack.top().isDelete = true;
            }
            a.delTogether = true;
            ActStack.top().delTogether = true;
        }
    }
    //if it's a tab
    if (a.m_value == '\t') {
        a.m_count += 3;
    }
    //save in our stack
    ActStack.push(a);
}

StudentUndo::Action StudentUndo::get(int& row, int& col, int& count, std::string& text) {
    //if there's nothing to undo
    if (ActStack.empty()) {
        return ERROR;
    }

    //if there's something to undo
    Act a = ActStack.top();
    count = a.m_count;
    //check to see the first undo object
    string temp = "";
    switch (a.m_act) {
    case INSERT: 
        row = a.m_row;
        col = a.m_col;
        while (ActStack.top().m_act == INSERT) {
            ActStack.pop();
            if (ActStack.empty()) {
                break;
            }
        }
        text = "";
        return DELETE;
        break;

    case DELETE: 
        text = "";
        temp = "";
        row = ActStack.top().m_row;
        col = ActStack.top().m_col;
        //if just deleted together
        if (ActStack.top().delTogether) {
            while (ActStack.top().delTogether) {
                //reverse the string if it's deleted not backspaced
                if (ActStack.top().isDelete) {
                    while (ActStack.top().isDelete) {
                        temp += ActStack.top().m_value;
                        ActStack.pop();
                    }
                    reverse(temp.begin(), temp.end());
                    text += temp;
                }
                //if still delete but from backspace
                if (ActStack.top().delTogether) {
                    text += ActStack.top().m_value;
                    ActStack.pop();
                }
            }
        }
        else {
            text += ActStack.top().m_value;
            ActStack.pop();
        }
        count = 1;
        return INSERT;
        break;
    
    case SPLIT: 
        row = a.m_row;
        col = a.m_col;
        count = 1;
        text = "";
        ActStack.pop();
        return JOIN;
        break;

    case JOIN: 
        row = a.m_row;
        col = a.m_col;
        count = 1;
        text = "";
        ActStack.pop();
        return SPLIT;
        break;
    default:
        return ERROR;
    }
}

void StudentUndo::clear() {
    //delete all items
    while (!ActStack.empty()) {
        ActStack.pop();
    }
}
