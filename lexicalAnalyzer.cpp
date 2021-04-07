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
#include <malloc.h>
using namespace std;
const bool SAVE_TO_FILE = true;
struct Result {
    // ʶ��ĵ��ʻ����
    string word;
    // ���ţ�����
    int identifyId, internalCode;
    Result(string wd, int idtfid, int incode = -1) {
        word = wd, identifyId = idtfid, internalCode = incode;
    }
};
vector<Result> ansSet;
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
const int CHAR_CONSTANTS_ID = 35;
const int STRING_CONSTANTS_ID = 36;
const int DELIMITER_ID = 37;
// ��ʶ������
enum IdentifyType { T_CHAR, T_SHORT, T_INT, T_LONG, LONG_LONG, T_DOUBLE, T_FLOAT, T_ARRAY, T_FUNCTION };
// ��̬���ɵ����ֳ�����
struct DynamicNumber{
    void *content;
    IdentifyType type;
    DynamicNumber(IdentifyType t, double num){
        type = t;
        switch(t){
            case T_INT:
                content = malloc(sizeof (int));
                *((int *)content) = num;
                break;
            case T_DOUBLE:
                content = malloc(sizeof (double));
                *((double *)content) = num;
                break;
            default: 
            content = nullptr;
        }
    }
};
vector<DynamicNumber> dynamicNumbers;
// ���
const string delimiters[] = {
    ",",  ";", "(",  ")",  "{",  "}",   "[", "]", "<", "<<",
    "<=", ">", ">>", ">=", "==", "!==", "=", "~", "!", "?",
    ":",  "|", "||", "&",  "&&", "+",   "-", "*", "/", "%"};
const char dlmt[] = ",;(){}[]<>=!~?:|&+-*/%";
char lineBuff[4096];
int rowNum, colNum;
vector<string> identifyTable;
vector<string> constNumberTable;
bool matching;
void error(string errorMsg) {
    cout << "lexical analyze error!, because is : "<< errorMsg <<
        "in row: " << (rowNum + 1)<<
        ", column: " << (colNum + 1) << endl;
}
bool isDelimiter(const char c) {
    for (int i = (sizeof dlmt) / (sizeof 'a') - 2; i >= 0; i--)
        if (c == dlmt[i]) return true;
    return false;
}
bool isDigital(const char c) { return c >= '0' && c <= '9'; }
bool isLetter(const char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}
bool isBlank(const char c) { return c == '\n' || c == '\t' || c == ' '; }
/**
 * @description: ����ָ���ַ��� delimiters ���е�λ��
 * @param {const string} c �������ַ�
 * @return {*} ָ���ַ��� delimiters ���е�λ�ã��������ڣ�����-1
 */
