`LR(1)`分析表来自于 [Vizdl](https://www.cnblogs.com/vizdl/) 的文章[LR(1)语法分析器生成器(生成Action表和Goto表)java实现(二)](https://www.cnblogs.com/vizdl/p/11331278.html)

8086汇编代码中，将16进制数据转为10进制并输出的代码来自[汇编:把寄存器AX的十六进制数转换成十进制形式输出](https://blog.csdn.net/pfboy/article/details/1549942)

非常感谢！

## C语言文法

下面的文法中，小写的是终结符，大写开头的是非终结符

$$
epsilon = \epsilon
$$

```code
Program -> int main() Block
Block   -> { Stmts }
Stmts   -> Stmt Stmts | epsilon
Stmt    ->Type id = Expr;
		|  if (Bool) Block
		|  if (Bool) Block else Block
		|  while (Bool) Block
		|  do (Bool) Block while(Bool);
Type    -> char | short | int | long | double | float
Bool	-> Expr == Expr
		|  Expr != Expr
		|  Expr <  Expr
		|  Expr >  Expr
		|  Bool 与运算 Bool
		|  Bool 或运算 Bool
		|  Expr
Expr	-> Expr + Temp1
		|  Expr - Temp1
		|  Temp1
Temp1	-> Temp1 * Temp2
		|  Temp1 / Temp2
		|  Temp2
Temp2	-> ( Expr )
		|  id
		|  number
```

布尔表达式文法

```
S   -> B
B   -> B_0 B
B   -> B_1 B
B   -> i
B   -> i < i
B   -> i > i
B_0 -> B &&
B_1 -> B ||
```
```
S   -> B
B   -> B_0 B
B   -> B_1 B
B   -> B
B   -> B ROP B
B   -> B BOP B
ROP ->  >
	  | >=
	  | <
      | <= 
      | !=
BOP -> +
B_0 -> B &&
B_1 -> B ||
```



### 表达式的`LR1`文法

| eof  | i    | <    | >    | &&   | `||` | S    | B    | B0   | B1   |
| ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- |
| err  | s2   | err  | err  | err  | err  | err  | s1   | s3   | s4   |
| acc  | err  | s6   | s5   | s8   | s7   | err  | err  | err  | err  |
| r4   | err  | r4   | r4   | r4   | r4   | err  | err  | err  | err  |
| err  | s2   | err  | err  | err  | err  | err  | s9   | s3   | s4   |
| err  | s2   | err  | err  | err  | err  | err  | s10  | s3   | s4   |
| err  | s2   | err  | err  | err  | err  | err  | s11  | s3   | s4   |
| err  | s2   | err  | err  | err  | err  | err  | s12  | s3   | s4   |
| err  | r8   | err  | err  | err  | err  | err  | err  | err  | err  |
| err  | r7   | err  | err  | err  | err  | err  | err  | err  | err  |
| r2   | err  | s6   | s5   | s8   | s7   | err  | err  | err  | err  |
| r3   | err  | s6   | s5   | s8   | s7   | err  | err  | err  | err  |
| r6   | err  | s6   | s5   | s8   | s7   | err  | err  | err  | err  |
| r5   | err  | s6   | s5   | s8   | s7   | err  | err  | err  | err  |


### 抽象数组

| eof  | i    | <    | >    | &&   | `||` | S    | B    | B0   | B1   |
| ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- |
| -1   | 2    | -1   | -1   | -1   | -1   | -1   | 1    | 3    | 4    |
| acc  | -1   | 6    | 5    | 8    | 7    | -1   | -1   | -1   | -1   |
| 104  | -1   | 104  | 104  | 104  | 104  | -1   | -1   | -1   | -1   |
| -1   | 2    | -1   | -1   | -1   | -1   | -1   | 9    | 3    | 4    |
| -1   | 2    | -1   | -1   | -1   | -1   | -1   | 10   | 3    | 4    |
| -1   | 2    | -1   | -1   | -1   | -1   | -1   | 11   | 3    | 4    |
| -1   | 2    | -1   | -1   | -1   | -1   | -1   | 12   | 3    | 4    |
| -1   | 108  | -1   | -1   | -1   | -1   | -1   | -1   | -1   | -1   |
| -1   | 107  | -1   | -1   | -1   | -1   | -1   | -1   | -1   | -1   |
| 102  | -1   | 6    | 5    | 8    | 7    | -1   | -1   | -1   | -1   |
| 103  | -1   | 6    | 5    | 8    | 7    | -1   | -1   | -1   | -1   |
| 106  | -1   | 6    | 5    | 8    | 7    | -1   | -1   | -1   | -1   |
| 105  | -1   | 6    | 5    | 8    | 7    | -1   | -1   | -1   | -1   |

![1622685264099](C:\Users\YaleXin\AppData\Roaming\Typora\typora-user-images\1622685264099.png)