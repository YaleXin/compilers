#include <iostream>
#include <cstdio>
#include "lexicalAnalyzer.cpp"
#include "tempStructs.h"
using namespace std;
/**
 * �������������ʷ������������﷨���������м�����������������Ż�����Ŀ�����������
 */ 
int main(int arc, const char *argv[]){
    std::ios::sync_with_stdio(false);
    LexicalAnalyzer l;
    // rsl���﷨�����������﷨����
    LexAnalyzerResult rsl;
    if (arc == 2)rsl = l.main(argv[1]);
    else rsl = l.main();
    return 0;
}