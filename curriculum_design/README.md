`LR(1)`分析表来自于 [Vizdl](https://www.cnblogs.com/vizdl/) 的文章[LR(1)语法分析器生成器(生成Action表和Goto表)java实现(二)](https://www.cnblogs.com/vizdl/p/11331278.html)

8086汇编代码中，将16进制数据转为10进制并输出的代码来自[汇编:把寄存器AX的十六进制数转换成十进制形式输出](https://blog.csdn.net/pfboy/article/details/1549942)

非常感谢！

## C语言文法

下面的文法中，小写的是终结符，大写开头的是非终结符

$$
epsilon = \epsilon
$$

```code
Program -> void main() Block
Block   -> { Stmts }
Stmts   -> Stmt Stmts | epsilon
Stmt    ->Type id;
		|  if (Bool) Block
		|  if (Bool) Block else Block
		|  while (Bool) Block
		|  do (Bool) Block while(Bool);
		| for (Expr;Bool;Expr)Block
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

## 编译器说明

- 目前只支持`if`语句，`while`语句，`do while`语句，赋值语句，布尔表达式，
- 仅支持16位整数
- 语句嵌套不支持，特别是含有if语句的嵌套（准确来说涉及布尔表达式），其他语句部分支持嵌套
- 布尔表达式中不能含有算数表达式，不能括号嵌套
- 若干缺点.......

## 例子

打印斐波拉契数列前50项

```c++
void main(){
   int num1, num2, num3, index;
   num1 = 1;
   num2 = 1;
   index = 3;
   print(num1);
   print(num2);
   while(index < 50){
       num3 = num1 + num2;
       print(num3);
       num1 = num2;
       num2 = num3;
       index = index + 1;
   }
}
```

中间代码（四元式序列）：

```
1  : (=     , 1     , -     , num1  )
2  : (=     , 1     , -     , num2  )
3  : (=     , 3     , -     , index )
4  : (print , -     , -     , num1  )
5  : (print , -     , -     , num2  )
6  : (j<    , index , 50    , 8     )
7  : (j     , -     , -     , 16    )
8  : (+     , num1  , num2  , T0    )
9  : (=     , T0    , -     , num3  )
10 : (print , -     , -     , num3  )
11 : (=     , num2  , -     , num1  )
12 : (=     , num3  , -     , num2  )
13 : (+     , index , 1     , T1    )
14 : (=     , T1    , -     , index )
15 : (j     , -     , -     , 6     )
```

目标代码（8086汇编）：

```assembly
DATA SEGMENT
        divisors  DW 10000, 1000, 100, 10, 1
        results   DB 0,0,0,0,0,"$"
        copyright db 'Welcome to use the compiler designed by Yalexin!$'
        main DW 0
        num1 DW 0
        num2 DW 0
        num3 DW 0
        index DW 0
        print DW 0
        T0 DW 0
        T1 DW 0
DATA ENDS
CODE SEGMENT
        ASSUME CS:CODE, DS:DATA
START:
        MOV AX, DATA
        MOV DS, AX
        CALL CP_FUNCT
        MOV   AX,   1
        MOV   num1, AX
        MOV   AX,   1
        MOV   num2, AX
        MOV   AX,   3
        MOV   index,AX
        MOV   AX,   num1
        CALL  PRINT_FUNCT
        MOV   AX,   num2
        CALL  PRINT_FUNCT
        L1:
        MOV   AX,   index
        MOV   BX,   50
        CMP   AX,   BX
        JC    L2
        JMP   L3
        L2:
        MOV   AX,   num1
        MOV   BX,   num2
        ADD   AX,   BX
        MOV   T0,   AX
        MOV   AX,   T0
        MOV   num3, AX
        MOV   AX,   num3
        CALL  PRINT_FUNCT
        MOV   AX,   num2
        MOV   num1, AX
        MOV   AX,   num3
        MOV   num2, AX
        MOV   AX,   index
        MOV   BX,   1
        ADD   AX,   BX
        MOV   T1,   AX
        MOV   AX,   T1
        MOV   index,AX
        JMP   L1
        L3:
        MOV AH, 4CH
        INT 21H
PRINT_FUNCT PROC
        mov     si, offset divisors
        mov     di, offset results
        mov     cx,5
aa:
        mov     dx,0
        div     word ptr [si]
        add     al,48
        mov     byte ptr [di],al
        inc     di
        add     si,2
        mov     ax,dx
        loop    aa
        mov     cx,4
        mov     di, offset results
bb:
        cmp     byte ptr [di],'0'
        jne     print_
        inc     di
        loop    bb
print_:
        mov     dx,di
        mov     ah,9
        int     21h
        mov ah,2
        mov dl,0DH
        int 21h
        mov dl,0AH
        INT 21H
        RET
CP_FUNCT PROC
        mov   DX, offset copyright
        mov ah, 9
        int  21h
        mov ah,2
        mov dl,0DH
        int 21h
        mov dl,0AH
        INT 21H
        RET
CODE ENDS
END START
```

使用`emu8086`调试上述代码如下：

![1623897899901](D:\my_cpp_workspace\compilers\curriculum_design\p1)

溢出了~~~

![1623898061088](D:\my_cpp_workspace\compilers\curriculum_design\p2)