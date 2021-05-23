/*
 * @Author      : YaleXin
 * @Email       : 181303209@yzu.edu.cn
 * @LastEditors : YaleXin
 */
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <queue>
#include <stack>
#include <vector>

#include "../lex.v2.cpp"
#include "../tempStructs.h"
using namespace std;

const int END = -2021, ADD = 62, SUB = 63;
const int MUL = 64, DIV = 65, IDENTIFY = 32;
const int LEFT = 39, RIGHT = 40, INT_CONSTANTS = 33, DBL_CONSTANTS = 34;
const int REAL_CONSTANTS = 34, EQL = 53;
const int ACC = 0, E_ID = 101;
const int INT_LEN = 500, DBL_LEN = 500, NML_LEN = 9000, TMP_LEN = 90000;
typedef Quaternion Quat;
vector<Quat>quats;
bool status;
Lex lex("D:\\my_cpp_workspace\\compilers\\exp4\\test.cp", status);
Result nowWord("", -1), copyWord("", -1), lastWord("", -1);
int action[][9] = {{3, -1, -1, -1, -1, 2, -1, -1, 1},
                   {-1, 4, 5, 6, 7, -1, -1, -2, -1},
                   {3, -1, -1, -1, -1, 2, -1, -1, 8},
                   {-1, 106, 106, 106, 106, -1, 106, 106, -1},
                   {3, -1, -1, -1, -1, 2, -1, -1, 9},
                   {3, -1, -1, -1, -1, 2, -1, -1, 10},
                   {3, -1, -1, -1, -1, 2, -1, -1, 11},
                   {3, -1, -1, -1, -1, 2, -1, -1, 12},
                   {-1, 4, 5, 6, 7, -1, 13, -1, -1},
                   {-1, 101, 101, 6, 7, -1, 101, 101, -1},
                   {-1, 102, 102, 7, 7, -1, 102, 102, -1},
                   {-1, 103, 103, 103, 103, -1, 103, 103, -1},
                   {-1, 104, 104, 104, 104, -1, 104, 104, -1},
                   {-1, 105, 105, 105, 105, -1, 105, 105, -1}
};
// 每个终结符在action表中对应的下标
map<int, int> vtMap = {{IDENTIFY, 0}, {ADD, 1}, {SUB, 2}, {MUL, 3}, {DIV, 4}, {LEFT, 5}, {RIGHT, 6}, {END, 7}, {E_ID, 8}};
vector<int>program[10] = {{IDENTIFY, ADD, IDENTIFY}};
string index2vt[] = {"i", "+", "-", "*", "/", "(", ")", "#", "E"};
map<int, string>int2op = {{101, "+"}, {102, "-"}, {103, "*"}, {104, "/"}};
int l, c;
// 符号栈  状态栈 PLACE栈 
vector<int>flagStk, stateStk, placeStk;
vector<vector<string>>asmTab;
bool S();
bool E();
int newTemp();
int entry(Result,int);
Quat gen(int, int, int, int);
void print(string, string);
void printStack();
void printQuat();
void printAsm();
int main(int argc, char const *argv[]){
    printf("%-4s %-5s %-10s %-30s %-30s %-50s\n", "步骤", "读头", "动作", "状态栈", "符号栈", "PLACE栈");
    nowWord = lex.getWord(l, c);
    if(!S()){
        printf("error");
    } else{
        printf("\n---------------\n");
        printQuat();
        printf("\n---------------\n");
        printAsm();
    }
    return 0;
}

