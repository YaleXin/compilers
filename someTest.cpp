/*
 * @Author      : YaleXin
 * @Email       : 181303209@yzu.edu.cn
 * @LastEditors : YaleXin
 */
#include <algorithm>
#include <cstdio>
#include <iostream>
using namespace std;
int main(){
    ios::sync_with_stdio(false);
    int a[] = {1,2,3,4,5,6,7,8};
    cout << lower_bound(a, a + 8, 10) - a << endl;
    cout << lower_bound(a, a + 8, 0) - a << endl;
    cout << lower_bound(a, a + 8, 6) - a << endl;
    return 0;
}