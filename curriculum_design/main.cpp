/*
 * @Author      : YaleXin
 * @Email       : 181303209@yzu.edu.cn
 * @LastEditors : YaleXin
 */
#include <algorithm>
#include <cstdio>
#include <iostream>
#include <map>
#include <vector>
#include <stack>
#include "../constant.h"
#include "../lex.v2.cpp"
#include "../tempStructs.h"
#define DEBUG
using namespace std;
Result nowWord("", -1), copyWord("", -1), lastWord("", -1);
// 符号栈  状态栈 PLACE栈 TC栈 FC栈
vector<int> flagStk, stateStk, placeStk, TC_Stk, FC_Stk, chainStk;
// @line: 读头所在的行，@col: 读头所在的列 @NXQ: 下一条四元式的下标
int line, col, NXQ = 1;
typedef pair<int, int> P;

bool status;
typedef Quaternion Quat;
vector<Quat> quats;
Lex lex("D:\\my_cpp_workspace\\compilers\\curriculum_design\\test.cp", status);
int cnt = 0;
bool Program();
bool Block();
bool Stmts();
bool Stmt();
bool ifStmt();
bool whileStmt();
bool Type();
void printQuats();
P Bool();
bool Expr();
// 定义变量语句
void variableDefinitions() {
    // 实际上严格的编译器，在定义变量的时候是要完成好多事情的，但是为了简化工作，
    // 这里只需要完成查询变量的符号地址，填写进表格中，然而此项工作我放在了词法分析器中
    while (nowWord.identifyId != SEMIC) {
        nowWord = lex.getWord(line, col);
    }
}

int MERG(int p1, int p2) {
    int head = p1;
    if (p2 == 0) return head;
    int p = p2;
    while (quats[p].t != 0) {
        p = quats[p].t;
    }
    quats[p].t = p1;
    head = p2;
    return head;
}
void BACKPATCH(int r, int t) {
    int m, q = r;
    while (q != 0) {
        m = quats[q].t;
        quats[q].t = t;
        q = m;
    }
}

void printStack(const string indexStr[], map<int, int> indexMap) {
    int len = stateStk.size();
    for (int i = 0; i < len; i++) printf("%-2d ", stateStk[i]);
    for (int i = len; i <= 10; i++) printf("%-2s ", " ");

    len = flagStk.size();
    for (int i = 0; i < len; i++)
        printf("%-2s ", indexStr[indexMap[flagStk[i]]].c_str());
    for (int i = len; i <= 10; i++) printf("%-2s ", " ");

    len = placeStk.size();
    for (int i = 0; i < len; i++) printf("%-5d ", placeStk[i]);
    for (int i = len; i <= 7; i++) printf("%-5s ", " ");

    len = TC_Stk.size();
    for (int i = 0; i < len; i++) printf("%-2d ", TC_Stk[i]);
    for (int i = len; i <= 10; i++) printf("%-2s ", " ");

    len = FC_Stk.size();
    for (int i = 0; i < len; i++) printf("%-2d ", FC_Stk[i]);
}
void print(string nowStr, string action, const string indexStr[],
           map<int, int> indexMap) {
#ifdef DEBUG
    printf("%-5s ", nowStr.c_str());
    printf("%-5s ", action.c_str());
    printStack(indexStr, indexMap);
    printf("\n");
#endif
}
int newTemp() { return cnt++; }
/**
 * @word: 待查询变量
 * @tempIndex: 临时变量下标
 * @return: 0     - 499：  整型常量
 *          500   - 999：  实型常量
 *          1000  - 9999： 普通变量
 *          10000 - 99999：临时变量
 */
int entry(Result word, int tempIndex) {
    if (tempIndex != -1) return tempIndex + INT_LEN + DBL_LEN + NML_LEN;
    // 只要是常量，词法分析的结果都带上内码
    else if (word.identifyId == INT_CONSTANTS)
        return word.internalCode % INT_LEN;
    else if (word.identifyId == DBL_CONSTANTS)
        return word.internalCode % DBL_LEN + INT_LEN;
    else if (word.identifyId == IDENTIFY)
        return word.internalCode % NML_LEN + INT_LEN + DBL_LEN;
    return -1;
}

