
## 表达式

```code
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

**消除左递归**

```code
// 先对Expr提取公因子：
// 意思是Expr 后面可以是（1）Expr 连接上 加号或者减号 连接上 Temp1 （2）Temp1
Expr -> Expr [+ | -] Temp1 | Temp1
// 消除 Expr 直接左递归
Expr    -> Temp1 E_1
E_1     -> [+ | -] Temp1 E_1 | epsilon

// 继续消除 Temp1 的左递归
// 提取公因子
Temp1	-> Temp1 [* | /] Temp2 | Temp2
// 消除直接左递归
Temp1   -> Temp2 E_2
E_2		-> [* | /] Temp2 E_2 | epsilon
// 综上，最终的文法

Expr    -> Temp1 E_1
E_1     -> [+ | -] Temp1 E_1 | epsilon

Temp1   -> Temp2 E_2
E_2		-> [* | /] Temp2 E_2 | epsilon

Temp2	-> ( Expr )
		|  id
		|  number
```

## 预测分析

对上述每个产生式进行编号：

```
Expr    -> Temp1 E_1 	(1)
E_1     -> + Temp1 E_1 	(2)
E_1     -> - Temp1 E_1	(3)
E_1		-> epsilon		(4)
Temp1   -> Temp2 E_2	(5)
E_2		-> * Temp2 E_2	(6)
E_2		-> / Temp2 E_2	(7)
E_2		-> epsilon		(8)
Temp2	-> ( Expr )		(9)
Temp2	-> id			(10)
Temp2	-> number		(11)
```

### First集和Follw集合

```code
First(Expr) = First(Temp1) = First(Temp2) = {(, id, number}
First(E_1)  = {+, -, epsilon}
First(E_2)  = {*, /, epsilon}

Follow(Expr)  = {#, )}
Follw(E_1) 	  =  Follow(Expr) = {#, )}
Follow(Temp1) = Follow(Expr) + First(E_1) - {epsilon} + Follow(E_1)   = {#, ), +, -}
Follow(E_2)	  = Follow(Temp1) = {#, ), +, -}
Follow(Temp2) = Follow(Temp1) + First(E_2) - {epsilon} + Follow(E_2)   = {#, ), +,  -, *, /}
```

### 预测分析表

|     | (    | +    | -    | *    | /    | id   | number | )    |#|
| ---- | ---- | ---- | ---- | ---- | ---- | ---- | ------ | ---- |----|
| `Expr` | `Expr->Temp1 E_1(1)` |      |      |      |      | `Expr->Temp1 E_1(1)` | `Expr->Temp1 E_1(1)` |      |    |
| `E_1` | | `E_1->+Temp1 E_1(2)` | `E_1->-Temp1 E_1(3)` | | | | | `E_1-> epsilon		(4)` | `E_1-> epsilon		(4)` |
| `Temp1` | `Temp1-> Temp2 E_2	(5)` | | | | | `Temp1-> Temp2 E_2	(5)` | `Temp1-> Temp2 E_2	(5)` | | |
| `E_2` | | `E_2		-> epsilon		(8)` | `E_2		-> epsilon		(8)` | `E_2		-> * Temp2 E_2	(6)` | `E_2		-> / Temp2 E_2	(7)` | | |`E_2		-> epsilon		(8)` | `E_2		-> epsilon		(8)` |
| `Temp2` | `Temp2	-> ( Expr )		(9)` | | | | | `Temp2	-> id			(10)` | `Temp2	-> number		(11)` | | |


