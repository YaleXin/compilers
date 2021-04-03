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
const int STRING_CONSTANTS_ID = 35;
const int DELIMITER_ID = 36;
// ��ʶ������
enum IdentifyType { CHAR, SHORT, INT, LONG, LONG_LONG, ARRAY, FUNCTION };
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
        // colNum--;
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
    } else {
        matching = true;
    }
    return sum;
}
bool handleDelimiter(const string delimiter){
    int index = getDelimiterIndex(delimiter);
    if (index == -1)return false;
    ansSet.push_back(Result(delimiter, DELIMITER_ID + index));
    return true;
}
void handleNumber(const double num){
    
}
void print(){
    int len = ansSet.size();
    for (int i = 0 ; i < len; i++)
        cout << "(\t" << ansSet[i].word << ", " << 
        ansSet[i].identifyId << ", " << 
        ansSet[i].internalCode << 
        "\t)" <<endl;
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
        cout << "��ȡָ���ļ�ʧ��" << endl;
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
                }else{
                    error("idetify analyze failed.");
                    return 0;
                }
            } else if (isDelimiter(lineBuff[colNum])) {
                wordBuff = tryReadDelimiter();
                if (!handleDelimiter(wordBuff)){
                    error("delimiter analyze failed.");
                    return 0;
                }
            }else if (isBlank(lineBuff[colNum]))colNum++;
        }
    }
    print();
    inFile.clear();
    inFile.close();
    return 0;
}