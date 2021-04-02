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
    // ʶ��ĵ��ʻ����
    string word;
    // ���ţ�����
    int identifyId, internalCode;
    Result(string wd, int idtfid, int incode = -1){ word = wd, identifyId = idtfid, internalCode = incode;}
};
vector<Result>ansSet;
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
const char dlmt[] = ",;(){}[]<>=!~?:|&+-*/%";
void error() {}
char lineBuff[4096];
int rowNum, culNum;
vector<string>identifyTable;
vector<string>constNumberTable;
bool matching;
bool isDelimiter(const char c){
    for (int i = (sizeof dlmt) / (sizeof 'a'); i >= 0;i++)
        if (c == dlmt[i])return true;
    return false;
}
bool isDigital(const char c){ return c >= '0' && c <= '9';}
bool isLetter(const char c){ return (c >= 'a' && c <='z') || (c >= 'A' && c <='Z');}
bool isBlank(const char c) { return c == '\n' || c == '\t' || c == ' ';}
/**
 * @description: ����ָ���ַ��� delimiter ���е�λ��
 * @param {const string} c �������ַ�
 * @return {*} ָ���ַ��� delimiter ���е�λ�ã��������ڣ�����-1
 */
int getDelimiterIndex(const string c){ 
    int index = -1;
    for(int i = 0; i <= 27; i++){
        if (delimiter[i] == c){
            index = i;
            break;
        }
    }
    return index;
}
/**
 * @description: ��ѯָ�������ڱ������е�λ��
 * @param {const string} identify
 * @return {*} ָ�������ڱ������е�λ�ã�������ʱ����-1
 */
int getIdentifyIndex(const string identify){
    int index = -1;
    for (int i = 0; i < identifyTable.size(); i++){
        if (identifyTable[i] == identify){
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
string tryReadIdentify(){
    int index = culNum;
    string buff = "";
    while(lineBuff[culNum] == '_' || isLetter(lineBuff[culNum])){
        buff += lineBuff[culNum++];
    }
    while(lineBuff[culNum] == '_' || isLetter(lineBuff[culNum]) || isDigital(lineBuff[culNum])){
        buff += lineBuff[culNum++];
    }
    if(isDelimiter(lineBuff[culNum]) || isBlank(lineBuff[culNum])){
        matching = true;
        culNum--;
    }else{
        culNum = index;
        matching = false;
        buff = "";
    }
    return buff;
};
void handleMatchingIdentify(string identify){
    int index = getReservedWordsIndex(identify);
    // ���Ǳ�����
    if (index == -1){
        // ��ȡ����
        index = getIdentifyIndex(identify);
        // �µı���
        if (index == -1){
            identifyTable.push_back(identify);
            index = identifyTable.size() - 1;
        }
        ansSet.push_back(Result(identify, IDENTIFY_ID, index));
    }else {
        ansSet.push_back(Result(identify, index));
    }
}
int main(int arc, const char *argv[]) {
    ifstream inFile;
    // Ĭ�϶�ȡtest.c
    if (arc == 2){
        inFile.open(argv[1], ios::in);
    }else {
        inFile.open("D:\\my_cpp_workspace\\compilers\\test.c", ios::in);
    }
    if (!inFile.is_open()) {
        cout << "��ȡָ���ļ�ʧ��" << endl;
        return 0;
    }
    string wordBuff;
    while (inFile.getline(lineBuff, sizeof lineBuff)) {
        // cout << rowNum++ << "\t" << lineBuff << endl;
        culNum = 0, matching = false;
        while(lineBuff[culNum] != '\0'){
            if (isLetter(lineBuff[culNum]) || lineBuff[culNum] == '_'){
                wordBuff = tryReadIdentify();
            }
        }
    }

    inFile.clear();
    inFile.close();
    return 0;
}