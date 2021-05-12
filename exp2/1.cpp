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
int nowIndex, cnt = 1;
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
    // cout << endl;
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
    printf("%-4s %-5s %-10s %-50s\n", "步骤", "读头", "动作", "输出带");
    printf("%-4s\n", "初态");
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
void print(string action){
    printf("%-4d ", cnt++);
    printf("%-5s ", nowIndex < resultLink.size() ? resultLink[nowIndex].word.c_str() : "");
    printf("%-10s ", action.c_str());
    printOut();
    printf("\n");
}
bool expr() {
    output.push_back(1);
    // cout << "Expr -> Temp1 E_1" << endl;
    print("推导");
    if (temp1()) return e_1();
    return false;
}
bool e_1() {
    int thisInex = nowIndex, thisSize = output.size();
    bool ok = false;
    if (nowIndex == resultLink.size()) {
        // cout << "E_1 -> null" << endl;
        output.push_back(4);
        print("推导空串");
        return true;
    }
    Result word = resultLink[nowIndex];
    if (word.identifyId == ADD_ID) {
        // cout << "E_1 -> + Temp1 E_1" << endl;
        output.push_back(2);
        print("匹配+号");
        nowIndex++;
        ok = temp1();
        if (ok)
            ok = e_1();
        else {
            error("miss (Expr) or id or number");
            er = true;
            return false;
        }
    } else if (word.identifyId == SUB_ID) {
        // cout << "E_1 -> - Temp1 E_1" << endl;
        output.push_back(3);
        print("匹配-号");
        nowIndex++;
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
        // cout << "rollback!!!" << endl;
        // cout << "E_1 -> null" << endl;
        output.push_back(4);
        print("回溯");
        nowIndex = thisInex;
    }
    return !er;
}
bool e_2() {
    int thisInex = nowIndex, thisSize = output.size();
    bool ok = false;
    if (nowIndex == resultLink.size()) {
        // cout << "E_2 -> null" << endl;
        output.push_back(8);
        print("推导空串");
        return true;
    }
    Result word = resultLink[nowIndex];
    if (word.identifyId == MUL_ID) {
        // cout << "E_2 -> * Temp2 E_2" << endl;
        output.push_back(6);
        print("匹配*号");
        nowIndex++;
        ok = temp2();
        if (ok)
            ok = e_2();
        else {
            error("miss (Expr) or id or number");
            er = true;
            return false;
        }
    } else if (word.identifyId == DIV_ID) {
        // cout << "E_2 -> / Temp2 E_2" << endl;
        output.push_back(7);
        print("匹配/号");
        nowIndex++;
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
        // cout << "rollback!!!" << endl;
        // cout << "E_2 -> null" << endl;
        output.push_back(8);
        print("回溯");
        nowIndex = thisInex;
    }
    return !er;
}
bool temp1() {
    // cout << "Temp1 -> Temp2 E_2" << endl;
    output.push_back(5);
    print("推导");
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
        // cout << "Temp2 -> ( Expr )" << endl;
        // cout << "the output is : " << ends;
        output.push_back(9);
        print("匹配(号");
        nowIndex++;
        if (expr()) {
            if (nowIndex == resultLink.size()) {
                error("expect \")\"");
                er = true;
                return false;
            }
            word = resultLink[nowIndex];
            if (word.identifyId == RIGHT_ID){
                print("匹配)号");
                 nowIndex++;
                 return true;
            }
        } else {
            er = true;
            error("expect \")\"");
        }
    } else if (wordId == IDENTIFY_ID) {
        // cout << "Temp2 -> id" << endl;
        // cout << "the output is : " << ends;
        output.push_back(10);
        print("匹配变量");
        nowIndex++;
        return true;
    } else if (wordId == REAL_CONSTANTS_ID || wordId == INTEGER_CONSTANTS_ID) {
        // cout << "Temp2 -> number" << endl;
        // cout << "the output is : " << ends;
        output.push_back(11);
        print("匹配数字");
        nowIndex++;
        return true;
    }
    return false;
}

void error(string errorMsg) {
    cout << "expression analyze error!, because is : " << errorMsg
         << "in row: " << (lineNum + 1) << ", column: " << (colNum + 1) << endl;
}
/*

ab + ab * bc - (cd + 1) * a
1 5 10 8 2 5 10 6 10 8 3 5 9 1 5 10 8 2 5 11 8 4 6 10 8 4
1 5 10 8 2 5 10 6 10 8 3 5 9 1 5 10 8 2 5 11 8 4 6 10 8 4
*/