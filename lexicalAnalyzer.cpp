/*
 * @Author      : YaleXin
 * @Email       : 181303209@yzu.edu.cn
 * @LastEditors : YaleXin
 */
#include <algorithm>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;
struct Result {
    string word;
    int identifyId, internalCode;
};

// 保留字 有序列表，方便于折半查找
const string reservedWords[] = {
    "auto",     "break",  "case",    "char",   "const",    "continue",
    "default",  "do",     "double",  "else",   "enum",     "extern",
    "float",    "for",    "goto",    "if",     "int",      "long",
    "register", "return", "short",   "signed", "sizeof",   "static",
    "struct",   "switch", "typedef", "union",  "unsigned", "void",
    "volatile", "while",
};
const int WORDS_LEN = 32;
// 标识符类号
const int IDENTIFY_ID = 32;
const int INTEGER_CONSTANTS_ID = 33;
const int REAL_CONSTANTS_ID = 34;
const int STRING_CONSTANTS_ID = 35;
// 标识符类型
enum IdentifyType { CHAR, SHORT, INT, LONG, LONG_LONG, ARRAY, FUNCTION };
// 界符
const string delimiter[] = {
    ",", ";", "(", ")", "{", "}", "[",  "]", "<",  "<=", ">", ">=", "==", "!==",
    "=", "~", "!", "?", ":", "|", "||", "&", "&&", "+",  "-", "*",  "/",  "%"};
void error() {}
int rowNum, culNum;
bool isDigital(const char c){ return c >= '0' && c <= '9';}
bool isLetter(const char c){ return (c >= 'a' && c <='z') || (c >= 'A' && c <='Z');}
bool isBlank(const char c) { return c == '\n' || c == '\t' || c == ' ';}
/**
 * @description: 返回指定字符在 delimiter 表中的位置
 * @param {const char} c 待查找字符
 * @return {*} 指定字符在 delimiter 表中的位置，若不存在，返回-1
 */
int getDelimiterIndex(const char c){ 
    int index = -1;
    for(int i = 0; i <= 27; i++){
        if (delimiter[i] == c){
            index = i;
            break;
        }
    }
    return index;
}
// 预处理
void preprocess(){}
/**
 * @description: 二分查找，返回字符串在保留字表中的位置
 * @param {const string} word
 * @return {*}字符串在保留字表中的位置，若不是保留字返回-1
 */
int getReservedWordsIndex(const string word){
    int index = lower_bound(reservedWords, reservedWords + WORDS_LEN, word) - reservedWords;
    if (index < WORDS_LEN)index = reservedWords[index] == word ? index : -1;
    return index;
}
int main(int arc, const char *argv[]) {
    ifstream inFile;
    // 默认读取test.c
    // if (arc == 2){
    //     inFile.open(argv[1], ios::in);
    // }else {
    //     inFile.open("./test.c", ios::in);
    // }
    inFile.open("D:\\my_cpp_workspace\\compilers\\test.c", ios::in);
    if (!inFile.is_open()) {
        cout << "读取指定文件失败" << endl;
        return 0;
    }
    char lineBuff[4096];
    while (inFile.getline(lineBuff, sizeof lineBuff)) {
        cout << rowNum++ << "\t" << lineBuff << endl;
    }

    inFile.clear();
    inFile.close();
    return 0;
}