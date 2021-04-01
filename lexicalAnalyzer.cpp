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

// ������ �����б��������۰����
const string reservedWords[] = {
    "auto",     "break",  "case",    "char",   "const",    "continue",
    "default",  "do",     "double",  "else",   "enum",     "extern",
    "float",    "for",    "goto",    "if",     "int",      "long",
    "register", "return", "short",   "signed", "sizeof",   "static",
    "struct",   "switch", "typedef", "union",  "unsigned", "void",
    "volatile", "while",
};
const int WORDS_LEN = 32;
// ��ʶ�����
const int IDENTIFY_ID = 32;
const int INTEGER_CONSTANTS_ID = 33;
const int REAL_CONSTANTS_ID = 34;
const int STRING_CONSTANTS_ID = 35;
// ��ʶ������
enum IdentifyType { CHAR, SHORT, INT, LONG, LONG_LONG, ARRAY, FUNCTION };
// ���
const string delimiter[] = {
    ",", ";", "(", ")", "{", "}", "[",  "]", "<",  "<=", ">", ">=", "==", "!==",
    "=", "~", "!", "?", ":", "|", "||", "&", "&&", "+",  "-", "*",  "/",  "%"};
void error() {}
int rowNum, culNum;
bool isDigital(const char c){ return c >= '0' && c <= '9';}
bool isLetter(const char c){ return (c >= 'a' && c <='z') || (c >= 'A' && c <='Z');}
bool isBlank(const char c) { return c == '\n' || c == '\t' || c == ' ';}
/**
 * @description: ����ָ���ַ��� delimiter ���е�λ��
 * @param {const char} c �������ַ�
 * @return {*} ָ���ַ��� delimiter ���е�λ�ã��������ڣ�����-1
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
// Ԥ����
void preprocess(){}
/**
 * @description: ���ֲ��ң������ַ����ڱ����ֱ��е�λ��
 * @param {const string} word
 * @return {*}�ַ����ڱ����ֱ��е�λ�ã������Ǳ����ַ���-1
 */
int getReservedWordsIndex(const string word){
    int index = lower_bound(reservedWords, reservedWords + WORDS_LEN, word) - reservedWords;
    if (index < WORDS_LEN)index = reservedWords[index] == word ? index : -1;
    return index;
}
int main(int arc, const char *argv[]) {
    ifstream inFile;
    // Ĭ�϶�ȡtest.c
    // if (arc == 2){
    //     inFile.open(argv[1], ios::in);
    // }else {
    //     inFile.open("./test.c", ios::in);
    // }
    inFile.open("D:\\my_cpp_workspace\\compilers\\test.c", ios::in);
    if (!inFile.is_open()) {
        cout << "��ȡָ���ļ�ʧ��" << endl;
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