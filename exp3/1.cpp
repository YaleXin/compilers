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
#include <stack>
#include <map>
#include <cstring>

#include "../lex.v2.cpp"
#include "../tempStructs.h"
using namespace std;

// 基于算符优先表
/*
文法
E		-> # Expr # 		(1)
Expr	-> Expr + Temp1		(2)
Expr	->  Expr - Temp1	(3)
Expr	->  Temp1			(4)
Temp1	-> Temp1 * Temp2	(5)
Temp1	->  Temp1 / Temp2	(6)
Temp1	->  Temp2			(7)
Temp2	-> ( Expr )			(8)
Temp2	->  id				(9)
Temp2	->  number			(10)

*/

const int EOF_ID = -2021, ADD_ID = 62, SUB_ID = 63;
const int MUL_ID = 64, DIV_ID = 65, IDENTIFY_ID = 32;
const int LEFT_ID = 39, RIGHT_ID = 40, INTEGER_CONSTANTS_ID = 33;
const int REAL_CONSTANTS_ID = 34;
// 实型整型数字都是number
const int NUMBER_ID = 90;
// # 号
const int END_ID = 92;
// 大于等于100的都是非终结符
const int OFFSET = 100, E_ID = 100, EXPR_ID = 101, TEMP_1_ID = 102, TEMP_2_ID = 103;
// 文法（每一行的第一个元素是产生式左部）
vector<int> grammar[11] = {{},
                            {E_ID,      END_ID, EXPR_ID, END_ID},
                            {EXPR_ID,   EXPR_ID, ADD_ID, TEMP_1_ID},
                            {EXPR_ID,   EXPR_ID, SUB_ID, TEMP_1_ID},
                            {EXPR_ID,   TEMP_1_ID},
                            {TEMP_1_ID, TEMP_1_ID, MUL_ID, TEMP_2_ID},
                            {TEMP_1_ID, TEMP_1_ID, DIV_ID, TEMP_2_ID},
                            {TEMP_1_ID, TEMP_2_ID},
                            {TEMP_2_ID, LEFT_ID, EXPR_ID, RIGHT_ID},
                            {TEMP_2_ID, IDENTIFY_ID},
                            {TEMP_2_ID, NUMBER_ID}
                        };
// 首终结符集合和尾终结符
vector<int>firstVT[4];
vector<int>lastVT[4];

// 二维布尔矩阵，isFirst[P,a]为真代表a是P的首终结符
bool isFirst[4][9];
// 同理
bool isLast[4][9];
// 算符优先表 -1 关系符号之间无符号， 0 ： 相等，1：小于，2：大于
const int NOT = -1, EQL = 0, LES = 1, GRT = 2;
int priorityTable[9][9];
// 序偶<P, a> P是非终结符，a是终结符
typedef pair<int,int>P;
stack<P> myStack;
// 算符栈和算量栈
vector<int>optr;
vector<int>opnd;

// 表的横坐标每个终结符对应的下标
map<int, int> vtMap = {{LEFT_ID, 0},   {ADD_ID, 1},  {SUB_ID, 2},
                       {MUL_ID, 3},    {DIV_ID, 4},  {IDENTIFY_ID, 5},
                       {NUMBER_ID, 6}, {RIGHT_ID, 7}, {END_ID, 8}};

string index2vt[] = {"(", "+", "-", "*", "/", "id", "number", ")", "#"};
vector<Result> ansSet;
bool status;
Lex lex("D:\\my_cpp_workspace\\compilers\\exp3\\test.cp", status);

void initialFirstVT();
void initialLastVT();
void initialPriorityTable();
void firstInsert(int, int);
void lastInsert(int, int);
void insert(stack<P> &);
void error(string);
bool operatorPriority();
void printStack();
int calculate(int, int, int);

int main(int argc, char const *argv[]) { 
    initialFirstVT();
    initialLastVT();
    initialPriorityTable();
    operatorPriority();
    return 0; 
}
    void printStack() {
        cout  << "------ opnd stack ------" << endl;
        int len = opnd.size();
        for (int i = 0; i < len; i++) cout << opnd[i] << ends;
        cout << endl << "------ optr stack ------" << endl;
        len = optr.size();
        for (int i = 0; i < len; i++) cout << index2vt[vtMap[optr[i]]] << ends;
        cout << endl << endl;
    }
