
## ���ʽ

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

## ����������ȵı��ʽ����

����������ʽ��һ�����

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

