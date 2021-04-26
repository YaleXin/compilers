
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

## 基于算符优先的表达式计算

将上述产生式进一步编号

```code
E		-> # Expr # 		(1)
Expr	-> Expr + Temp1		(2)
Expr	->  Expr - Temp1	(3)
Expr	->  Temp1			(4)
Temp1	-> Temp1 * Temp2	(5)
Temp1	->  Temp1 / Temp2	(6)
Temp1	->  Temp2			(7)
Temp2	-> ( Expr )			(8)
Temp2	->  id				(9)
Temp2	->  number			(10)
```

