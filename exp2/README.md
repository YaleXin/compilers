
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

**������ݹ�**

```code
// �ȶ�Expr��ȡ�����ӣ�
// ��˼��Expr ��������ǣ�1��Expr ������ �ӺŻ��߼��� ������ Temp1 ��2��Temp1
Expr -> Expr [+ | -] Temp1 | Temp1
// ���� Expr ֱ����ݹ�
Expr    -> Temp1 E_1
E_1     -> [+ | -] Temp1 E_1 | epsilon

// �������� Temp1 ����ݹ�
// ��ȡ������
Temp1	-> Temp1 [* | /] Temp2 | Temp2
// ����ֱ����ݹ�
Temp1   -> Temp2 E_2
E_2		-> [* | /] Temp2 E_2 | epsilon
// ���ϣ����յ��ķ�

Expr    -> Temp1 E_1
E_1     -> [+ | -] Temp1 E_1 | epsilon

Temp1   -> Temp2 E_2
E_2		-> [* | /] Temp2 E_2 | epsilon

Temp2	-> ( Expr )
		|  id
		|  number
```

## Ԥ�����

������ÿ������ʽ���б�ţ�

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

### First����Follw����

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

### Ԥ�������

|     | (    | +    | -    | *    | /    | id   | number | )    |#|
| ---- | ---- | ---- | ---- | ---- | ---- | ---- | ------ | ---- |----|
| `Expr` | `Expr->Temp1 E_1(1)` |      |      |      |      | `Expr->Temp1 E_1(1)` | `Expr->Temp1 E_1(1)` |      |    |
| `E_1` | | `E_1->+Temp1 E_1(2)` | `E_1->-Temp1 E_1(3)` | | | | | `E_1-> epsilon		(4)` | `E_1-> epsilon		(4)` |
| `Temp1` | `Temp1-> Temp2 E_2	(5)` | | | | | `Temp1-> Temp2 E_2	(5)` | `Temp1-> Temp2 E_2	(5)` | | |
| `E_2` | | `E_2		-> epsilon		(8)` | `E_2		-> epsilon		(8)` | `E_2		-> * Temp2 E_2	(6)` | `E_2		-> / Temp2 E_2	(7)` | | |`E_2		-> epsilon		(8)` | `E_2		-> epsilon		(8)` |
| `Temp2` | `Temp2	-> ( Expr )		(9)` | | | | | `Temp2	-> id			(10)` | `Temp2	-> number		(11)` | | |


