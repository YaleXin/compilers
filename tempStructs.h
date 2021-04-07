#ifndef __TEMPSTRUCTS_H__
#define __TEMPSTRUCTS_H__
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
struct LexAnalyzerResult{
    vector<Result> ansSet;
    vector<string> identifyTable;
    vector<int>intConstants;
    vector<double>doubleConstants;
    vector<string>stringConstants;
};
#endif  