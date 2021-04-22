/*
 * @Author      : YaleXin
 * @Email       : 181303209@yzu.edu.cn
 * @LastEditors : YaleXin
 */
#include <malloc.h>

#include <algorithm>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "tempStructs.h"
using namespace std;
class Lex {
    ifstream inFile;
    const int EOF_ID = -2021;
    // ������ �����б��������۰����
    const string reservedWords[32] = {
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

    // ���
    const string delimiters[30] = {
        ",",  ";", "(",  ")",  "{",  "}",   "[", "]", "<", "<<",
        "<=", ">", ">>", ">=", "==", "!==", "=", "~", "!", "?",
        ":",  "|", "||", "&",  "&&", "+",   "-", "*", "/", "%"};
    const char dlmt[23] = ",;(){}[]<>=!~?:|&+-*/%";

    // ��̬���ɵĳ�����
    vector<int> intConstants;
    vector<double> doubleConstants;
    vector<string> stringConstants;

    vector<string> identifyTable;

    char lineBuff[4096];
    int rowNum = 0, colNum = 0;

    bool matching = false;
    void error(string errorMsg) {
        cout << "lexical analyze error!, because is : " << errorMsg
             << "in row: " << (rowNum + 1) << ", column: " << (colNum + 1)
             << endl;
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
    bool isBlank(const char c) { return c == '\t' || c == ' '; }
    /**
     * @description: ����ָ���ַ��� delimiters ���е�λ��
     * @param {const string} c �������ַ�
     * @return {*} ָ���ַ��� delimiters ���е�λ�ã��������ڣ�����-1
     */
    int getDelimiterIndex(const string c) {
        int index = -1;
        for (int i = 0; i <= 29; i++) {
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
        int index =
            lower_bound(reservedWords, reservedWords + WORDS_LEN, word) -
            reservedWords;
        if (index < WORDS_LEN)
            index = reservedWords[index] == word ? index : -1;
        return index;
    }
    string tryReadIdentify() {
        int index = colNum;
        string buff = "";
        while (lineBuff[colNum] == '_' || isLetter(lineBuff[colNum]) ||
               isDigital(lineBuff[colNum])) {
            buff += lineBuff[colNum++];
        }
        if (isDelimiter(lineBuff[colNum]) || isBlank(lineBuff[colNum]) || lineBuff[colNum] == '\0') {
            matching = true;
        } else {
            colNum = index;
            matching = false;
            buff = "";
        }
        return buff;
    };
    void handleMatchingIdentify(string identify, Result &rst) {
        rst.word = identify;
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
            rst.identifyId = IDENTIFY_ID, rst.internalCode = index;
        } else {
            // �Ǳ�����
            rst.identifyId = index;
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
            if (lineBuff[colNum + 1] == '=')
                delimiter = "!=", colNum++;
            else
                delimiter = "!";
        } else if (lineBuff[colNum] == '=') {
            if (lineBuff[colNum + 1] == '=')
                delimiter = "==", colNum++;
            else
                delimiter = "=";
        } else if (lineBuff[colNum] == '~') {
            delimiter = "~";
        } else if (lineBuff[colNum] == '<') {
            if (lineBuff[colNum + 1] == '=')
                delimiter = "<=", colNum++;
            else if (lineBuff[colNum + 1] == '<')
                delimiter = "<<", colNum++;
            else
                delimiter = "<";
        } else if (lineBuff[colNum] == '>') {
            if (lineBuff[colNum + 1] == '=')
                delimiter = ">=", colNum++;
            else if (lineBuff[colNum + 1] == '>')
                delimiter = ">>", colNum++;
            else
                delimiter = ">";
        } else if (lineBuff[colNum] == '&') {
            if (lineBuff[colNum + 1] == '&')
                delimiter = "&&", colNum++;
            else
                delimiter = "&";
        } else if (lineBuff[colNum] == '|') {
            if (lineBuff[colNum + 1] == '|')
                delimiter = "||", colNum++;
            else
                delimiter = "|";
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
    double tryReadNumber() {
        int index = colNum;
        double sum = 0;
        while (isDigital(lineBuff[colNum])) {
            sum = sum * 10 + lineBuff[colNum] - '0';
            colNum++;
        }
        // ������
        if (lineBuff[colNum] == '.') {
            if (colNum == 0 || !isDigital(lineBuff[colNum - 1])) {
                matching = false;
                colNum = index;
                return 0.0;
            } else {
                colNum++;
                if (isDigital(lineBuff[colNum])) {
                    double base = 0.1;
                    while (isDigital(lineBuff[colNum])) {
                        sum += (lineBuff[colNum] - '0') * base;
                        colNum++;
                        base /= 10;
                    }
                } else {
                    matching = false;
                    colNum = index;
                    return 0.0;
                }
            }
        }
        // ���ֽ�����ʱ������ǽ���������� (, {, [
        if (isBlank(lineBuff[colNum]) || lineBuff[colNum] == '\0' || (isDelimiter(lineBuff[colNum]) && lineBuff[colNum] != '(' &&
            lineBuff[colNum] != '{' && lineBuff[colNum] != '[')) {
            matching = true;
        } else {
            matching = false;
        }
        return sum;
    }
    int escapeCharacter2ascll(char c) {
        int ascllCode;
        switch (c) {
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
    int tryReadChar() {
        int index = colNum++;
        int ascllCode = -1;
        // ��ת���ַ���
        if (lineBuff[colNum] == '\\') {
            colNum++;
            ascllCode = escapeCharacter2ascll(lineBuff[colNum++]);
        } else {
            ascllCode = lineBuff[colNum++];
        }
        if (lineBuff[colNum++] == '\'') {
            matching = true;
        } else {
            ascllCode = -1;
            matching = false;
            colNum = index;
        }
        return ascllCode;
    }
    string tryReadString() {
        int index = colNum++;
        string readString = "";
        while (lineBuff[colNum] != '\0') {
            if (lineBuff[colNum] == '\\') {
                colNum++;
                int escapeCharacterCode =
                    escapeCharacter2ascll(lineBuff[colNum]);
                if (escapeCharacterCode == -1) {
                    matching = false;
                    colNum = index;
                    return "";
                }
                string char2string(1, escapeCharacterCode);
                readString += char2string;
            } else if (lineBuff[colNum] == '\"') {
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
    bool handleDelimiter(const string delimiter, Result &rst) {
        int index = getDelimiterIndex(delimiter);
        rst.word = delimiter, rst.identifyId = DELIMITER_ID + index;
        return true;
    }
    /**
     * @description:
     * ����ȡ�������ֳ��������������������������ֻ��int���ͺ�double����
     * @param {const double} num
     * @return
     */
    void handleNumber(const double num, Result &rst) {
        if ((int)num == num) {
            rst.word = to_string((int)num),
            rst.identifyId = INTEGER_CONSTANTS_ID,
            rst.internalCode = intConstants.size();
            intConstants.push_back(num);
        } else {
            rst.word = to_string(num), rst.identifyId = REAL_CONSTANTS_ID,
            rst.internalCode = doubleConstants.size();
            doubleConstants.push_back(num);
        }
    }
    void handleChar(const int ascllCode, Result &rst) {
        string char2string(1, ascllCode);
        rst.word = char2string, rst.identifyId = CHAR_CONSTANTS_ID,
        rst.internalCode = ascllCode;
    }
    void handleString(const string str, Result &rst) {
        rst.word = str, rst.identifyId = STRING_CONSTANTS_ID,
        rst.internalCode = stringConstants.size();
        stringConstants.push_back(str);
    }

   public:
    Lex(string filename, bool &status) {
        cout << "object init...." << endl;
        inFile.open(filename, ios::in);
        if (!inFile.is_open()) {
            cout << "failed to read file" << endl;
            status = false;
        } else {
            status = true;
            inFile.getline(lineBuff, sizeof lineBuff);
        }
    }
    Result getWord(int &line, int &col) {
        // �Ƿ��ȡ������Ч�ַ�
        bool flag = false;
        string wordBuff;
        Result rst("", -1);
        while (!flag) {
            if (isLetter(lineBuff[colNum]) || lineBuff[colNum] == '_') {
                wordBuff = tryReadIdentify();
                if (matching) {
                    handleMatchingIdentify(wordBuff, rst);
                } else {
                    error("idetify analyze failed.");
                }
                flag = true;
            } else if (isDelimiter(lineBuff[colNum])) {
                wordBuff = tryReadDelimiter();
                handleDelimiter(wordBuff, rst);
                flag = true;
            } else if (isDigital(lineBuff[colNum])) {
                double numBuff = tryReadNumber();
                if (matching)
                    handleNumber(numBuff, rst);
                else {
                    error("number analyze failed.");
                }
                flag = true;
            } else if (lineBuff[colNum] == '\'') {
                int ascllCode = tryReadChar();
                if (matching && ascllCode != -1) {
                    handleChar(ascllCode, rst);
                } else {
                    error("char analyze failed.");
                }
                flag = true;
            } else if (lineBuff[colNum] == '\"') {
                wordBuff = tryReadString();
                if (matching)
                    handleString(wordBuff, rst);
                else {
                    error("string analyze failed.");
                }
                flag = true;
            } else if (isBlank(lineBuff[colNum]) && !flag) {
                colNum++;
            } else if (lineBuff[colNum] == '\0' && !flag) {
                inFile.getline(lineBuff, sizeof lineBuff);
                colNum = 0;
                rowNum++;
                if (inFile.eof()) {
                    // �����ļ���ȡ����
                    rst.identifyId = EOF_ID;
                    inFile.clear();
                    inFile.close();
                    break;
                }
            }
        }
        line = rowNum, col = colNum;
        return rst;
    }
};