/*
 * @Author      : YaleXin
 * @Email       : 181303209@yzu.edu.cn
 * @LastEditors : YaleXin
 */
#include <algorithm>
#include <cstdio>
#include <iostream>
using namespace std;
const string delimiters[] = {
    ",",  ";", "(",  ")",  "{",  "}",   "[", "]", "<", "<<",
    "<=", ">", ">>", ">=", "==", "!==", "=", "~", "!", "?",
    ":",  "|", "||", "&",  "&&", "+",   "-", "*", "/", "%"};
int main(){
    ios::sync_with_stdio(false);
    // int a[] = {1,2,3,4,5,6,7,8};
    // cout << lower_bound(a, a + 8, 10) - a << endl;
    // cout << lower_bound(a, a + 8, 0) - a << endl;
    // cout << lower_bound(a, a + 8, 6) - a << endl;
    cout << ((int)8.000000000000 == 8.0) << endl;
    int p = 5;
    double p2 = 5.555;
    int *pp2 = &p;
    cout << (*pp2) << endl;
    for (int i = 0; i < 30; i++){
        cout << delimiters[i] << endl;
    }
    return 0;
}