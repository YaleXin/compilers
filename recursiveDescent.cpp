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


// 动态生成的常量表
vector<int> intConstants;
vector<double> doubleConstants;
vector<string> stringConstants;

vector<string> identifyTable;

const int EOF_ID = -2021;

int main(int argc, char const *argv[]){
    
    return 0;
}



// 测试 词法分析器是否能够正常运行
int test() {
    ios::sync_with_stdio(false);
    string filename="D:\\my_cpp_workspace\\compilers\\test.c";
    bool status;
    Lex lex(filename, status);
    while(1){
        Result r = lex.getWord();
        if (r.identifyId == -EOF_ID || r.identifyId == -1)break;
        else printf("<\t%-20s,%02d,%02d\t>\n", r.word.c_str(),
               r.identifyId, r.internalCode);
    }
    return 0;
}