void error(string errorMsg){
    cout << "error by " << errorMsg << endl;
}
int calculate(int num1, int num2, int op) {
    int sum = 0;
    switch (op) {
        case ADD_ID:
            sum = num2 + num1;
            break;
        case SUB_ID:
            sum = num2 - num1;
            break;
        case MUL_ID:
            sum = num2 * num1;
            break;
        case DIV_ID:
            sum = num2 / num1;
            break;
        default:
            sum = 0;
    }
    return sum;
}

bool operatorPriority(){
    int r, c;
    while(!opnd.empty())opnd.pop_back();
    while(!optr.empty())optr.pop_back();
    optr.push_back(END_ID);
    Result nowWord = lex.getWord(r, c);
    if (nowWord.identifyId == EOF_ID)nowWord.identifyId = END_ID;
    bool ok = false;
    printStack();
    // optr栈顶符号
    int theta;
    while (!ok) {
        theta = optr.back();
        if (theta == END_ID && nowWord.identifyId == END_ID) {
            ok = true;
            // 匹配左右括号
        } else if (theta == LEFT_ID && nowWord.identifyId == RIGHT_ID) {
            optr.pop_back();
            nowWord = lex.getWord(r, c);
            if (nowWord.identifyId == EOF_ID)nowWord.identifyId = END_ID;
            // 读取到整型数字常量
        } else if (nowWord.identifyId == INTEGER_CONSTANTS_ID){
            cout << "移进" << endl;
            opnd.push_back(stoi(nowWord.word));
            printStack();
            nowWord = lex.getWord(r, c);
            if (nowWord.identifyId == EOF_ID)nowWord.identifyId = END_ID;
        } else {
            int theta1 = vtMap[theta], theta2 = vtMap[nowWord.identifyId];
            int status = priorityTable[theta1][theta2];
            // 移进
            if (status == LES) {
                cout << "移进" << endl;
                optr.push_back(nowWord.identifyId);
                printStack();
                nowWord = lex.getWord(r, c);
                if (nowWord.identifyId == EOF_ID) nowWord.identifyId = END_ID;
            } else if (status == GRT) {
                cout << "规约" <<endl;
                //规约
                int num1 = opnd.back();
                opnd.pop_back();
                int num2 = opnd.back();
                opnd.pop_back();
                int rst = calculate(num1, num2, theta);
                opnd.push_back(rst);
                optr.pop_back();
                printStack();
            } else {
                error("未知错误");
                return false;
            }
        }
    }
    cout << opnd.back() << endl;
}
void initialPriorityTable(){
    memset(priorityTable, -1, sizeof priorityTable);
    for (int  i = 1; i <= 10; i++){
        int len = grammar[i].size();
        for(int j = 1; j < len - 1; j++){
            // grammar[i][j] 中存放的是0-99,要把它映射到0-9
            // 相邻的终结符
            if(grammar[i][j] < 100 && grammar[i][j + 1] < 100)
                priorityTable[vtMap[grammar[i][j]]][vtMap[grammar[i][j + 1]]] = EQL;
            // 间隔的终结符
            if (j + 2 <= len - 1 && 
                grammar[i][j] < 100 && grammar[i][j + 2] < 100 &&
                grammar[i][j + 1] >= 100)
                priorityTable[vtMap[grammar[i][j]]][vtMap[grammar[i][j + 2]]] = EQL;
            // aP
            if (grammar[i][j] < 100 && grammar[i][j + 1] >= 100){
                int vtIndex = vtMap[grammar[i][j]], fVt, vnIndex = grammar[i][j + 1] - OFFSET;
                for(int k = 0; k <= 8; k++){
                    if (isFirst[vnIndex][k])priorityTable[vtIndex][k] = LES;
                }
            }
            // Pa
            if (grammar[i][j] >= 100 && grammar[i][j + 1] < 100){
                int vtIndex = vtMap[grammar[i][j + 1]], fVt, vnIndex = grammar[i][j] - OFFSET;
                for(int k = 0; k <= 8; k++){
                    if (isLast[vnIndex][k])priorityTable[k][vtIndex] = GRT;
                }
            }
        }
    }
    cout << "----- priority table -----" << endl;
    printf("%-7s", "");
    for(int i = 0; i <= 8; i++)printf("%-7s", index2vt[i].c_str());
    printf("\n");
    for(int i = 0; i <= 8; i++){
        printf("%-7s", index2vt[i].c_str());
        for(int j = 0; j <= 8; j++){
            if(priorityTable[i][j] == LES)printf("%-7s", "<");
            else if(priorityTable[i][j] == EQL)printf("%-7s", "=");
            else if(priorityTable[i][j] == GRT)printf("%-7s", ">");
            else printf("%-7s", "");
        }
        printf("\n");
    }
    cout << "----------------" << endl;
}
void initialFirstVT(){
    while(!myStack.empty())myStack.pop();
    // P -> a... 或者 P -> Qa...
    for(int i = 1; i <= 10; i++)
        if(grammar[i][1] < 100 )firstInsert(grammar[i][0], grammar[i][1]);
        else if(grammar[i].size() >= 3 && grammar[i][2] < 100)firstInsert(grammar[i][0], grammar[i][2]);
    while(!myStack.empty()){
        P top = myStack.top();
        myStack.pop();
        int Q = top.first, a = top.second;
        for(int i = 1; i <= 10; i++)
            if(grammar[i][1] == Q)firstInsert(grammar[i][0], a);
    }
    cout << "--------- firstVT 表 ---------" << endl;

    printf("%-7s%-7s%-7s%-7s%-7s%-7s%-7s%-7s%-7s%-7s\n", "", "(", "+", "-", "*", "/", "id", "number", ")", "#");
    printf("%-7s", "E");
    for (int i = 0; i <= 8; i++)printf("%-7s", isFirst[0][i] ? "true" : "false");
    printf("\n%-7s", "Expr");
    for (int i = 0; i <= 8; i++)printf("%-7s", isFirst[1][i] ? "true" : "false");
    printf("\n%-7s", "Temp1");
    for (int i = 0; i <= 8; i++)printf("%-7s", isFirst[2][i] ? "true" : "false");
    printf("\n%-7s", "Temp2");
    for (int i = 0; i <= 8; i++)printf("%-7s", isFirst[3][i] ? "true" : "false");
    cout << "\n---------------------" << endl;
}
void initialLastVT(){
while(!myStack.empty())myStack.pop();
    // P -> ...a 或者 P -> ...aQ
    for(int i = 1; i <= 10; i++)
        if(*(grammar[i].end() - 1) < 100 )lastInsert(grammar[i][0], *(grammar[i].end() - 1));
        else if(grammar[i].size() >= 3 && *(grammar[i].end() - 2) < 100)lastInsert(grammar[i][0], *(grammar[i].end() - 2));
    while(!myStack.empty()){
        P top = myStack.top();
        myStack.pop();
        int Q = top.first, a = top.second;
        for(int i = 1; i <= 10; i++)
            if(grammar[i][1] == Q)lastInsert(grammar[i][0], a);
    }
    cout << "--------- lastVT 表 ---------" << endl;

    printf("%-7s%-7s%-7s%-7s%-7s%-7s%-7s%-7s%-7s%-7s\n", "", "(", "+", "-", "*", "/", "id", "number", ")", "#");
    printf("%-7s", "E");
    for (int i = 0; i <= 8; i++)printf("%-7s", isLast[0][i] ? "true" : "false");
    printf("\n%-7s", "Expr");
    for (int i = 0; i <= 8; i++)printf("%-7s", isLast[1][i] ? "true" : "false");
    printf("\n%-7s", "Temp1");
    for (int i = 0; i <= 8; i++)printf("%-7s", isLast[2][i] ? "true" : "false");
    printf("\n%-7s", "Temp2");
    for (int i = 0; i <= 8; i++)printf("%-7s", isLast[3][i] ? "true" : "false");
    cout << "\n---------------------" << endl;
}
void firstInsert(int VN, int VT){
    if(!isFirst[VN - OFFSET][vtMap[VT]]){
        isFirst[VN - OFFSET][vtMap[VT]] = true;
        P p(VN, VT);
        myStack.push(p);
    }
}

void lastInsert(int VN, int VT){
    if(!isLast[VN - OFFSET][vtMap[VT]]){
        isLast[VN - OFFSET][vtMap[VT]] = true;
        P p(VN, VT);
        myStack.push(p);
    }
}