bool S(){
    if (nowWord.identifyId == IDENTIFY){
        nowWord = lex.getWord(l, c);
        if (nowWord.identifyId == EQL){
            nowWord = lex.getWord(l, c);
            copyWord = nowWord;
            return E();
        } else{
            return false;
        }
    }else{
        return false;
    }
}
int getTable(int stateTop,const Result &nowWord){
    return action[stateTop][vtMap[nowWord.identifyId]];
}
// 处理加减乘除
bool handleCal(int type) {
    int tIndex = newTemp();
    tIndex = entry(nowWord, tIndex);
    int E1_PLACE = *(placeStk.end() - 3), E2_PLACE = *(placeStk.end() - 1);
    Quat q = gen(type, E1_PLACE, E2_PLACE, tIndex);
    quats.push_back(q);
    for (int i = 1; i <= 3; i++) stateStk.pop_back(), flagStk.pop_back(), placeStk.pop_back();
    flagStk.push_back(E_ID);
    int stateTop = *(stateStk.end() - 1);
    int value = action[stateTop][8];
    if (value == -1){
        return false;
    }
    stateStk.push_back(value);
    placeStk.push_back(tIndex);
    return true;
}
bool E(){
    // 状态
    int state = 0;
    stateStk.push_back(0), flagStk.push_back(END), placeStk.push_back(-1);
    print(nowWord.word, "");
    if (nowWord.identifyId == IDENTIFY || nowWord.identifyId == INT_CONSTANTS)
        nowWord.identifyId = IDENTIFY;
    bool acc = false, er = false;
    int stateTop, flagTOP, value;
    while(!acc && !er){
        stateTop = *(stateStk.end() - 1);
        // 查表
        value = action[stateTop][vtMap[nowWord.identifyId]];
        if (value == -1){ 
            er = true;
            continue;    
        }
        else if (value == -2)acc = true;
        // 移进
        else if (value >= 0 && value < 100){
            // printf("移进转%d\n", value);
            stateStk.push_back(value), flagStk.push_back(nowWord.identifyId);
            int index = entry(copyWord, -1);
            placeStk.push_back(index);
            print(nowWord.word, "移进");
            nowWord = lex.getWord(l, c);
            lastWord = copyWord;
            copyWord = nowWord;
            if (nowWord.identifyId == IDENTIFY || nowWord.identifyId == INT_CONSTANTS)
                nowWord.identifyId = IDENTIFY;
        }
        if (value >= 101 && value <= 200){
            // printf("按照 %d 规约\n", value);
            switch(value){
                case 101: 
                case 102: 
                case 103: 
                case 104: 
                    if (!handleCal(value)){
                        er = true;
                        break;
                    }
                    print(nowWord.word, "规约");
                    break;
                // E -> (E)
                case 105:{
                    int E_PLACE = *(placeStk.end() - 2);
                    for (int i = 1; i <= 3; i++) stateStk.pop_back(), flagStk.pop_back(), placeStk.pop_back();
                    flagStk.push_back(E_ID);
                    stateTop = *(stateStk.end() - 1);
                    value = action[stateTop][8];
                    if (value == -1) {
                        er = true;
                        break;
                    }
                    placeStk.push_back(E_PLACE);
                    stateStk.push_back(value);
                    break;
                // E -> i
                }case 106:
                    stateStk.pop_back(), flagStk.pop_back(), placeStk.pop_back();
                    flagStk.push_back(E_ID);
                    stateTop = *(stateStk.end() - 1);
                    value = action[stateTop][8];
                    if (value == -1) {
                        er = true;
                        break;
                    }
                    stateStk.push_back(value);
                    int index = entry(lastWord, -1);
                    placeStk.push_back(index);
                    print(nowWord.word, "规约");
                    break;
            }
        }
    }
    if (acc && !er)return true;
    return false;
}

int sum;
void print(string nowStr, string action) {
    printf("%-4d ", sum++);
    printf("%-5s ", nowStr.c_str());
    printf("%-10s ", action.c_str());
    printStack();
    printf("\n");
}
void printStack(){
    int len = stateStk.size();
    for (int i = 0; i < len; i++) printf("%-2d ", stateStk[i]);
    for (int i = len; i <= 10; i++) printf("%-2s ", " ");
    len = flagStk.size();
    for (int i = 0; i < len; i++)
        printf("%-2s ", index2vt[vtMap[flagStk[i]]].c_str());
    for (int i = len; i <= 9; i++) printf("%-2s ", " ");
    len = placeStk.size();
    for(int i = 0; i < len; i++)printf("%-5d ", placeStk[i]);
}
int cnt = 0;
vector<int>tempTab;
int newTemp(){ return cnt++; }
/**
 * @word: 待查询变量
 * @tempIndex: 临时变量下标
 * @return: 0     - 499：  整型常量
 *          500   - 999：  实型常量
 *          1000  - 9999： 普通变量
 *          10000 - 99999：临时变量
 */ 
