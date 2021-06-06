/*
 * @Author      : YaleXin
 * @Email       : 181303209@yzu.edu.cn
 * @LastEditors : YaleXin
 */
#include <algorithm>
#include <cstdio>
#include <iostream>
#include "../tempStructs.h"
using namespace std;
Quaternion quats[500];
int index = 0;
int main(){
    int op, src1, src2, dst;
    while(1){
        scanf("%d%d%d%d", &op, &src1, &src2, &dst);
        if (op == -1 && src1 == -1 && src2 == -1 && dst == -1)break;
        quats[index].op   = op, 
        quats[index].arg1 = src1, 
        quats[index].arg2 = src2, 
        quats[index].t    = dst;
        index++;
    }
    // for(int i = 0; i < index;)
    return 0;
}