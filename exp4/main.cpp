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
const int LEFT = 39, RIGHT = 40, INT_CONSTANTS = 33;
const int REAL_CONSTANTS = 34, EQL = 53;
const int ACC = 0, E_ID = 101;
bool status;
Lex lex("D:\\my_cpp_workspace\\compilers\\exp4\\test.cp", status);
Result nowWord("", -1);
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
// ÿ���ս����action���ж�Ӧ���±�
map<int, int> vtMap = {{IDENTIFY, 0}, {ADD, 1}, {SUB, 2}, {MUL, 3}, {DIV, 4}, {LEFT, 5}, {RIGHT, 6}, {END, 7}, {E_ID, 8}};
vector<int>program[10] = {{IDENTIFY, ADD, IDENTIFY}};
string index2vt[] = {"i", "+", "-", "*", "/", "(", ")", "#", "E"};

int l, c;
// ����ջ ��״̬ջ
vector<int>flagStk, stateStk;
bool S();
bool E();
void print(string, string);
void printStack();
int main(int argc, char const *argv[]){
    printf("%-4s %-5s %-10s %-30s %-50s\n", "����", "��ͷ", "����", "״̬ջ",
           "����ջ");
    nowWord = lex.getWord(l, c);
    S();
    return 0;
}

bool S(){
    if (nowWord.identifyId == IDENTIFY){
        nowWord = lex.getWord(l, c);
        if (nowWord.identifyId == EQL){
            nowWord = lex.getWord(l, c);
            E();
        }
    }
}
int getTable(int stateTop,const Result &nowWord){
    return action[stateTop][vtMap[nowWord.identifyId]];
}
// ����Ӽ��˳�
void handleCal(int type) {
    for (int i = 1; i <= 3; i++) stateStk.pop_back(), flagStk.pop_back();
    flagStk.push_back(E_ID);
    int stateTop = *(stateStk.end() - 1);
    int value = action[stateTop][8];
    stateStk.push_back(value);
}
bool E(){
    // ״̬
    int state = 0;
    stateStk.push_back(0), flagStk.push_back(END);
    print(nowWord.word, "");
    if (nowWord.identifyId == IDENTIFY || nowWord.identifyId == INT_CONSTANTS)
        nowWord.identifyId = IDENTIFY;
    bool acc = false, er = false;
    int stateTop, flagTOP, value;
    while(!acc && !er){
        stateTop = *(stateStk.end() - 1);
        // value = getTable(stateTop, nowWord);
        // ���
        value = action[stateTop][vtMap[nowWord.identifyId]];
        if (value == -1)er = true;
        else if (value == -2)acc = true;
        // �ƽ�
        else if (value >= 0 && value < 100){
            // printf("�ƽ�ת%d\n", value);
            stateStk.push_back(value), flagStk.push_back(nowWord.identifyId);
            print(nowWord.word, "�ƽ�");
            nowWord = lex.getWord(l, c);
            if (nowWord.identifyId == IDENTIFY || nowWord.identifyId == INT_CONSTANTS)
                nowWord.identifyId = IDENTIFY;
        }
        if (value >= 101 && value <= 200){
            // printf("���� %d ��Լ\n", value);
            switch(value){
                case 101: 
                case 102: 
                case 103: 
                case 104: 
                    handleCal(0);
                    print(nowWord.word, "��Լ");
                    break;
                // E -> (E)
                case 105:
                    for (int i = 1; i <= 3; i++) stateStk.pop_back(), flagStk.pop_back();
                    flagStk.push_back(E_ID);
                    stateTop = *(stateStk.end() - 1);
                    value = action[stateTop][8];
                    stateStk.push_back(value);
                    break;
                // E -> i
                case 106:
                    stateStk.pop_back(), flagStk.pop_back();
                    flagStk.push_back(E_ID);
                    stateTop = *(stateStk.end() - 1);
                    stateStk.push_back(action[stateTop][8]);
                    print(nowWord.word, "��Լ");
                    break;
            }
        }
    }
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
    for (int i = 0; i < len; i++) printf("%-3d ", stateStk[i]);
    for (int i = len; i <= 7; i++) printf("%-3s ", " ");
    len = flagStk.size();
    for (int i = 0; i < len; i++)
        printf("%-3s ", index2vt[vtMap[flagStk[i]]].c_str());

}