/*
 * @Author      : YaleXin
 * @Email       : 181303209@yzu.edu.cn
 * @LastEditors : YaleXin
 */
#include <algorithm>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <vector>

#include "lex.v2.cpp"
#include "tempStructs.h"
using namespace std;

vector<Result> ansSet;
bool status, matching;
Lex lex("D:\\my_cpp_workspace\\compilers\\test.c", status);

// ��̬���ɵĳ�����
vector<int> intConstants;
vector<double> doubleConstants;
vector<string> stringConstants;

vector<string> identifyTable;
/*
 ��ǰ��ͷ��Ӧ�ĵ��ʣ���Ȼ����ֱ����string���б��棬
 ����Ϊ�˷����ȡ�õ��ʵ���ź����룬ֱ��ʹ��Result����
*/
Result nowWord;
/*

*/

const int EOF_ID = -2021;
int lineNum, colNum;
void program();
void block();
void statements();
void statement();
void type();
void boolExp();
void boolExp2();
void temp0();
void expr();
void temp1();
void temp2();
int main(int argc, char const *argv[]) {
    if (!status) {
        return 0;
    }
    program();
    if (matching) {
        cout << "syntax correct!" << endl;
    } else {
        cout << "syntax fail!" << endl;
    }
    return 0;
}

// ���� �ʷ��������Ƿ��ܹ���������
int test() {
    ios::sync_with_stdio(false);
    string filename = "D:\\my_cpp_workspace\\compilers\\test.c";
    bool status;
    int l, c;
    Lex lexTest(filename, status);
    while (1) {
        Result r = lexTest.getWord(l, c);
        if (r.identifyId == EOF_ID || r.identifyId == -1)
            break;
        else
            printf("<\t%-20s,%02d,%02d\t>\n", r.word.c_str(), r.identifyId,
                   r.internalCode);
    }
    return 0;
}

void program() {
    Result intKeyWord = lex.getWord(lineNum, colNum);
    Result mainFunc = lex.getWord(lineNum, colNum);
    Result l0 = lex.getWord(lineNum, colNum);
    Result r0 = lex.getWord(lineNum, colNum);
    if (intKeyWord.word == "int" && mainFunc.word == "main" && l0.word == "(" &&
        r0.word == ")") {
        block();
    } else {
        matching = false;
        error("");
    }
}

void block() {
    Result lPt = lex.getWord(lineNum, colNum);
    if (matching) statements();
    Result rPt = lex.getWord(lineNum, colNum);
}

void statements(){
    statement();
}

void statement(){
    
}

void error(string errorMsg) {
    cout << "syntax analyze error!, because is : " << errorMsg
         << "in row: " << (lineNum + 1) << ", column: " << (colNum + 1) << endl;
}