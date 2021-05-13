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

// ����������ȱ�
/*
�ķ�
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
// ʵ���������ֶ���number
const int NUMBER_ID = 90;
// # ��
const int END_ID = 92;
// ���ڵ���100�Ķ��Ƿ��ս��
const int OFFSET = 100, E_ID = 100, EXPR_ID = 101, TEMP_1_ID = 102,
          TEMP_2_ID = 103;
// �ķ���ÿһ�еĵ�һ��Ԫ���ǲ���ʽ�󲿣�
vector<int> grammar[11] = {{},
                           {E_ID, END_ID, EXPR_ID, END_ID},
                           {EXPR_ID, EXPR_ID, ADD_ID, TEMP_1_ID},
                           {EXPR_ID, EXPR_ID, SUB_ID, TEMP_1_ID},
                           {EXPR_ID, TEMP_1_ID},
                           {TEMP_1_ID, TEMP_1_ID, MUL_ID, TEMP_2_ID},
                           {TEMP_1_ID, TEMP_1_ID, DIV_ID, TEMP_2_ID},
                           {TEMP_1_ID, TEMP_2_ID},
                           {TEMP_2_ID, LEFT_ID, EXPR_ID, RIGHT_ID},
                           {TEMP_2_ID, IDENTIFY_ID},
                           {TEMP_2_ID, NUMBER_ID}};
// ���ս�����Ϻ�β�ս��
vector<int> firstVT[4];
vector<int> lastVT[4];

// ��ά��������isFirst[P,a]Ϊ�����a��P�����ս��
bool isFirst[4][9];
// ͬ��
bool isLast[4][9];
// ������ȱ� -1 ��ϵ����֮���޷��ţ� 0 �� ��ȣ�1��С�ڣ�2������
const int NOT = -1, EQL = 0, LES = 1, GRT = 2;
int priorityTable[9][9];
// ��ż<P, a> P�Ƿ��ս����a���ս��
typedef pair<int, int> P;
stack<P> myStack;
// ���ջ������ջ
vector<int> optr;
vector<int> opnd;


// ���Ⱥ���
int f[9], g[9];

// ��ĺ�����ÿ���ս����Ӧ���±�
map<int, int> vtMap = {{LEFT_ID, 0},   {ADD_ID, 1},   {SUB_ID, 2},
                       {MUL_ID, 3},    {DIV_ID, 4},   {IDENTIFY_ID, 5},
                       {NUMBER_ID, 6}, {RIGHT_ID, 7}, {END_ID, 8}};

string index2vt[] = {"(", "+", "-", "*", "/", "id", "num", ")", "#"};
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
void initialFunction();

int main(int argc, char const *argv[]) {
    initialFirstVT();
    initialLastVT();
    initialPriorityTable();
    initialFunction();
    printf("%-4s %-5s %-10s %-30s %-50s\n", "����", "��ͷ", "����", "���ջ",
           "����ջ");
    operatorPriority();
    return 0;
}
void initialFunction(){
    memset(f, 0, sizeof f);
    memset(g, 0, sizeof g);
    bool change = false;
    do {
        change = false;
        for (int i = 0; i <= 8; i++)
            for (int j = 0; j <= 8; j++) {
                if (priorityTable[i][j] == GRT && f[i] <= g[j]) {
                    f[i] = g[j] + 1;
                    change = true;
                } else if (priorityTable[i][j] == LES && f[i] >= g[j]) {
                    g[j] = f[i] + 1;
                    change = true;
                } else if (priorityTable[i][j] == EQL && f[i] != g[j]) {
                    int maxV = max(f[i], g[j]);
                    change = true;
                }
            }
    } while (change);
    cout << "----- priority function -----" << endl;
    printf("%-7s", "");
    for(int i = 0; i <= 8; i++)printf("%-7s", index2vt[i].c_str());
    printf("\n%-7s", "f()");
    for(int i = 0; i <= 8; i++)printf("%-7d", f[i]);
    printf("\n%-7s", "g()");
    for(int i = 0; i <= 8; i++)printf("%-7d", g[i]);
    cout << "\n----------" << endl;
}
int sum;
void print(string nowStr, string action) {
    printf("%-4d ", sum++);
    printf("%-5s ", nowStr.c_str());
    printf("%-10s ", action.c_str());
    printStack();
    printf("\n");
}
void printStack() {
    int len = opnd.size();
    for (int i = 0; i < len; i++) printf("%-3d ", opnd[i]);
    for (int i = len; i <= 7; i++) printf("%-3s ", " ");
    len = optr.size();
    for (int i = 0; i < len; i++)
        printf("%-3s ", index2vt[vtMap[optr[i]]].c_str());
}
void error(string errorMsg) { cout << "error by " << errorMsg << endl; }
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

bool operatorPriority() {
    int r, c;
    while (!opnd.empty()) opnd.pop_back();
    while (!optr.empty()) optr.pop_back();
    optr.push_back(END_ID);
    Result nowWord = lex.getWord(r, c);
    if (nowWord.identifyId == EOF_ID) nowWord.identifyId = END_ID;
    bool ok = false;
    print(nowWord.word, " ");
    // optrջ������
    int theta;
    while (!ok) {
        theta = optr.back();
        if (theta == END_ID && nowWord.identifyId == END_ID) {
            ok = true;
            // ƥ����������
        } else if (theta == LEFT_ID && nowWord.identifyId == RIGHT_ID) {
            optr.pop_back();
            nowWord = lex.getWord(r, c);
            if (nowWord.identifyId == EOF_ID) nowWord.identifyId = END_ID;
            // ��ȡ���������ֳ���
        } else if (nowWord.identifyId == INTEGER_CONSTANTS_ID) {
            opnd.push_back(stoi(nowWord.word));
            print(nowWord.word, "�ƽ�");
            nowWord = lex.getWord(r, c);
            if (nowWord.identifyId == EOF_ID) nowWord.identifyId = END_ID;
        } else {
            int index1 = vtMap[theta], index2 = vtMap[nowWord.identifyId];
            bool isLess = f[index1] < g[index2];
            bool isGreater = f[index1] > g[index2];
            // �ƽ�
            if (isLess) {
                optr.push_back(nowWord.identifyId);
                print(nowWord.word, "�ƽ�");
                nowWord = lex.getWord(r, c);
                if (nowWord.identifyId == EOF_ID) nowWord.identifyId = END_ID;
            } else if (isGreater) {
                //��Լ
                int num1 = opnd.back();
                opnd.pop_back();
                int num2 = opnd.back();
                opnd.pop_back();
                int rst = calculate(num1, num2, theta);
                opnd.push_back(rst);
                optr.pop_back();
                print(nowWord.word, "��Լ");
            } else {
                error("δ֪����");
                return false;
            }
        }
    }
    printf("----------\n");
    cout << opnd.back() << endl;
}
void initialPriorityTable() {
    memset(priorityTable, -1, sizeof priorityTable);
    for (int i = 1; i <= 10; i++) {
        int len = grammar[i].size();
        for (int j = 1; j < len - 1; j++) {
            // grammar[i][j] �д�ŵ���0-99,Ҫ����ӳ�䵽0-9
            // ���ڵ��ս��
            if (grammar[i][j] < 100 && grammar[i][j + 1] < 100)
                priorityTable[vtMap[grammar[i][j]]][vtMap[grammar[i][j + 1]]] =
                    EQL;
            // ������ս��
            if (j + 2 <= len - 1 && grammar[i][j] < 100 &&
                grammar[i][j + 2] < 100 && grammar[i][j + 1] >= 100)
                priorityTable[vtMap[grammar[i][j]]][vtMap[grammar[i][j + 2]]] =
                    EQL;
            // aP
            if (grammar[i][j] < 100 && grammar[i][j + 1] >= 100) {
                int vtIndex = vtMap[grammar[i][j]], fVt,
                    vnIndex = grammar[i][j + 1] - OFFSET;
                for (int k = 0; k <= 8; k++) {
                    if (isFirst[vnIndex][k]) priorityTable[vtIndex][k] = LES;
                }
            }
            // Pa
            if (grammar[i][j] >= 100 && grammar[i][j + 1] < 100) {
                int vtIndex = vtMap[grammar[i][j + 1]], fVt,
                    vnIndex = grammar[i][j] - OFFSET;
                for (int k = 0; k <= 8; k++) {
                    if (isLast[vnIndex][k]) priorityTable[k][vtIndex] = GRT;
                }
            }
        }
    }
    cout << "----- priority table -----" << endl;
    printf("%-7s", "");
    for (int i = 0; i <= 8; i++) printf("%-7s", index2vt[i].c_str());
    printf("\n");
    for (int i = 0; i <= 8; i++) {
        printf("%-7s", index2vt[i].c_str());
        for (int j = 0; j <= 8; j++) {
            if (priorityTable[i][j] == LES)
                printf("%-7s", "<");
            else if (priorityTable[i][j] == EQL)
                printf("%-7s", "=");
            else if (priorityTable[i][j] == GRT)
                printf("%-7s", ">");
            else
                printf("%-7s", "");
        }
        printf("\n");
    }
    cout << "----------------" << endl;
}
void initialFirstVT() {
    while (!myStack.empty()) myStack.pop();
    // P -> a... ���� P -> Qa...
    for (int i = 1; i <= 10; i++)
        if (grammar[i][1] < 100)
            firstInsert(grammar[i][0], grammar[i][1]);
        else if (grammar[i].size() >= 3 && grammar[i][2] < 100)
            firstInsert(grammar[i][0], grammar[i][2]);
    while (!myStack.empty()) {
        P top = myStack.top();
        myStack.pop();
        int Q = top.first, a = top.second;
        for (int i = 1; i <= 10; i++)
            if (grammar[i][1] == Q) firstInsert(grammar[i][0], a);
    }
    cout << "--------- firstVT �� ---------" << endl;

    printf("%-7s%-7s%-7s%-7s%-7s%-7s%-7s%-7s%-7s%-7s\n", "", "(", "+", "-", "*",
           "/", "id", "num", ")", "#");
    printf("%-7s", "E");
    for (int i = 0; i <= 8; i++)
        printf("%-7s", isFirst[0][i] ? "true" : "false");
    printf("\n%-7s", "Expr");
    for (int i = 0; i <= 8; i++)
        printf("%-7s", isFirst[1][i] ? "true" : "false");
    printf("\n%-7s", "Temp1");
    for (int i = 0; i <= 8; i++)
        printf("%-7s", isFirst[2][i] ? "true" : "false");
    printf("\n%-7s", "Temp2");
    for (int i = 0; i <= 8; i++)
        printf("%-7s", isFirst[3][i] ? "true" : "false");
    cout << "\n---------------------" << endl;
}
void initialLastVT() {
    while (!myStack.empty()) myStack.pop();
    // P -> ...a ���� P -> ...aQ
    for (int i = 1; i <= 10; i++)
        if (*(grammar[i].end() - 1) < 100)
            lastInsert(grammar[i][0], *(grammar[i].end() - 1));
        else if (grammar[i].size() >= 3 && *(grammar[i].end() - 2) < 100)
            lastInsert(grammar[i][0], *(grammar[i].end() - 2));
    while (!myStack.empty()) {
        P top = myStack.top();
        myStack.pop();
        int Q = top.first, a = top.second;
        for (int i = 1; i <= 10; i++)
            if (grammar[i][1] == Q) lastInsert(grammar[i][0], a);
    }
    cout << "--------- lastVT �� ---------" << endl;

    printf("%-7s%-7s%-7s%-7s%-7s%-7s%-7s%-7s%-7s%-7s\n", "", "(", "+", "-", "*",
           "/", "id", "num", ")", "#");
    printf("%-7s", "E");
    for (int i = 0; i <= 8; i++)
        printf("%-7s", isLast[0][i] ? "true" : "false");
    printf("\n%-7s", "Expr");
    for (int i = 0; i <= 8; i++)
        printf("%-7s", isLast[1][i] ? "true" : "false");
    printf("\n%-7s", "Temp1");
    for (int i = 0; i <= 8; i++)
        printf("%-7s", isLast[2][i] ? "true" : "false");
    printf("\n%-7s", "Temp2");
    for (int i = 0; i <= 8; i++)
        printf("%-7s", isLast[3][i] ? "true" : "false");
    cout << "\n---------------------" << endl;
}
void firstInsert(int VN, int VT) {
    if (!isFirst[VN - OFFSET][vtMap[VT]]) {
        isFirst[VN - OFFSET][vtMap[VT]] = true;
        P p(VN, VT);
        myStack.push(p);
    }
}

void lastInsert(int VN, int VT) {
    if (!isLast[VN - OFFSET][vtMap[VT]]) {
        isLast[VN - OFFSET][vtMap[VT]] = true;
        P p(VN, VT);
        myStack.push(p);
    }
}