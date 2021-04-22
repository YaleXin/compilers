/*
 * @Author      : YaleXin
 * @Email       : 181303209@yzu.edu.cn
 * @LastEditors : YaleXin
 */
#include <algorithm>
#include <cstdio>
#include <iostream>
#include <map>
#include <stack>
#include <vector>

#include "../lex.v2.cpp"
#include "../tempStructs.h"
using namespace std;
/*
表达式文法

Expr    -> Temp1 E_1 	(1)
E_1     -> + Temp1 E_1 	(2)
E_1     -> - Temp1 E_1	(3)
E_1		-> epsilon		(4)
Temp1   -> Temp2 E_2	(5)
E_2		-> * Temp2 E_2	(6)
E_2		-> / Temp2 E_2	(7)
E_2		-> epsilon		(8)
Temp2	-> ( Expr )		(9)
Temp2	-> id			(10)
Temp2	-> number		(11)

*/
const int EOF_ID = -2021, ADD_ID = 62, SUB_ID = 63;
const int MUL_ID = 64, DIV_ID = 65, IDENTIFY_ID = 32;
const int LEFT_ID = 39, RIGHT_ID = 40, INTEGER_CONSTANTS_ID = 33;
const int REAL_CONSTANTS_ID = 34;
// 实型整型数字都是number，便于构造预测表
const int NUMBER_ID = 90;
// 空串
const int EPSILON_ID = 91;
// # 号
const int END_ID = 92;
// 大于等于100的都是非终结符
const int OFFSET = 100, EXPR_ID = 100, E_1_ID = 101, TEMP_1_ID = 102,
          E_2_ID = 103, TEMP_2_ID = 104;
// 文法
vector<int> grammar[12] = {{},
                           {TEMP_1_ID, E_1_ID},
                           {ADD_ID, TEMP_1_ID, E_1_ID},
                           {SUB_ID, TEMP_1_ID, E_1_ID},
                           {EPSILON_ID},
                           {TEMP_2_ID, E_2_ID},
                           {MUL_ID, TEMP_2_ID, E_2_ID},
                           {DIV_ID, TEMP_2_ID, E_2_ID},
                           {EPSILON_ID},
                           {LEFT_ID, EXPR_ID, RIGHT_ID},
                           {IDENTIFY_ID},
                           {NUMBER_ID}};
const int ERROR = -1;
// 预测分析表的横坐标每个终结符对应的下标
map<int, int> vtMap = {{LEFT_ID, 0},   {ADD_ID, 1},  {SUB_ID, 2},
                       {MUL_ID, 3},    {DIV_ID, 4},  {IDENTIFY_ID, 5},
                       {NUMBER_ID, 6}, {LEFT_ID, 7}, {END_ID, 8}};
// 预测分析表
int table[5][9] = {{1, -1, -1, -1, -1, 1, 1, -1, -1},
                   {-1, 2, 3, -1, -1, -1, -1, 4, 4},
                   {5, -1, -1, -1, -1, 5, 5, -1, -1},
                   {-1, 8, 8, 6, 7, -1, -1, 8, 8},
                   {9, -1, -1, -1, -1, 10, 11, -1, -1}};
// 第几个单词
int cnt = 0;
map<int,string>index2sting = {{32, "变量"}, {REAL_CONSTANTS_ID, "实型数字"},
                                {INTEGER_CONSTANTS_ID, "整型数字"},
                                {LEFT_ID, "左括号"}, {RIGHT_ID, "右括号"},
                                {ADD_ID, "加号"}, {SUB_ID, "减号"},
                                {MUL_ID, "乘号"}, {DIV_ID, "除号"},
                                {END_ID, "#号"}};
stack<int> myStack;
bool status;
vector<int>output;
Lex lex("D:\\my_cpp_workspace\\compilers\\exp2\\test.cp", status);
void syntaxError(int, int);
void pushProd(int);
bool forecastAnalysis();
void test();
void syntaxError(int errorId , int expectedId = -1) {
    if (expectedId != -1) {
        cout << "syntax analyze fail：expected " << expectedId << " "
             << "but get " << errorId << " in th " << cnt << endl;
    } else {
        cout << "syntax analyze fail：not expected " << expectedId << " in th "
             << cnt << endl;
    }
}
// 将第index号产生式进栈
void pushProd(int index) {
    int len = grammar[index].size();
    // 从右至左进栈
    for (int i = len - 1; i >= 0; i--) myStack.push(grammar[index][i]);
}
bool forecastAnalysis() {
    // # 和文法开始符先依次进栈
    myStack.push(END_ID);
    myStack.push(EXPR_ID);
    bool flag = true;
    int topId;
    int line = 0, col = 0, iIndex, jIndex, nxtProdIndex;
    Result nowWord = lex.getWord(line, col);
    cnt++;
    if (nowWord.identifyId == EOF_ID)nowWord.identifyId = END_ID;
    while (flag) {
        topId = myStack.top();
        // 小于100的均是终结符或者#号
        if (topId < OFFSET) {
            if (topId == EPSILON_ID){
                cout << "匹配 空串" << endl;
                myStack.pop();
                continue;
            }
            // 如果二者相等或者读头是数字
            if (topId == nowWord.identifyId ||
               (topId == NUMBER_ID && 
               (nowWord.identifyId == REAL_CONSTANTS_ID ||
            (nowWord.identifyId == INTEGER_CONSTANTS_ID)))) {
                if (topId == END_ID) {
                    // 成功
                    cout << "匹配 " << index2sting[topId] << endl;
                    return true;
                } else {
                    cout << "匹配 " << index2sting[topId] << endl;
                    // 匹配
                    myStack.pop();
                    nowWord = lex.getWord(line, col);
                    cnt++;
                    if (nowWord.identifyId == EOF_ID)
                        nowWord.identifyId = END_ID;
                }
            } else {
                syntaxError(nowWord.identifyId, topId);
                return false;
            }
        } else {
            // 栈顶是非终结符
            iIndex = topId - OFFSET;
            if(nowWord.identifyId == REAL_CONSTANTS_ID || 
                nowWord.identifyId == INTEGER_CONSTANTS_ID)jIndex = vtMap[NUMBER_ID];
            else jIndex = vtMap[nowWord.identifyId];
            nxtProdIndex = table[iIndex][jIndex];
            if (nxtProdIndex){
                cout << "推导" << endl;
                output.push_back(nxtProdIndex);
                myStack.pop();
                pushProd(nxtProdIndex);
            } else {
                syntaxError(nowWord.identifyId);
                return false;
            }
        }
    }
}
void test(){
    int line, col;
    while(1){
        Result nowWord = lex.getWord(line, col);
        cout << nowWord.identifyId << endl;
        if (nowWord.identifyId == -1 || nowWord.identifyId == EOF_ID)break;
    }
}
int main() {
    // test();
    bool ok = forecastAnalysis();
    ios::sync_with_stdio(false);
    return 0;
}