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
bool status;
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
int main(int argc, char const *argv[]){
    if (!status){
        return 0;
    }
    return 0;
}



// ���� �ʷ��������Ƿ��ܹ���������
int test() {
    ios::sync_with_stdio(false);
    string filename="D:\\my_cpp_workspace\\compilers\\test.c";
    bool status;
    Lex lexTest(filename, status);
    while(1){
        Result r = lexTest.getWord();
        if (r.identifyId == -EOF_ID || r.identifyId == -1)break;
        else printf("<\t%-20s,%02d,%02d\t>\n", r.word.c_str(),
               r.identifyId, r.internalCode);
    }
    return 0;
}
void program(){
    
}