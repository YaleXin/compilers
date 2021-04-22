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
���ʽ�ķ�

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
// ʵ���������ֶ���number�����ڹ���Ԥ���
const int NUMBER_ID = 90;
// �մ�
const int EPSILON_ID = 91;
// # ��
const int END_ID = 92;
// ���ڵ���100�Ķ��Ƿ��ս��
const int OFFSET = 100, EXPR_ID = 100, E_1_ID = 101, TEMP_1_ID = 102,
          E_2_ID = 103, TEMP_2_ID = 104;
// �ķ�
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
// Ԥ�������ĺ�����ÿ���ս����Ӧ���±�
map<int, int> vtMap = {{LEFT_ID, 0},   {ADD_ID, 1},  {SUB_ID, 2},
                       {MUL_ID, 3},    {DIV_ID, 4},  {IDENTIFY_ID, 5},
                       {NUMBER_ID, 6}, {LEFT_ID, 7}, {END_ID, 8}};
// Ԥ�������
int table[5][9] = {{1, -1, -1, -1, -1, 1, 1, -1, -1},
                   {-1, 2, 3, -1, -1, -1, -1, 4, 4},
                   {5, -1, -1, -1, -1, 5, 5, -1, -1},
                   {-1, 8, 8, 6, 7, -1, -1, 8, 8},
                   {9, -1, -1, -1, -1, 10, 11, -1, -1}};
// �ڼ�������
int cnt = 0;
map<int,string>index2sting = {{32, "����"}, {REAL_CONSTANTS_ID, "ʵ������"},
                                {INTEGER_CONSTANTS_ID, "��������"},
                                {LEFT_ID, "������"}, {RIGHT_ID, "������"},
                                {ADD_ID, "�Ӻ�"}, {SUB_ID, "����"},
                                {MUL_ID, "�˺�"}, {DIV_ID, "����"},
                                {END_ID, "#��"}};
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
        cout << "syntax analyze fail��expected " << expectedId << " "
             << "but get " << errorId << " in th " << cnt << endl;
    } else {
        cout << "syntax analyze fail��not expected " << expectedId << " in th "
             << cnt << endl;
    }
}
// ����index�Ų���ʽ��ջ
void pushProd(int index) {
    int len = grammar[index].size();
    // ���������ջ
    for (int i = len - 1; i >= 0; i--) myStack.push(grammar[index][i]);
}
bool forecastAnalysis() {
    // # ���ķ���ʼ�������ν�ջ
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
        // С��100�ľ����ս������#��
        if (topId < OFFSET) {
            if (topId == EPSILON_ID){
                cout << "ƥ�� �մ�" << endl;
                myStack.pop();
                continue;
            }
            // ���������Ȼ��߶�ͷ������
            if (topId == nowWord.identifyId ||
               (topId == NUMBER_ID && 
               (nowWord.identifyId == REAL_CONSTANTS_ID ||
            (nowWord.identifyId == INTEGER_CONSTANTS_ID)))) {
                if (topId == END_ID) {
                    // �ɹ�
                    cout << "ƥ�� " << index2sting[topId] << endl;
                    return true;
                } else {
                    cout << "ƥ�� " << index2sting[topId] << endl;
                    // ƥ��
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
            // ջ���Ƿ��ս��
            iIndex = topId - OFFSET;
            if(nowWord.identifyId == REAL_CONSTANTS_ID || 
                nowWord.identifyId == INTEGER_CONSTANTS_ID)jIndex = vtMap[NUMBER_ID];
            else jIndex = vtMap[nowWord.identifyId];
            nxtProdIndex = table[iIndex][jIndex];
            if (nxtProdIndex){
                cout << "�Ƶ�" << endl;
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