#ifndef __TEMPSTRUCTS_H__
#define __TEMPSTRUCTS_H__

#include <string>

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
    Result result;

};
#endif  