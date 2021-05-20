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

bool status;
Lex lex("D:\\my_cpp_workspace\\compilers\\exp4\\test.cp", status);
Result nowWord;
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
int l, c;
bool S();
bool E();
int main(int argc, char const *argv[]){
    return 0;
}

