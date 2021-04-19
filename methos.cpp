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

#include "tempStructs.h"
using namespace std;

vector<Result> ansSet;

// 动态生成的常量表
vector<int> intConstants;
vector<double> doubleConstants;
vector<string> stringConstants;

vector<string> identifyTable;

// 存放一些可能用得到的东西

void print() {
    int len = ansSet.size();
    for (int i = 0; i < len; i++)
        printf("<\t%-20s,%02d,%02d\t>\n", ansSet[i].word.c_str(),
               ansSet[i].identifyId, ansSet[i].internalCode);
    printf("\n----------------\n");
}

void saveIdentify() {
    ofstream outFile;
    outFile.open("identify.txt", ios::out | ios::trunc);
    if (!outFile.is_open()) {
        cout << "failed to read file" << endl;
        return;
    }
    int size = identifyTable.size();
    for (int i = 0; i < size; i++) outFile << identifyTable[i] << endl;
    outFile.clear();
    outFile.close();
}
void saveNumberConstant() {
    ofstream outFile;
    outFile.open("intConstants.txt", ios::out | ios::trunc);
    if (!outFile.is_open()) {
        cout << "failed to read file" << endl;
        return;
    }
    int size = intConstants.size();
    for (int i = 0; i < size; i++) outFile << intConstants[i] << endl;
    outFile.clear();
    outFile.close();

    outFile.open("doubleConstants.txt", ios::out | ios::trunc);
    if (!outFile.is_open()) {
        cout << "failed to read file" << endl;
        return;
    }
    size = doubleConstants.size();
    for (int i = 0; i < size; i++) outFile << doubleConstants[i] << endl;
    outFile.clear();
    outFile.close();
}
void saveStringConstant() {
    ofstream outFile;
    outFile.open("stringConstants.txt", ios::out | ios::trunc);
    if (!outFile.is_open()) {
        cout << "failed to read file" << endl;
        return;
    }
    int size = stringConstants.size();
    for (int i = 0; i < size; i++) outFile << stringConstants[i] << endl;
    outFile.clear();
    outFile.close();
}
void saveToFile() {
    saveIdentify();
    saveNumberConstant();
    saveStringConstant();
}
int main(int argc, char const *argv[]) { return 0; }
