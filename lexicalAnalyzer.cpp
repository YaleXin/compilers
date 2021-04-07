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
#include "tempStructs.h"
using namespace std;
class LexicalAnalyzer{
    const bool SAVE_TO_FILE = true;
    vector<Result> ansSet;
    // 保留字 有序列表，方便于折半查找
    const string reservedWords[32] = {
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
    const int CHAR_CONSTANTS_ID = 35;
    const int STRING_CONSTANTS_ID = 36;
    const int DELIMITER_ID = 37;
    // 标识符类型
    enum IdentifyType { T_CHAR, T_SHORT, T_INT, T_LONG, LONG_LONG, T_DOUBLE, T_FLOAT, T_ARRAY, T_FUNCTION };
    // 动态生成的常量表
    vector<int>intConstants;
    vector<double>doubleConstants;
    vector<string>stringConstants;
    // 界符
    const string delimiters[30] = {
        ",",  ";", "(",  ")",  "{",  "}",   "[", "]", "<", "<<",
        "<=", ">", ">>", ">=", "==", "!==", "=", "~", "!", "?",
        ":",  "|", "||", "&",  "&&", "+",   "-", "*", "/", "%"};
    const char dlmt[23] = ",;(){}[]<>=!~?:|&+-*/%";
    char lineBuff[4096];
    int rowNum = 0, colNum = 0;
    vector<string> identifyTable;
    bool matching = false;
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
     * @description: 返回指定字符在 delimiters 表中的位置
     * @param {const string} c 待查找字符
     * @return {*} 指定字符在 delimiters 表中的位置，若不存在，返回-1
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
     * @description: 查询指定变量在变量表中的位置
     * @param {const string} identify
     * @return {*} 指定变量在变量表中的位置，不存在时返回-1
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
    // 预处理
    void preprocess() {}
    /**
     * @description: 二分查找，返回字符串在保留字表中的位置
     * @param {const string} word
     * @return {*}字符串在保留字表中的位置，若不是保留字返回-1
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
        // 不是保留字
        if (index == -1) {
            // 获取内码
            index = getIdentifyIndex(identify);
            // 新的变量
            if (index == -1) {
                identifyTable.push_back(identify);
                index = identifyTable.size() - 1;
            }
            ansSet.push_back(Result(identify, IDENTIFY_ID, index));
        } else {
            // 是保留字
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
            delimiter = "]";
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
    int escapeCharacter2ascll(char c){
        int ascllCode;
        switch(c){
            case 'o':
                ascllCode = 0;
                break;
            case 'n':
                ascllCode = 10;
                break;
            case 'r':
                ascllCode = 13;
                break;
            case 't':
                ascllCode = 9;
                break;
            case 'v':
                ascllCode = 11;
                break;
            case 'a':
                ascllCode = 7;
                break;
            case 'b':
                ascllCode = 8;
                break;
            case 'f':
                ascllCode = 12;
                break;
            case '\'':
                ascllCode = 39;
                break;
            case '\"':
                ascllCode = 34;
                break;
            case '\\':
                ascllCode = 92;
                break;
            case '\?':
                ascllCode = 63;
                break;
            default:
                ascllCode = -1;
        }
        return ascllCode;
    }
    int tryReadChar(){
        int index = colNum++;
        int ascllCode = -1;
        // 带转义字符的
        if (lineBuff[colNum] == '\\'){
            colNum++;
            ascllCode = escapeCharacter2ascll(lineBuff[colNum++]);
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
    string tryReadString(){
        int index = colNum++;
        string readString = "";
        while(lineBuff[colNum] != '\0'){
            if (lineBuff[colNum] == '\\'){
                colNum++;
                int escapeCharacterCode = escapeCharacter2ascll(lineBuff[colNum]);
                if (escapeCharacterCode == -1){
                    matching = false;
                    colNum = index;
                    return "";
                }
                string char2string(1, escapeCharacterCode);
                readString += char2string;
            } else if (lineBuff[colNum] == '\"'){
                matching = true;
                colNum++;
                break;
            } else {
                string char2string(1, lineBuff[colNum]);
                readString += char2string;
            }
            colNum++;
        }
        return readString;
    }
    bool handleDelimiter(const string delimiter){
        int index = getDelimiterIndex(delimiter);
        if (index == -1)return false;
        ansSet.push_back(Result(delimiter, DELIMITER_ID + index));
        return true;
    }
    /**
     * @description: 将读取到的数字常量保存起来，方便起见，这里只有int类型和double类型
     * @param {const double} num
     * @return 
     */
    void handleNumber(const double num){
        if ((int)num == num){
            ansSet.push_back(Result(to_string((int)num), INTEGER_CONSTANTS_ID, intConstants.size() - 1));
            intConstants.push_back(num);
        } else {
            ansSet.push_back(Result(to_string(num), REAL_CONSTANTS_ID, doubleConstants.size() - 1));
            doubleConstants.push_back(num);
        }
    }
    void handleChar(const int ascllCode){
        string char2string(1, ascllCode);
        ansSet.push_back(Result(char2string, CHAR_CONSTANTS_ID, ascllCode));
    }
    void handleString(const string str){
        ansSet.push_back(Result(str, STRING_CONSTANTS_ID, stringConstants.size()));
        stringConstants.push_back(str);
    }
    void print(){
        int len = ansSet.size();
        for (int i = 0 ; i < len; i++)
            printf("<\t%-20s,%02d,%02d\t>\n", ansSet[i].word.c_str(), ansSet[i].identifyId, ansSet[i].internalCode);
        printf("\n----------------\n");
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
        outFile.open("intConstants.txt", ios::out | ios::trunc);
        if (!outFile.is_open()) {
            cout << "failed to read file" << endl;
            return ;
        }
        int size = intConstants.size();
        for (int i = 0; i < size; i++) outFile << intConstants[i] << endl;
        outFile.clear();
        outFile.close();

        outFile.open("doubleConstants.txt", ios::out | ios::trunc);
        if (!outFile.is_open()) {
            cout << "failed to read file" << endl;
            return ;
        }
        size = doubleConstants.size();
        for (int i = 0; i < size; i++) outFile << doubleConstants[i] << endl;
        outFile.clear();
        outFile.close();
    }
    void saveStringConstant(){
        ofstream outFile;
        outFile.open("stringConstants.txt", ios::out | ios::trunc);
        if (!outFile.is_open()) {
            cout << "failed to read file" << endl;
            return ;
        }
        int size = stringConstants.size();
        for (int i = 0; i < size; i++) outFile << stringConstants[i] << endl;
        outFile.clear();
        outFile.close();
    }
    void saveToFile(){
        saveIdentify();
        saveNumberConstant();
        saveStringConstant();
    }
public:
    LexAnalyzerResult main(string filename="D:\\my_cpp_workspace\\compilers\\test.c") {
        LexAnalyzerResult rsl;
        tryReadString();
        ifstream inFile;
        // 默认读取test.c
        inFile.open(filename, ios::in);
        if (!inFile.is_open()) {
            cout << "failed to read file" << endl;
            return rsl;
        }
        string wordBuff;
        while (inFile.getline(lineBuff, sizeof lineBuff)) {
            colNum = 0, matching = false;
            while (lineBuff[colNum] != '\0') {
                if (isLetter(lineBuff[colNum]) || lineBuff[colNum] == '_') {
                    wordBuff = tryReadIdentify();
                    if (matching){
                        handleMatchingIdentify(wordBuff);
                    } else {
                        error("idetify analyze failed.");
                        return rsl;
                    }
                } else if (isDelimiter(lineBuff[colNum])) {
                    wordBuff = tryReadDelimiter();
                    if (!handleDelimiter(wordBuff)){
                        error("delimiter analyze failed.");
                        return rsl;
                    }
                }else if (isDigital(lineBuff[colNum])){
                    double numBuff = tryReadNumber();
                    if (matching)handleNumber(numBuff);
                    else {
                        error("number analyze failed.");
                        return rsl;
                    }
                }else if (lineBuff[colNum] == '\''){
                    int ascllCode = tryReadChar();
                    if (matching && ascllCode != -1){
                        handleChar(ascllCode);
                    } else {
                        error("char analyze failed.");
                        return rsl;
                    }
                }else if(lineBuff[colNum] == '\"'){
                    wordBuff = tryReadString();
                    if (matching)handleString(wordBuff);
                    else{
                        error("string analyze failed.");
                        return rsl;
                    }
                }else if(isBlank(lineBuff[colNum]))colNum++;
            }
            rowNum++;
        }
        print();
        if (SAVE_TO_FILE)saveToFile();
        inFile.clear();
        inFile.close();
        rsl.ansSet = ansSet, rsl.doubleConstants = doubleConstants, 
        rsl.identifyTable = identifyTable, rsl.intConstants = intConstants,
        rsl.stringConstants = stringConstants;
        return rsl;
    }
};