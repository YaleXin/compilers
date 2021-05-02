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

#include "../lex.v2.cpp"
#include "../tempStructs.h"
using namespace std;
vector<int> intConstants;
vector<double> doubleConstants;
vector<string> stringConstants;
vector<string> identifyTable;
const int EOF_ID = -2021;
bool status;
int line, col;
Lex lex("D:\\my_cpp_workspace\\compilers\\exp1\\test.cp", status, 
intConstants, doubleConstants, stringConstants, identifyTable);
int main(){
    Result nowWord("", -1);
    while (1) {
        Result r = lex.getWord(line, col);
        if (r.identifyId == EOF_ID || r.identifyId == -1) break;
        else
            printf("<\t%-20s,%02d,%02d\t>\n", r.word.c_str(), r.identifyId,
                   r.internalCode);
    }
    return 0;
}