<!--

 * @Author      : YaleXin
 * @Email       : 181303209@yzu.edu.cn
 * @LastEditors : YaleXin
-->
## 说明
`xinCpl.cpp`是编译器顶层文件
## 总体设计
0-31：保留字
32：标识符
33：整型常量
34：实型常量
35：字符常量

36：字符串常量

37：界符常量

## 保留字
|符号|类别号|
|---|---|
|auto| 0|
|break| 1|
|case| 2|
|char| 3|
|const| 4|
|continue| 5|
|default| 6|
|do| 7|
|double| 8|
|else| 9|
|enum| 10|
|extern| 11|
|float| 12|
|for| 13|
|goto| 14|
|if| 15|
|int| 16|
|long| 17|
|register| 18|
|return| 19|
|short| 20|
|signed| 21|
|sizeof| 22|
|static| 23|
|struct| 24|
|switch| 25|
|typedef| 26|
|union| 27|
|unsigned| 28|
|void| 29|
|volatile| 30|
|while| 31|

## 标识符

标识符类号为`32`，内部使用内码加以区分不同的标识符。

## 常量

| 常量类型   | 类号 |
| ---------- | ---- |
| 整型常量   | 33   |
| 实型常量   | 34   |
| 字符型常量 | 35   |
| 字符串常量 | 36   |

## 界限符

简称界符，类号从`37`开始，详细如下:
|界符|类号|
|---|---|
|,|	37|
|;|	38|
|(|	39|
|)|	40|
|{|	41|
|}|	42|
|[|	43|
|]|	44|
|<|	45|
|<<|	46|
|<=|	47|
|>|	48|
|>>|	49|
|>=|	50|
|==|	51|
|!==|	52|
|=|	53|
|~|	54|
|!|	55|
|?|	56|
|:|	57|
|`|`|	58|
|`||`|	59|
|&|	60|
|&&|	61|
|+|	62|
|-|	63|
|*|	64|
|/|	65|
|%|	66|

## 支持的转义字符

简单起见，只支持下列常见的转义字符

| 转义字符 | 意义                                | ASCII码值（十进制） |
| -------- | ----------------------------------- | ------------------- |
| \a       | 响铃(BEL)                           | 007                 |
| \b       | 退格(BS) ，将当前位置移到前一列     | 008                 |
| \f       | 换页(FF)，将当前位置移到下页开头    | 012                 |
| \n       | 换行(LF) ，将当前位置移到下一行开头 | 010                 |
| \r       | 回车(CR) ，将当前位置移到本行开头   | 013                 |
| \t       | 水平制表(HT)                        | 009                 |
| \v       | 垂直制表(VT)                        | 011                 |
| `\'`     | 单引号                              | 039                 |
| `\"`     | 双引号                              | 034                 |
| `\\`     | 反斜杠                              | 092                 |

**不支持`\177`或者` \x7f `**

## 递归下降分析器

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
		|  do (Bool) Block
		|  Block
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

对于非终结符`Bool`的产生式出现直接左递归！

下面对`Bool`进行消除左递归：

```code
// 先将它化简一下
Bool    -> Bool 或运算 或者 与运算 Bool | T
T       -> Expr == Expr
		|  Expr != Expr
		|  Expr <  Expr
		|  Expr >  Expr
		|  Expr
//消除递归
Bool   -> T Bool2
Bool2  -> 或运算 或者 与运算 Bool Bool2
```

即最终：

```code
Program -> int main() Block
Block   -> { Stmts }
Stmts   -> Stmt Stmts | epsilon
Stmt    ->Type id = Expr;
		|  if (Bool) Block
		|  if (Bool) Block else Block
		|  while (Bool) Block
		|  do (Bool) Block
		|  Block
Type    -> char | short | int | long | double | float
Bool    -> Temp0 Bool2
Bool2   -> 或运算 或者 与运算 Bool Bool2
Temp0   -> Expr == Expr
		|  Expr != Expr
		|  Expr <  Expr
		|  Expr >  Expr
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