// 处理加减乘除
bool handleCal(int type) {
    int tIndex = newTemp();
    tIndex = entry(nowWord, tIndex);
    int E1_PLACE = *(placeStk.end() - 3), E2_PLACE = *(placeStk.end() - 1);
    Quat q(type, E1_PLACE, E2_PLACE, tIndex);
    quats.push_back(q);
    NXQ++;
    for (int i = 1; i <= 3; i++)
        stateStk.pop_back(), flagStk.pop_back(), placeStk.pop_back();
    flagStk.push_back(E_ID);
    int stateTop = *(stateStk.end() - 1);
    int value = exprLRTab[stateTop][8];
    if (value == -1) {
        return false;
    }
    stateStk.push_back(value);
    placeStk.push_back(tIndex);
    return true;
}
// 算术表达式
bool Expr() {
    printf("%-5s %-5s %-30s %-30s %-50s %-30s %-30s\n", "读头", "动作",
           "状态栈", "符号栈", "PLACE栈", "TC", "FC");
    Result leftWord = nowWord;
    nowWord = lex.getWord(line, col);
    if (nowWord.identifyId == EQL) {
        nowWord = lex.getWord(line, col);
        copyWord = nowWord;
    } else
        return true;
    stateStk.clear(), flagStk.clear(), placeStk.clear();
    stateStk.push_back(0), flagStk.push_back(SEMIC), placeStk.push_back(-1);
    print(nowWord.word, "", exprIndexStr, exprIndexMap);
    if (nowWord.identifyId == IDENTIFY || nowWord.identifyId == INT_CONSTANTS)
        nowWord.identifyId = IDENTIFY;
    bool acc = false, er = false;
    int stateTop, flagTOP, value;
    while (!acc && !er) {
        stateTop = *(stateStk.end() - 1);
        // 查表
        value = exprLRTab[stateTop][exprIndexMap[nowWord.identifyId]];
        if (value == -1) {
            er = true;
            continue;
        } else if (value == -2)
            acc = true;
        // 移进
        else if (value >= 0 && value < 100) {
            // printf("移进转%d\n", value);
            stateStk.push_back(value), flagStk.push_back(nowWord.identifyId);
            int index = entry(copyWord, -1);
            placeStk.push_back(index);
            nowWord = lex.getWord(line, col);
            lastWord = copyWord;
            copyWord = nowWord;
            if (nowWord.identifyId == IDENTIFY ||
                nowWord.identifyId == INT_CONSTANTS)
                nowWord.identifyId = IDENTIFY;
            print(nowWord.word, "移进", exprIndexStr, exprIndexMap);
        }
        if (value >= 101 && value <= 200) {
            // printf("按照 %d 规约\n", value);
            switch (value) {
                case 101:
                case 102:
                case 103:
                case 104: {
                    if (!handleCal(value)) {
                        er = true;
                    }
                    break;
                }
                // E -> (E)
                case 105: {
                    int E_PLACE = *(placeStk.end() - 2);
                    for (int i = 1; i <= 3; i++)
                        stateStk.pop_back(), flagStk.pop_back(),
                            placeStk.pop_back();
                    flagStk.push_back(E_ID);
                    stateTop = *(stateStk.end() - 1);
                    value = exprLRTab[stateTop][8];
                    if (value == -1) {
                        er = true;
                        break;
                    }
                    placeStk.push_back(E_PLACE);
                    stateStk.push_back(value);
                    break;
                    // E -> i
                }
                case 106: {
                    stateStk.pop_back(), flagStk.pop_back(),
                        placeStk.pop_back();
                    flagStk.push_back(E_ID);
                    stateTop = *(stateStk.end() - 1);
                    value = exprLRTab[stateTop][8];
                    if (value == -1) {
                        er = true;
                        break;
                    }
                    stateStk.push_back(value);
                    int index = entry(lastWord, -1);
                    placeStk.push_back(index);
                    break;
                }
            }
            print(nowWord.word, "规约", exprIndexStr, exprIndexMap);
        }
    }
    // 完成赋值操作
    int index = entry(leftWord, -1);
    Quat q(EQL, *(placeStk.end() - 1), -1, index);
    quats.push_back(q);
    NXQ++;
    stateStk.clear(), flagStk.clear(), placeStk.clear(), TC_Stk.clear(),
        FC_Stk.clear();
    if (acc && !er) return true;
    return false;
}
// 布尔表达式 返回待填的 E.TC E.FC
P Bool() {
    stateStk.clear(), flagStk.clear(), placeStk.clear(), TC_Stk.clear(),
        FC_Stk.clear();
    printf("%-5s %-5s %-30s %-30s %-50s %-30s %-30s\n", "读头", "动作",
           "状态栈", "符号栈", "PLACE栈", "TC", "FC");

    // 此处不应该使用 # ，因为实际上应该使用 )
    // 作为结束标志（简单起见，布尔表达式中没有左右括号）
    stateStk.push_back(0), flagStk.push_back(RIGHT), placeStk.push_back(-1),
        TC_Stk.push_back(-1), FC_Stk.push_back(-1);
    copyWord = nowWord;
    if (nowWord.identifyId == IDENTIFY || nowWord.identifyId == INT_CONSTANTS)
        nowWord.identifyId = IDENTIFY;
    bool acc = false, er = false;
    int stateTop, flagTOP, value, topTC, topFC, newFc, newTc, varIndex;
    print(nowWord.word, "", boolIndexStr, boolIndexMap);
    while (!acc && !er) {
        stateTop = *(stateStk.end() - 1);
        // 查表
        value = boolLRTab[stateTop][boolIndexMap[nowWord.identifyId]];
        if (value == -1) {
            er = true;
            continue;
        } else if (value == -2)
            acc = true;
        // 移进
        else if (value >= 0 && value < 100) {
            // printf("移进转%d\n", value);
            stateStk.push_back(value), flagStk.push_back(nowWord.identifyId);
            int index = entry(copyWord, -1);
            placeStk.push_back(index);
            FC_Stk.push_back(-1), TC_Stk.push_back(-1);
            nowWord = lex.getWord(line, col);
            lastWord = copyWord;
            copyWord = nowWord;
            if (nowWord.identifyId == IDENTIFY ||
                nowWord.identifyId == INT_CONSTANTS)
                nowWord.identifyId = IDENTIFY;
            print(nowWord.word, "移进", boolIndexStr, boolIndexMap);
        }  // 规约
        else if (value >= 102 && value <= 108) {
            switch (value) {
                // B   -> B_0 B
                case 102: {
                    newTc = *(TC_Stk.end() - 1);
                    newFc = MERG(*(FC_Stk.end() - 2), *(FC_Stk.end() - 1));
                    stateStk.pop_back(), flagStk.pop_back(),
                        placeStk.pop_back(), FC_Stk.pop_back(),
                        TC_Stk.pop_back();
                    stateStk.pop_back(), flagStk.pop_back(),
                        placeStk.pop_back(), FC_Stk.pop_back(),
                        TC_Stk.pop_back();
                    int stateTop = *(stateStk.end() - 1);
                    value = boolLRTab[stateTop][boolIndexMap[B_ID]];
                    stateStk.push_back(value), flagStk.push_back(B_ID),
                        placeStk.push_back(-1);
                    TC_Stk.push_back(newTc), FC_Stk.push_back(newFc);
                    break;
                }
                // B   -> B_1 B
                case 103: {
                    newFc = *(FC_Stk.end() - 1);
                    newTc = MERG(*(TC_Stk.end() - 2), *(TC_Stk.end() - 1));
                    stateStk.pop_back(), flagStk.pop_back(),
                        placeStk.pop_back(), FC_Stk.pop_back(),
                        TC_Stk.pop_back();
                    stateStk.pop_back(), flagStk.pop_back(),
                        placeStk.pop_back(), FC_Stk.pop_back(),
                        TC_Stk.pop_back();
                    int stateTop = *(stateStk.end() - 1);
                    value = boolLRTab[stateTop][boolIndexMap[B_ID]];
                    stateStk.push_back(value), flagStk.push_back(B_ID),
                        placeStk.push_back(-1);
                    TC_Stk.push_back(newTc), FC_Stk.push_back(newFc);
                    break;
                }
                // B   -> i
                case 104: {
                    // 本应出栈再进栈 但是只有一个元素 直接修改栈顶即可
                    *(flagStk.end() - 1) = B_ID, *(TC_Stk.end() - 1) = NXQ,
                                      *(FC_Stk.end() - 1) = NXQ + 1,
                                      *(placeStk.end() - 1) =
                                          entry(lastWord, -1);
                    ;
                    stateStk.pop_back();
                    stateTop = *(stateStk.end() - 1);
                    value = boolLRTab[stateTop][boolIndexMap[B_ID]];
                    varIndex = entry(lastWord, -1);
                    stateStk.push_back(value);
                    Quat q(JNZ, varIndex, -1, 0);
                    quats.push_back(q);
                    Quat q1(J, -1, -1, 0);
                    quats.push_back(q1);
                    NXQ += 2;
                    break;
                }
                // B   -> i < i
                case 105:
                // B   -> i > i
                case 106: {
                    int type = value == 105 ? JLES : JGRT;
                    int b1_place = *(placeStk.end() - 3),
                        b2_place = *(placeStk.end() - 1);
                    for (int i = 1; i <= 3; i++)
                        stateStk.pop_back(), flagStk.pop_back(),
                            placeStk.pop_back(), TC_Stk.pop_back(),
                            FC_Stk.pop_back();
                    int stateTop = *(stateStk.end() - 1);
                    value = boolLRTab[stateTop][boolIndexMap[B_ID]];
                    stateStk.push_back(value);
                    TC_Stk.push_back(NXQ), FC_Stk.push_back(NXQ + 1);

                    Quat q(type, b1_place, b2_place, 0);
                    quats.push_back(q);

                    Quat q1(J, -1, -1, 0);
                    quats.push_back(q1);
                    placeStk.push_back(-1);
                    flagStk.push_back(B_ID);
                    NXQ += 2;
                    break;
                }
                // B_0 -> B &&
                case 107: {
                    int b_tc = *(TC_Stk.end() - 2), b_fc = *(FC_Stk.end() - 2);
                    BACKPATCH(b_tc, NXQ);
                    for (int i = 1; i <= 2; i++)
                        stateStk.pop_back(), flagStk.pop_back(),
                            placeStk.pop_back(), TC_Stk.pop_back(),
                            FC_Stk.pop_back();
                    stateTop = *(stateStk.end() - 1);
                    value = boolLRTab[stateTop][boolIndexMap[B_0_ID]];
                    stateStk.push_back(value);
                    placeStk.push_back(-1), TC_Stk.push_back(0),
                        flagStk.push_back(B_1_ID), FC_Stk.push_back(b_fc);
                    break;
                }
                // B_1 -> B ||
                case 108: {
                    int b_tc = *(TC_Stk.end() - 2), b_fc = *(FC_Stk.end() - 2);
                    BACKPATCH(b_fc, NXQ);
                    for (int i = 1; i <= 2; i++)
                        stateStk.pop_back(), flagStk.pop_back(),
                            placeStk.pop_back(), TC_Stk.pop_back(),
                            FC_Stk.pop_back();
                    stateTop = *(stateStk.end() - 1);
                    value = boolLRTab[stateTop][boolIndexMap[B_0_ID]];
                    stateStk.push_back(value);
                    placeStk.push_back(-1), TC_Stk.push_back(b_tc),
                        flagStk.push_back(B_1_ID), FC_Stk.push_back(0);
                    break;
                }
            }
            print(nowWord.word, "规约", boolIndexStr, boolIndexMap);
        }
    }
    if (acc && !er) {
        int E_TC = *(TC_Stk.end() - 1), E_FC = *(FC_Stk.end() - 1);
        P p(E_TC, E_FC);
        stateStk.clear(), flagStk.clear(), placeStk.clear(), TC_Stk.clear(),
            FC_Stk.clear();
        return p;
    } else {
        P p(-1, -1);
        stateStk.clear(), flagStk.clear(), placeStk.clear(), TC_Stk.clear(),
            FC_Stk.clear();
        return p;
    }
}
bool Type() {}
// if (Bool) {} else {}
// 目前能力有限，假如 抽到的 if 语句是嵌套的，则不做复合语句
// 目前先以 复合语句来
bool ifStmt() {
    nowWord = lex.getWord(line, col);
    if (nowWord.identifyId == LEFT) {
        nowWord = lex.getWord(line, col);
        P p = Bool();
        // 回填真出口
        BACKPATCH(p.first, NXQ);
        // 布尔表达式的假出口
        int boolFC = p.second;

        // 此时完成了布尔表达式的翻译
        nowWord = lex.getWord(line, col);
        Block();
        // Stmts();

        

        // nowWord = lex.getWord(line, col);
        if (nowWord.identifyId == ELSE) {
            // 执行完 条件为真的代码块后，需要来一条指令，
            // 跳过 else 后面的代码块
            Quat q(J, -1, -1, 0);
            quats.push_back(q);
            // 记下 q(J, -1, -1, 0); 所在下标，过会需要进行回填
            int this_NXQ = NXQ;

            NXQ++;
            // 回填假出口
            BACKPATCH(boolFC, NXQ);

            nowWord = lex.getWord(line, col);
            Block();
            Quat q1(J, -1, -1, NXQ + 1);
            quats.push_back(q1);
            // 回填
            quats[this_NXQ].t = NXQ + 1;
            NXQ++;
        } else {
            // 如果 if 后面不带 else
            // 回填假出口
            BACKPATCH(boolFC, NXQ);
        }
    }
}
// 单语句
bool Stmt() {
    if (nowWord.identifyId == SEMIC) return true;
    if (nowWord.identifyId == INT || nowWord.identifyId == DBL ||
        nowWord.identifyId == CHAR)
        variableDefinitions();
    else if (nowWord.identifyId == IF)
        ifStmt();
    else if (nowWord.identifyId == IDENTIFY)
        Expr();
    else if (nowWord.identifyId == WHILE)
        whileStmt();
}
bool whileStmt() {
    // 先记下来，后面需要跳转到这里
    int this_NXQ = NXQ;
    nowWord = lex.getWord(line, col);
    if (nowWord.identifyId == LEFT) {
        nowWord = lex.getWord(line, col);
        P p = Bool();
        int e_TC = p.first, e_FC = p.second;
        nowWord = lex.getWord(line, col);
        if (nowWord.identifyId == LEFT_BIG) {
            // 回填真出口
            BACKPATCH(e_TC, NXQ);
            Block();
            // 跳转回去，检查循环条件
            Quat q(J, -1, -1, this_NXQ);
            quats.push_back(q);
            NXQ++;

            // 回填假出口
            BACKPATCH(e_FC, NXQ);
        }
    }
}
// 多语句
bool Stmts() {
    Stmt();
    if (nowWord.identifyId == RIGHT_BIG) return true;

    if (nowWord.identifyId == SEMIC) {
        nowWord = lex.getWord(line, col);
        Stmts();
    }
}
stack<bool>st;
// 代码块
bool Block() {
    if (nowWord.identifyId == LEFT_BIG) {
        st.push(true);
        nowWord = lex.getWord(line, col);
        Stmts();
        // 右大括号，有可能是当前的代码块结束，也有可能是尚未结束
        // 考虑一种情况 
        /*
                    {
                {

            }
        }
        */
        /* if (nowWord.identifyId == RIGHT_BIG){
            st.pop();
            if (st.empty())return true;
            nowWord = lex.getWord(line, col);
            Stmts();
        } */
    }
    if (nowWord.identifyId == RIGHT_BIG){
        nowWord = lex.getWord(line, col);
        st.pop();
        return true;
    } else{
        Stmts();
    }
}
// C语言程序
bool Program() {
    if (nowWord.identifyId == VOID) {
        nowWord = lex.getWord(line, col);
        if (nowWord.identifyId == IDENTIFY && nowWord.word == "main") {
            nowWord = lex.getWord(line, col);
            if (nowWord.identifyId == LEFT) {
                nowWord = lex.getWord(line, col);
                if (nowWord.identifyId == RIGHT) {
                    nowWord = lex.getWord(line, col);
                    Block();
                }
            }
        }
    }
}
void printQuats() {
    // 词法分析器中保存的几张表要拿出来了
    vector<string> idtfTab = lex.getIdtfTab();
    vector<int> intTab = lex.getIntTab();
    vector<double> dblTab = lex.getDblTab();

    for (int i = 1; i < NXQ; i++) {
        Quat q = quats[i];
        int qOp = q.op;
        string src1Str = "-", src2Str = "-", dstStr = "-";
        // srcStr 记录的都是入口地址信息

        // 整型常量
        if (q.arg1 >= 0 && q.arg1 < INT_LEN)
            src1Str = to_string(intTab[q.arg1]);
        else if (q.arg1 >= INT_LEN && q.arg1 < INT_LEN + DBL_LEN)
            src1Str = to_string(dblTab[q.arg1 - INT_LEN]);
        // 普通变量
        else if (q.arg1 >= INT_LEN + DBL_LEN &&
                 q.arg1 < INT_LEN + DBL_LEN + NML_LEN)
            src1Str = idtfTab[q.arg1 - INT_LEN - DBL_LEN];
        // 临时变量
        else if (q.arg1 >= 10000)
            src1Str = "T" + to_string(q.arg1 - 10000);

        if (q.arg2 >= 0 && q.arg2 < INT_LEN)
            src2Str = to_string(intTab[q.arg2]);
        else if (q.arg2 >= INT_LEN && q.arg2 < INT_LEN + DBL_LEN)
            src2Str = to_string(dblTab[q.arg2 - INT_LEN]);
        else if (q.arg2 >= INT_LEN + DBL_LEN &&
                 q.arg2 < INT_LEN + DBL_LEN + NML_LEN)
            src2Str = idtfTab[q.arg2 - INT_LEN - DBL_LEN];
        else if (q.arg2 >= 10000)
            src2Str = "T" + to_string(q.arg2 - 10000);
        if (qOp >= 1 && qOp <= 5) {
            dstStr = to_string(q.t);
        } else {
            if (q.t < INT_LEN)
                dstStr = to_string(intTab[q.t]);
            else if (q.t >= INT_LEN && q.t < INT_LEN + DBL_LEN)
                dstStr = to_string(dblTab[q.t - INT_LEN]);
            else if (q.t >= INT_LEN + DBL_LEN &&
                     q.t < INT_LEN + DBL_LEN + NML_LEN)
                dstStr = idtfTab[q.t - INT_LEN - DBL_LEN];
            else
                dstStr = "T" + to_string(q.t - 10000);
        }

        printf("%-3d: (%-6s, %-6s, %-6s, %-6s)\n", i, opMap[qOp].c_str(),
               src1Str.c_str(), src2Str.c_str(), dstStr.c_str());
    }
}
int main() {
    Quat q(-1, -1, -1, -1);
    quats.push_back(q);
    nowWord = lex.getWord(line, col);
    Program();
    printQuats();
    return 0;
}