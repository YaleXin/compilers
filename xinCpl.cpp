#include<iostream>
#include<cstdio>
#include "lexicalAnalyzer.cpp"
#include "tempStructs.h"
using namespace std;
/**
 * 编译器：包含词法分析武器、语法分析器、中间代码生成器、代码优化器、目标代码生成器
 */ 
int main(int arc, const char *argv[]){
    std::ios::sync_with_stdio(false);
    LexicalAnalyzer l;
    if (arc == 2)l.main(argv[1]);
    else l.main();
    return 0;
}