int getDelimiterIndex(const string c) {
    int index = -1;
    for (int i = 0; i <= 27; i++) {
        if (delimiters[i] == c) {
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
int getIdentifyIndex(const string identify) {
    int index = -1;
    for (int i = 0; i < identifyTable.size(); i++) {
        if (identifyTable[i] == identify) {
            index = i;
            break;
        }
    }
    return index;
}
// Ԥ����
void preprocess() {}
/**
 * @description: ���ֲ��ң������ַ����ڱ����ֱ��е�λ��
 * @param {const string} word
 * @return {*}�ַ����ڱ����ֱ��е�λ�ã������Ǳ����ַ���-1
 */
int getReservedWordsIndex(const string word) {
    int index = lower_bound(reservedWords, reservedWords + WORDS_LEN, word) -
                reservedWords;
    if (index < WORDS_LEN) index = reservedWords[index] == word ? index : -1;
    return index;
}
string tryReadIdentify() {
    int index = colNum;
    string buff = "";
    while (lineBuff[colNum] == '_' || isLetter(lineBuff[colNum])) {
        buff += lineBuff[colNum++];
    }
    while (lineBuff[colNum] == '_' || isLetter(lineBuff[colNum]) ||
           isDigital(lineBuff[colNum])) {
        buff += lineBuff[colNum++];
    }
    if (isDelimiter(lineBuff[colNum]) || isBlank(lineBuff[colNum])) {
        matching = true;
    } else {
        colNum = index;
        matching = false;
        buff = "";
    }
    return buff;
};
void handleMatchingIdentify(string identify) {
    int index = getReservedWordsIndex(identify);
    // ���Ǳ�����
    if (index == -1) {
        // ��ȡ����
        index = getIdentifyIndex(identify);
        // �µı���
        if (index == -1) {
            identifyTable.push_back(identify);
            index = identifyTable.size() - 1;
        }
        ansSet.push_back(Result(identify, IDENTIFY_ID, index));
    } else {
        // �Ǳ�����
        ansSet.push_back(Result(identify, index));
    }
}
string tryReadDelimiter() {
    int index = colNum;
    string delimiter = "";
    if (lineBuff[colNum] == ',') {
        delimiter = ",";
    } else if (lineBuff[colNum] == ';') {
        delimiter = ";";
    } else if (lineBuff[colNum] == '(') {
        delimiter = "(";
    } else if (lineBuff[colNum] == ')') {
        delimiter = ")";
    } else if (lineBuff[colNum] == '{') {
        delimiter = "{";
    } else if (lineBuff[colNum] == '}') {
        delimiter = "}";
    } else if (lineBuff[colNum] == '[') {
        delimiter = "[";
    } else if (lineBuff[colNum] == ']') {
        delimiter = "[";
    } else if (lineBuff[colNum] == '!') {
        if (lineBuff[colNum + 1] == '=') delimiter = "!=", colNum++;
        else delimiter = "!";
    } else if (lineBuff[colNum] == '=') {
        if (lineBuff[colNum + 1] == '=') delimiter = "==",colNum++;
        else delimiter = "=";
    } else if (lineBuff[colNum] == '~') {
        delimiter = "~";
    } else if (lineBuff[colNum] == '<') {
        if (lineBuff[colNum + 1] == '=')delimiter = "<=", colNum++;
        else if (lineBuff[colNum + 1] == '<')delimiter = "<<", colNum++;
        else delimiter = "<";
    } else if (lineBuff[colNum] == '>') {
        if (lineBuff[colNum + 1] == '=')delimiter = ">=", colNum++;
        else if (lineBuff[colNum + 1] == '>')delimiter = ">>", colNum++;
        else delimiter = ">";
    } else if (lineBuff[colNum] == '&') {
        if (lineBuff[colNum + 1] == '&') 
            delimiter = "&&", colNum++;
         else delimiter = "&";
    } else if (lineBuff[colNum] == '|') {
        if (lineBuff[colNum + 1] == '|') delimiter = "||", colNum++;
        else delimiter = "|";
    } else if (lineBuff[colNum] == '+') {
        delimiter = "+";
    } else if (lineBuff[colNum] == '-') {
        delimiter = "-";
    } else if (lineBuff[colNum] == '*') {
        delimiter = "*";
    } else if (lineBuff[colNum] == '/') {
        delimiter = "/";
    } else if (lineBuff[colNum] == '%') {
        delimiter = "%";
    }
    matching = true;
    colNum++;
    return delimiter;
}
double tryReadNumber(){
    int index = colNum;
    double sum = 0;
    bool positive = false;
    if (lineBuff[colNum] == '-'){
        positive = true;
        colNum++;
    } else if (lineBuff[colNum] == '+'){
        colNum++;
    }
    while(isDigital(lineBuff[colNum])){
        sum = sum * 10 + lineBuff[colNum] - '0';
        colNum++;
    }
    if (lineBuff[colNum] == '.'){
        if (colNum == 0 || !isDigital(lineBuff[colNum - 1])){
            matching = false;  
            colNum = index;
        } else{       
            colNum++;
            if (isDigital(lineBuff[colNum])){
                double base = 0.1;
                while(isDigital(lineBuff[colNum])){
                    sum += (lineBuff[colNum] - '0') * base;
                    colNum++;
                    base /= 10;
                }
                matching = true;            
            }else {
                matching = false;  
                colNum = index;
            }
        }
    } else {
        matching = true;
    }
    return sum;
}
int tryReadChar(){
    int index = colNum++;
    int ascllCode = -1;
    // ��ת���ַ���
    if (lineBuff[colNum] == '\\'){
        colNum++;
        switch(lineBuff[colNum++]){
            case 'o':
                ascllCode = 0;
                break;
            case 'n':
                ascllCode = 0;
                break;
            case 'r':
                ascllCode = 0;
                break;
            case 't':
                ascllCode = 0;
                break;
            case 'v':
                ascllCode = 0;
                break;
            case 'a':
                ascllCode = 0;
                break;
            case 'b':
                ascllCode = 0;
                break;
            case 'f':
                ascllCode = 0;
                break;
            case '\'':
                ascllCode = 0;
                break;
            case '\"':
                ascllCode = 0;
                break;
            case '\\':
                ascllCode = 0;
                break;
            case '\?':
                ascllCode = 0;
                break;
            default:
                ascllCode = -1;
        }
    } else {
        ascllCode = lineBuff[colNum++];
    }
    if (lineBuff[colNum++] == '\''){
            matching = true;
    } else {
        ascllCode = -1;
        matching = false;
        colNum = index;
    }
    return ascllCode;
}
bool handleDelimiter(const string delimiter){
    int index = getDelimiterIndex(delimiter);
    if (index == -1)return false;
    ansSet.push_back(Result(delimiter, DELIMITER_ID + index));
    return true;
}
/**
 * @description: ����ȡ�������ֳ��������������������������ֻ��int���ͺ�double����
 * @param {const double} num
 * @return 
 */
void handleNumber(const double num){
    if ((int)num == num){
        DynamicNumber d(T_INT, num);
        dynamicNumbers.push_back(d);
        ansSet.push_back(Result(to_string((int)num), INTEGER_CONSTANTS_ID, dynamicNumbers.size() - 1));
    } else {
        DynamicNumber d(T_DOUBLE, num);
        dynamicNumbers.push_back(d);
        ansSet.push_back(Result(to_string(num), REAL_CONSTANTS_ID, dynamicNumbers.size() - 1));
    }
}
void handleChar(const int ascllCode){
    string char2string(1, ascllCode);
    ansSet.push_back(Result(char2string, CHAR_CONSTANTS_ID, ascllCode));
}
void print(){
    int len = ansSet.size();
    for (int i = 0 ; i < len; i++)
        cout << "(\t" << ansSet[i].word << ", " << 
        ansSet[i].identifyId << ", " << 
        ansSet[i].internalCode << 
        "\t)" <<endl;
}
void saveIdentify(){
    ofstream outFile;
    outFile.open("identify.txt", ios::out | ios::trunc);
    if (!outFile.is_open()) {
        cout << "failed to read file" << endl;
        return ;
    }
    int size = identifyTable.size();
    for (int i = 0; i < size; i++)outFile<< identifyTable[i] <<endl;
    outFile.clear();
    outFile.close();
}
void saveNumberConstant(){
    ofstream outFile;
    outFile.open("number.txt", ios::out | ios::trunc);
    if (!outFile.is_open()) {
        cout << "failed to read file" << endl;
        return ;
    }
    int size = dynamicNumbers.size();
    for (int i = 0; i < size; i++)
        if (dynamicNumbers[i].type == T_INT){
            cout<< *((int *) dynamicNumbers[i].content) <<endl;
            outFile<< *((int *) dynamicNumbers[i].content) <<endl;
        } else if (dynamicNumbers[i].type == T_DOUBLE){
            outFile<< *((double *) dynamicNumbers[i].content) <<endl;
            cout<< *((double *) dynamicNumbers[i].content) <<endl;
        } else {
            outFile << "unknown-type number" << endl;
        }
    outFile.clear();
    outFile.close();
}
void saveToFile(){
    saveIdentify();
    saveNumberConstant();
}
int main(int arc, const char *argv[]) {
    ifstream inFile;
    // Ĭ�϶�ȡtest.c
    if (arc == 2) {
        inFile.open(argv[1], ios::in);
    } else {
        inFile.open("D:\\my_cpp_workspace\\compilers\\test.c", ios::in);
    }
    if (!inFile.is_open()) {
        cout << "failed to read file" << endl;
        return 0;
    }
    string wordBuff;
    while (inFile.getline(lineBuff, sizeof lineBuff)) {
        // cout << rowNum++ << "\t" << lineBuff << endl;
        colNum = 0, matching = false;
        while (lineBuff[colNum] != '\0') {
            if (isLetter(lineBuff[colNum]) || lineBuff[colNum] == '_') {
                wordBuff = tryReadIdentify();
                if (matching){
                    handleMatchingIdentify(wordBuff);
                } else {
                    error("idetify analyze failed.");
                    return 0;
                }
            } else if (isDelimiter(lineBuff[colNum])) {
                wordBuff = tryReadDelimiter();
                if (!handleDelimiter(wordBuff)){
                    error("delimiter analyze failed.");
                    return 0;
                }
            }else if (isDigital(lineBuff[colNum])){
                double numBuff = tryReadNumber();
                if (matching)handleNumber(numBuff);
                else {
                    error("number analyze failed.");
                    return 0;
                }
            }else if (lineBuff[colNum] == '\''){
                int ascllCode = tryReadChar();
                if (matching && ascllCode != -1){
                    handleChar(ascllCode);
                } else {
                    error("char analyze failed.");
                    return 0;
                }
            }else if(isBlank(lineBuff[colNum]))colNum++;
        }
        rowNum++;
    }
    print();
    if (SAVE_TO_FILE)saveToFile();
    inFile.clear();
    inFile.close();
    return 0;
}