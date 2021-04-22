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
const int OFFSET = 100, EXPR_ID = 101, E_1_ID = 102, TEMP_1_ID = 103,
          E_2_ID = 104, TEMP_2_ID = 105;
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
// 预测分析表
int table[5][9] = {{1, -1, -1, -1, -1, 1, 1, -1, -1},
                   {-1, 2, 3, -1, -1, -1, -1, 4, 4},
                   {5, -1, -1, -1, -1, 5, 5, -1, -1},
                   {-1, 8, 8, 6, 7, -1, -1, 8, 8},
                   {9, -1, -1, -1, -1, 10, 11, -1, -1}};

stack<int> myStack;
bool status;
Lex lex("D:\\my_cpp_workspace\\compilers\\exp2\\test.cp", status);
void syntaxError(int expectedId, int errorId) {
    cout << "syntax analyze fail：expected " << expectedId << " "
         << "but get " << errorId << endl;
}
// 将第index号产生式进栈
void pushProd(int index) {
    int len = grammar[index].size();
    myStack.pop();
    // 从右至左进栈
    for (int i = len - 1; i >= 0; i++) myStack.push(grammar[index][i]);
}
bool forecastAnalysis() {
    // # 和文法开始符先依次进栈
    myStack.push(END_ID);
    myStack.push(EXPR_ID);
    bool flag = true;
    int topId;
    int line = 0, col = 0;
    Result nowWord = lex.getWord(line, col);
    while (flag) {
        topId = myStack.top();
        // 小于100的均是非终结符或者#号
        if (topId < OFFSET) {
            if (topId == nowWord.identifyId) {
                if (topId == nowWord.identifyId) {
                    if (topId == END_ID) {
                        // 成功
                        return true;
                    } else {
                        // 匹配
                        myStack.pop();
                        nowWord = lex.getWord(line, col);
                    }
                } else {
                    syntaxError(topId, nowWord.identifyId);
                }
            }
        }
    }
}
int main() {
    ios::sync_with_stdio(false);
    return 0;
}