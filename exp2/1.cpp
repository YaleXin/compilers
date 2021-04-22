/*
 * @Author      : YaleXin
 * @Email       : 181303209@yzu.edu.cn
 * @LastEditors : YaleXin
 */
#include <algorithm>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <list>
#include <queue>
#include <vector>

#include "../lex.v2.cpp"
#include "../tempStructs.h"
using namespace std;

vector<Result> ansSet;
bool status, er;
Lex lex("D:\\my_cpp_workspace\\compilers\\exp2\\test.cp", status);
int nowIndex;
vector<Result> resultLink;
vector<int> output;
const int EOF_ID = -2021;
const int ADD_ID = 62;
const int SUB_ID = 63;
const int MUL_ID = 64;
const int DIV_ID = 65;
const int IDENTIFY_ID = 32;
const int LEFT_ID = 39;
const int RIGHT_ID = 40;
const int INTEGER_CONSTANTS_ID = 33;
const int REAL_CONSTANTS_ID = 34;

int lineNum, colNum;
void error(string msg);
void printOut() {
    for (int i = 0; i < output.size(); i++) {
        cout << output[i] << ends;
    }
    cout << endl;
}
int test();
bool expr();
bool e_1();
bool e_2();
bool temp1();
bool temp2();
int main(int argc, char const *argv[]) {
    while (1) {
        Result r = lex.getWord(lineNum, colNum);
        if (r.identifyId == -1) {
            return 0;
        } else if (r.identifyId == EOF_ID) {
            break;
        } else {
            resultLink.push_back(r);
        }
    }
    lineNum = 0;
    if (!expr() || er) cout << "expression anlalyz fail!" << endl;
    else if (nowIndex < resultLink.size()){
        cout << "expression anlalyz fail!" << endl;
    } else {
        cout << "expression anlalyz success!" << endl;
    }
    return 0;
}
// 测试 词法分析器是否能够正常运行
int test() {
    ios::sync_with_stdio(false);
    string filename = "D:\\my_cpp_workspace\\compilers\\exp2\\test.cp";
    int l, c;
    while (1) {
        Result r = lex.getWord(l, c);
        if (r.identifyId == EOF_ID || r.identifyId == -1)
            break;
        else
            printf("<\t%-20s,%02d,%02d\t>\n", r.word.c_str(), r.identifyId,
                   r.internalCode);
    }
    return 0;
}
bool expr() {
    output.push_back(1);
    cout << "Expr -> Temp1 E_1" << endl;
    cout << "the output is : " << ends;
    printOut();
    if (temp1()) return e_1();
    return false;
}
bool e_1() {
    int thisInex = nowIndex, thisSize = output.size();
    bool ok = false;
    if (nowIndex == resultLink.size()) {
        cout << "E_1 -> null" << endl;
        output.push_back(4);
        printOut();
        return true;
    }
    Result word = resultLink[nowIndex];
    if (word.identifyId == ADD_ID) {
        nowIndex++;
        cout << "E_1 -> + Temp1 E_1" << endl;
        output.push_back(2);
        cout << "the output is : " << ends;
        printOut();
        ok = temp1();
        if (ok)
            ok = e_1();
        else {
            error("miss (Expr) or id or number");
            er = true;
            return false;
        }
    } else if (word.identifyId == SUB_ID) {
        nowIndex++;
        cout << "E_1 -> - Temp1 E_1" << endl;
        output.push_back(3);
        cout << "the output is : " << ends;
        printOut();
        ok = temp1();
        if (ok)
            ok = e_1();
        else {
            error("miss (Expr) or id or number");
            er = true;
            return false;
        }
    }
    // 回溯条件是没有出错，但是之前选错了产生式
    if (!ok && !er) {
        while (output.size() > thisSize) output.pop_back();
        cout << "rollback!!!" << endl;
        cout << "E_1 -> null" << endl;
        output.push_back(4);
        printOut();
        nowIndex = thisInex;
    }
    return !er;
}
bool e_2() {
    int thisInex = nowIndex, thisSize = output.size();
    bool ok = false;
    if (nowIndex == resultLink.size()) {
        cout << "E_2 -> null" << endl;
        output.push_back(8);
        printOut();
        return true;
    }
    Result word = resultLink[nowIndex];
    if (word.identifyId == MUL_ID) {
        nowIndex++;
        cout << "E_2 -> * Temp2 E_2" << endl;
        output.push_back(6);
        cout << "the output is : " << ends;
        printOut();
        ok = temp2();
        if (ok)
            ok = e_2();
        else {
            error("miss (Expr) or id or number");
            er = true;
            return false;
        }
    } else if (word.identifyId == DIV_ID) {
        nowIndex++;
        cout << "E_2 -> / Temp2 E_2" << endl;
        output.push_back(7);
        cout << "the output is : " << ends;
        printOut();
        ok = temp2();
        if (ok)
            ok = e_2();
        else {
            error("miss (Expr) or id or number");
            er = true;
            return false;
        }
    }
    // 回溯条件是没有出错，但是之前选错了产生式
    if (!ok && !er) {
        while (output.size() > thisSize) output.pop_back();
        cout << "rollback!!!" << endl;
        cout << "E_2 -> null" << endl;
        output.push_back(8);
        printOut();
        nowIndex = thisInex;
    }
    return !er;
}
bool temp1() {
    cout << "Temp1 -> Temp2 E_2" << endl;
    output.push_back(5);
    cout << "the output is : " << ends;
    printOut();
    if (temp2()) return e_2();
    return false;
}
bool temp2() {
    int thisIndex = nowIndex;
    if (nowIndex == resultLink.size()) {
        error("miss (Expr) or id or number");
        er = true;
        return false;
    }
    Result word = resultLink[nowIndex];
    int wordId = word.identifyId;
    if (wordId == LEFT_ID) {
        cout << "Temp2 -> ( Expr )" << endl;
        cout << "the output is : " << ends;
        output.push_back(9);
        printOut();
        nowIndex++;
        if (expr()) {
            if (nowIndex == resultLink.size()) {
                error("expect \")\"");
                er = true;
                return false;
            }
            word = resultLink[nowIndex];
            if (word.identifyId == RIGHT_ID){
                 nowIndex++;
                 return true;
            }
        } else {
            er = true;
            error("expect \")\"");
        }
    } else if (wordId == IDENTIFY_ID) {
        cout << "Temp2 -> id" << endl;
        cout << "the output is : " << ends;
        output.push_back(10);
        printOut();
        nowIndex++;
        return true;
    } else if (wordId == REAL_CONSTANTS_ID || wordId == INTEGER_CONSTANTS_ID) {
        cout << "Temp2 -> number" << endl;
        cout << "the output is : " << ends;
        output.push_back(11);
        printOut();

        nowIndex++;
        return true;
    }
    return false;
}

void error(string errorMsg) {
    cout << "expression analyze error!, because is : " << errorMsg
         << "in row: " << (lineNum + 1) << ", column: " << (colNum + 1) << endl;
}