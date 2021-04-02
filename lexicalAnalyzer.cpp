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
    // 识别的单词或符号
    string word;
    // 类别号，内码
    int identifyId, internalCode;
    Result(string wd, int idtfid, int incode = -1){ word = wd, identifyId = idtfid, internalCode = incode;}
};
vector<Result>ansSet;
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
 * @description: 返回指定字符在 delimiter 表中的位置
 * @param {const string} c 待查找字符
 * @return {*} 指定字符在 delimiter 表中的位置，若不存在，返回-1
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
 * @description: 查询指定变量在变量表中的位置
 * @param {const string} identify
 * @return {*} 指定变量在变量表中的位置，不存在时返回-1
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
    // 不是保留字
    if (index == -1){
        // 获取内码
        index = getIdentifyIndex(identify);
        // 新的变量
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
    // 默认读取test.c
    if (arc == 2){
        inFile.open(argv[1], ios::in);
    }else {
        inFile.open("D:\\my_cpp_workspace\\compilers\\test.c", ios::in);
    }
    if (!inFile.is_open()) {
        cout << "读取指定文件失败" << endl;
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