#include <iostream>
#include <cstdio>
#include "lexicalAnalyzer.cpp"
#include "tempStructs.h"
using namespace std;
/**
 * 编译器：包含词法分析武器、语法分析器、中间代码生成器、代码优化器、目标代码生成器
 */ 
int main(int arc, const char *argv[]){
    std::ios::sync_with_stdio(false);
    LexicalAnalyzer l;
    // rsl给语法分析器进行语法分析
    LexAnalyzerResult rsl;
    if (arc == 2)rsl = l.main(argv[1]);
    else rsl = l.main();
    return 0;
}