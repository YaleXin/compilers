/*
 * @Author      : YaleXin
 * @Email       : 181303209@yzu.edu.cn
 * @LastEditors : YaleXin
 */
#include <algorithm>
#include <cstdio>
#include <vector>
#include <map>
#include <iostream>
#include "../constant.h"
#include "../tempStructs.h"
#include "../lex.v2.cpp"
using namespace std;
Result nowWord("", -1), copyWord("", -1), lastWord("", -1);
// 符号栈  状态栈 PLACE栈 TC栈 FC栈
vector<int>flagStk, stateStk, placeStk, TC_Stk, FC_Stk;
int line, col, NXQ = 0;
bool status;
typedef Quaternion Quat;
vector<Quat>quats;
Lex lex("D:\\my_cpp_workspace\\compilers\\exp4\\test.cp", status);
int cnt = 0;
// 定义变量语句
void variableDefinitions(){
    // 实际上严格的编译器，在定义变量的时候是要完成好多事情的，但是为了简化工作，
    // 这里只需要完成查询变量的符号地址，填写进表格中，然而此项工作我放在了词法分析器中
    while (nowWord.identifyId != SEMIC){ nowWord = lex.getWord(line, col); }
}
// if (Bool) {} else {}
bool ifStmt(){
    nowWord = lex.getWord(line, col);
    if (nowWord.identifyId == LEFT){
        nowWord = lex.getWord(line, col);
        Bool();
    }
}
int MERG(int p1, int p2){
    int head = p1;
    if (p2 == 0)return head;
    int p = p2;
    while (quats[p].t != 0){
        p = quats[p].t;
    }
    quats[p].t = p1;
    head = p2;
    return head;
}
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

using namespace std;
// 算术表达式
bool Expr(){}
// 布尔表达式
bool Bool(){
    map<int, int> boolVtMap = {{RIGHT, 0},  {IDENTIFY, 1}, {LES, 2},  {GRT, 3},
                           {AND, 4},    {OR, 5},       {S_ID, 6}, {B_ID, 7},
                           {B_0_ID, 8}, {B_1_ID, 9}};
    // 此处不应该使用 # ，因为实际上应该使用 )
    // 作为结束标志（简单起见，布尔表达式中没有左右括号）
    stateStk.push_back(0), flagStk.push_back(RIGHT), placeStk.push_back(-1), TC_Stk.push_back(-1), FC_Stk.push_back(-1);
    if (nowWord.identifyId == IDENTIFY || nowWord.identifyId == INT_CONSTANTS)
        nowWord.identifyId = IDENTIFY;
    bool acc = false, er = false;
    int stateTop, flagTOP, value, topTC, topFC, newFc, newTc, varIndex;
    while(!acc && !er){
        stateTop = *(stateStk.end() - 1);
        // 查表
        value = boolLRTab[stateTop][boolVtMap[nowWord.identifyId]];
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
            nowWord = lex.getWord(line, col);
            lastWord = copyWord;
            copyWord = nowWord;
            if (nowWord.identifyId == IDENTIFY || nowWord.identifyId == INT_CONSTANTS)
                nowWord.identifyId = IDENTIFY;
        }// 规约
        else if (value >= 102 && value < 108){
            switch(value){
                // B   -> B_0 B
                case 102:
                    newTc = *(TC_Stk.end() - 1);
                    newFc = MERG(*(FC_Stk.end() - 2), *(FC_Stk.end() - 1));
                    stateStk.pop_back(), flagStk.pop_back(), placeStk.pop_back(), FC_Stk.pop_back(), TC_Stk.pop_back();
                    stateStk.pop_back(), flagStk.pop_back(), placeStk.pop_back(), FC_Stk.pop_back(), TC_Stk.pop_back();
                    stateStk.push_back(value), flagStk.push_back(B_ID), placeStk.push_back(-1);
                    TC_Stk.push_back(newTc), FC_Stk.push_back(newFc);
                    break;
                // B   -> B_1 B
                case 103:
                    newFc = *(FC_Stk.end() - 1);
                    newTc = MERG(*(TC_Stk.end() - 2), *(TC_Stk.end() - 1));
                    stateStk.pop_back(), flagStk.pop_back(), placeStk.pop_back(), FC_Stk.pop_back(), TC_Stk.pop_back();
                    stateStk.pop_back(), flagStk.pop_back(), placeStk.pop_back(), FC_Stk.pop_back(), TC_Stk.pop_back();
                    stateStk.push_back(value), flagStk.push_back(B_ID), placeStk.push_back(-1);
                    TC_Stk.push_back(newTc), FC_Stk.push_back(newFc);
                    break;
                // B   -> i
                case 104:{  
                    // 本应出栈再进栈 但是只有一个元素 直接修改栈顶即可
                    *(flagStk.end() - 1) = B_ID, *(TC_Stk.end() - 1) = NXQ, *(FC_Stk.end() - 1) = NXQ + 1, *(placeStk.end() - 1) = entry();
                    varIndex = entry(copyWord, -1);
                    *(placeStk.end() - 1) = varIndex;
                    Quat q(JNZ, varIndex, -1, 0);
                    quats.push_back(q);
                    Quat q(J, -1, -1, 0);
                    quats.push_back(q);
                    NXQ += 2;
                    break;
                }
                // B   -> B < B
                case 105:
                // B   -> B > B
                case 106:{
                    int type = value == 105 ? JLES : JGRT;
                    int b1_place = *(placeStk.end() - 2), b2_place = *(placeStk.end() - 1);
                    for (int i = 1; i <= 3; i++)
                        stateStk.pop_back(), flagStk.pop_back(), placeStk.pop_back(), TC_Stk.pop_back(), FC_Stk.pop_back();
                    // stateStk.
                    break;
                }
                // B_0 -> B &&
                case 107:
                    break;
                // B_1 -> B ||
                case 108:
                    break;
            }
        }
    }
    if (acc && !er)return true;
    return false;
}
bool Type(){}
// 单语句
bool Stmt(){
    if (nowWord.identifyId == SEMIC)return true;
    if (nowWord.identifyId == INT || nowWord.identifyId == DBL || nowWord.identifyId == CHAR)variableDefinitions();
}
// 多语句
bool Stmts(){
    Stmt();
    if (nowWord.identifyId == RIGHT_BIG)return true;
    if (nowWord.identifyId == IF)ifStmt();
}
// 代码块
bool Block(){
    if (nowWord.identifyId == LEFT_BIG){
        Stmts();
    }
    if (nowWord.identifyId == RIGHT_BIG){
        return true;
    }
}
// C语言程序
bool Program(){
    if (nowWord.identifyId == INT){
        nowWord = lex.getWord(line, col);
        if (nowWord.identifyId == IDENTIFY && nowWord.word == "main"){
            nowWord = lex.getWord(line, col);
            Block();
        }
    }
}
int main(){
    nowWord = lex.getWord(line, col);
    Program();
    return 0;
}