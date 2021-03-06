/*
 * @Author      : YaleXin
 * @Email       : 181303209@yzu.edu.cn
 * @LastEditors : YaleXin
 */
#ifndef __CONSTANT_H__
#define __CONSTANT_H__
using namespace std;
const int exprLRTab[][9] = {{3, -1, -1, -1, -1, 2, -1, -1, 1},
                            {-1, 4, 5, 6, 7, -1, -1, -2, -1},
                            {3, -1, -1, -1, -1, 2, -1, -1, 8},
                            {-1, 106, 106, 106, 106, -1, 106, 106, -1},
                            {3, -1, -1, -1, -1, 2, -1, -1, 9},
                            {3, -1, -1, -1, -1, 2, -1, -1, 10},
                            {3, -1, -1, -1, -1, 2, -1, -1, 11},
                            {3, -1, -1, -1, -1, 2, -1, -1, 12},
                            {-1, 4, 5, 6, 7, -1, 13, -1, -1},
                            {-1, 101, 101, 6, 7, -1, 101, 101, -1},
                            {-1, 102, 102, 7, 7, -1, 102, 102, -1},
                            {-1, 103, 103, 103, 103, -1, 103, 103, -1},
                            {-1, 104, 104, 104, 104, -1, 104, 104, -1},
                            {-1, 105, 105, 105, 105, -1, 105, 105, -1}};
const int boolLRTab[13][10] = {{-1, 3, -1, -1, -1, -1, -1, 2, 1, 4},
                               {-1, 3, -1, -1, -1, -1, -1, 5, 1, 4},
                               {-2, -1, -1, -1, 7, 6, -1, -1, -1, -1},
                               {104, -1, 8, 9, 104, 104, -1, -1, -1, -1},
                               {-1, 3, -1, -1, -1, -1, -1, 10, 1, 4},
                               {102, -1, -1, -1, 7, 102, -1, -1, -1, -1},
                               {-1, 108, -1, -1, -1, -1, -1, -1, -1, -1},
                               {-1, 107, -1, -1, -1, -1, -1, -1, -1, -1},
                               {-1, 11, -1, -1, -1, -1, -1, -1, -1, -1},
                               {-1, 12, -1, -1, -1, -1, -1, -1, -1, -1},
                               {103, -1, -1, -1, 103, 103, -1, -1, -1, -1},
                               {105, -1, -1, -1, 105, 105, -1, -1, -1, -1},
                               {106, -1, -1, -1, 106, 106, -1, -1, -1, -1}};
const int END = -2021, ADD = 62, SUB = 63;
const int MUL = 64, DIV = 65, IDENTIFY = 32;
const int LEFT = 39, RIGHT = 40, LEFT_BIG = 41, RIGHT_BIG = 42,
          INT_CONSTANTS = 33, DBL_CONSTANTS = 34;
const int REAL_CONSTANTS = 34, EQL = 53, SEMIC = 38;
const int ACC = 0, E_ID = 101, S_ID = 101, B_ID = 102, B_0_ID = 103,
          B_1_ID = 104;
const int INT_LEN = 500, DBL_LEN = 500, NML_LEN = 9000, TMP_LEN = 90000;
const int DO = 7, ELSE = 9, WHILE = 31, IF = 15, INT = 16, DBL = 8, CHAR = 3, VOID = 29, FOR = 13;
const int LES = 45, GRT = 48, AND = 61, OR = 59;
const int J = 1, JLES = 2, JGRT = 3, JNZ = 4, OP_ADD = 101, OP_SUB = 102, OP_MUL = 103, OP_DIV = 104, OP_PRINT = 200;
map<int, string> opMap = {{0, "?"},     {J, "j"},     {JLES, "j<"},
                          {JGRT, "j>"}, {JNZ, "jnz"}, {EQL, "="}, {101, "+"}, {102, "-"}, {103, "*"}, {104, "/"}, {OP_PRINT, "print"}};
map<int, int> boolIndexMap = {{RIGHT, 0},  {IDENTIFY, 1}, {LES, 2},  {GRT, 3},
                              {AND, 4},    {OR, 5},       {S_ID, 6}, {B_ID, 7},
                              {B_0_ID, 8}, {B_1_ID, 9}};
string boolIndexStr[] = {")", "i", "<", ">", "&&", "||", "S", "B", "B0", "B1"};
map<int, int> exprIndexMap = {{IDENTIFY, 0}, {ADD, 1},   {SUB, 2},
                              {MUL, 3},      {DIV, 4},   {LEFT, 5},
                              {RIGHT, 6},    {SEMIC, 7}, {E_ID, 8}};
string exprIndexStr[] = {"i", "+", "-", "*", "/", "(", ")", ";", "E"};

#endif