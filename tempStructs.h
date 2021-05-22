/*
 * @Author      : YaleXin
 * @Email       : 181303209@yzu.edu.cn
 * @LastEditors : YaleXin
 */
#ifndef __TEMPSTRUCTS_H__
#define __TEMPSTRUCTS_H__
#include <string>
#include <vector>
using namespace std;

struct Result {
    // 识别的单词或符号
    string word;
    // 类别号，内码
    int identifyId, internalCode;
    Result(string wd, int idtfid, int incode = -1) {
        word = wd, identifyId = idtfid, internalCode = incode;
    }
};
struct LexAnalyzerResult{
    vector<Result> ansSet;
    vector<string> identifyTable;
    vector<int>intConstants;
    vector<double>doubleConstants;
    vector<string>stringConstants;
};

// 标识符类型
enum IdentifyType {
    T_CHAR,
    T_SHORT,
    T_INT,
    T_LONG,
    LONG_LONG,
    T_DOUBLE,
    T_FLOAT,
    T_ARRAY,
    T_FUNCTION
};
// 四元式
struct Quaternion {
    int op, arg1, arg2, t;
};
#endif  