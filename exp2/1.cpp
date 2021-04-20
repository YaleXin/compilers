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

#include "../lex.v2.cpp"
#include "../tempStructs.h"
using namespace std;

vector<Result> ansSet;
bool status, matching;
Lex lex("D:\\my_cpp_workspace\\compilers\\exp2\\test.cp", status);

// 动态生成的常量表
vector<int> intConstants;
vector<double> doubleConstants;
vector<string> stringConstants;

vector<string> identifyTable;
/*
 当前读头对应的单词，当然可以直接用string进行保存，
 但是为了方便获取该单词的类号和内码，直接使用Result类型
*/
// Result nowWord;
/*

*/

const int EOF_ID = -2021;
const int ADD = 62;
const int SUB = 63;
const int MUL = 64;
const int DIV = 65;
const int IDENTIFY = 65;
const int LEFT = 39;
const int RIGHT = 40;
int lineNum, colNum;
int test();
bool expr();
bool e_1();
bool e_2();
bool temp1();
bool temp2();
int main(int argc, char const *argv[]) {
    // test();
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

bool expr() {
    temp1();
    e_1();
}
bool e_1() {}
bool e_2() {
    Result word = lex.getWord(lineNum, colNum);
    // 空也是可以的
    if (word.identifyId == -1 || word.identifyId == EOF_ID)
        return true;
    // else if ()
}
bool temp1() {
    temp2();
    e_2();
}
bool temp2() {}

void error(string errorMsg) {
    cout << "syntax analyze error!, because is : " << errorMsg
         << "in row: " << (lineNum + 1) << ", column: " << (colNum + 1) << endl;
}