int entry(Result word, int tempIndex){
    if (tempIndex != -1) return tempIndex + INT_LEN + DBL_LEN + NML_LEN;
    // 只要是常量，词法分析的结果都带上内码
    else if (word.identifyId == INT_CONSTANTS)return word.internalCode % INT_LEN;
    else if(word.identifyId == DBL_CONSTANTS) return word.internalCode % DBL_LEN + INT_LEN;
    else if (word.identifyId == IDENTIFY) return word.internalCode % NML_LEN + INT_LEN + DBL_LEN;
    return -1;
}
Quat gen(int op, int e1, int e2, int t){
    Quat q(op, e1, e2, t);
    return q;
}
// 遍历打印四元式的同时转汇编代码
void printQuat(){
    vector<string> idtfTab =  lex.getIdtfTab();
    vector<int>intTab = lex.getIntTab();
    vector<double>dblTab = lex.getDblTab();
    int len = quats.size();
    for (int i = 0; i < len; i++){
        Quat q = quats[i];
        if (q.op >= 101 && q.op <= 104){
            string e1Str, e2Str, tStr;
            // arg1 记录的是入口地址信息
            // 整型常量
            if (q.arg1 < INT_LEN)e1Str = to_string(intTab[q.arg1]);
            else if (q.arg1 >= INT_LEN && q.arg1 < INT_LEN + DBL_LEN)e1Str = to_string(dblTab[q.arg1 - INT_LEN]);
            // 普通变量
            else if (q.arg1 >= INT_LEN + DBL_LEN && q.arg1 < INT_LEN + DBL_LEN + NML_LEN)
                e1Str = idtfTab[q.arg1 - INT_LEN - DBL_LEN];
            // 临时变量
            else e1Str = "T" + to_string(q.arg1 - 10000);

            if (q.arg2 < INT_LEN)e2Str = to_string(intTab[q.arg2]);
            else if (q.arg2 >= INT_LEN && q.arg2 < INT_LEN + DBL_LEN)e2Str = to_string(dblTab[q.arg2 - INT_LEN]);
            else if (q.arg2 >= INT_LEN + DBL_LEN && q.arg2 < INT_LEN + DBL_LEN + NML_LEN)
                e2Str = idtfTab[q.arg2 - INT_LEN - DBL_LEN];
            else e2Str = "T" + to_string(q.arg2 - 10000);

            if (q.t < INT_LEN)tStr = to_string(intTab[q.t]);
            else if (q.t >= INT_LEN && q.t < INT_LEN + DBL_LEN)tStr = to_string(dblTab[q.t - INT_LEN]);
            else if (q.t >= INT_LEN + DBL_LEN && q.t < INT_LEN + DBL_LEN + NML_LEN)
                tStr = idtfTab[q.t - INT_LEN - DBL_LEN];
            else tStr = "T" + to_string(q.t - 10000);
            printf("(%-2s, %-5s, %-5s, %-5s)\n", int2op[q.op].c_str(), e1Str.c_str(), e2Str.c_str(), tStr.c_str());
            vector<string> item;
            switch(q.op){
                case 101:
                    item = {"MOV", "AX", e1Str};
                    asmTab.push_back(item);
                    item = {"ADD", "AX", e2Str};
                    asmTab.push_back(item);
                    item = {"MOV", tStr, "AX"};
                    asmTab.push_back(item);
                    break;
                case 102:
                    item = {"MOV", "AX", e1Str};
                    asmTab.push_back(item);
                    item = {"SUB", "AX", e2Str};
                    asmTab.push_back(item);
                    item = {"MOV", tStr, "AX"};
                    asmTab.push_back(item);
                    break;
                case 103:
                    item = {"MOV", "AL", e1Str};
                    asmTab.push_back(item);
                    item = {"MUL",  e2Str};
                    asmTab.push_back(item);
                    item = {"MOV", tStr, "AX"};
                    asmTab.push_back(item);
                    break;
                case 104:
                    item = {"MOV", "AX", e1Str};
                    asmTab.push_back(item);
                    item = {"DIV",  e2Str};
                    asmTab.push_back(item);
                    item = {"MOV", tStr, "AL"};
                    asmTab.push_back(item);
                    break;
            }
        } else{
            printf("未知op!\n");
        }
    }
}
void printAsm(){
    int len = asmTab.size(), itemLen;
    for(int i = 0; i < len; i++){
        itemLen = asmTab[i].size();
        for (int j = 0; j < itemLen; j++){
            printf("%-5s ", asmTab[i][j].c_str());
        }
        printf("\n");
    }
}