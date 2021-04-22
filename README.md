<!--

 * @Author      : YaleXin
 * @Email       : 181303209@yzu.edu.cn
 * @LastEditors : YaleXin
-->
## ˵��
`xinCpl.cpp`�Ǳ����������ļ�
## �������
0-31��������
32����ʶ��
33�����ͳ���
34��ʵ�ͳ���
35���ַ�����

36���ַ�������

37���������

## ������
|����|����|
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

## ��ʶ��

��ʶ�����Ϊ`32`���ڲ�ʹ������������ֲ�ͬ�ı�ʶ����

## ����

| ��������   | ��� |
| ---------- | ---- |
| ���ͳ���   | 33   |
| ʵ�ͳ���   | 34   |
| �ַ��ͳ��� | 35   |
| �ַ������� | 36   |

## ���޷�

��ƽ������Ŵ�`37`��ʼ����ϸ����:
|���|���|
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

## ֧�ֵ�ת���ַ�

�������ֻ֧�����г�����ת���ַ�

| ת���ַ� | ����                                | ASCII��ֵ��ʮ���ƣ� |
| -------- | ----------------------------------- | ------------------- |
| \a       | ����(BEL)                           | 007                 |
| \b       | �˸�(BS) ������ǰλ���Ƶ�ǰһ��     | 008                 |
| \f       | ��ҳ(FF)������ǰλ���Ƶ���ҳ��ͷ    | 012                 |
| \n       | ����(LF) ������ǰλ���Ƶ���һ�п�ͷ | 010                 |
| \r       | �س�(CR) ������ǰλ���Ƶ����п�ͷ   | 013                 |
| \t       | ˮƽ�Ʊ�(HT)                        | 009                 |
| \v       | ��ֱ�Ʊ�(VT)                        | 011                 |
| `\'`     | ������                              | 039                 |
| `\"`     | ˫����                              | 034                 |
| `\\`     | ��б��                              | 092                 |

**��֧��`\177`����` \x7f `**

## �ݹ��½�������

������ķ��У�Сд�����ս������д��ͷ���Ƿ��ս��

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
		|  Bool ������ Bool
		|  Bool ������ Bool
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

���ڷ��ս��`Bool`�Ĳ���ʽ����ֱ����ݹ飡

�����`Bool`����������ݹ飺

```code
// �Ƚ�������һ��
Bool    -> Bool ������ ���� ������ Bool | T
T       -> Expr == Expr
		|  Expr != Expr
		|  Expr <  Expr
		|  Expr >  Expr
		|  Expr
//�����ݹ�
Bool   -> T Bool2
Bool2  -> ������ ���� ������ Bool Bool2
```

�����գ�

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
Type    -> char | short | int | long | double | float | epsilon
Bool    -> Temp0 Bool2
Bool2   -> ������ ���� ������ Bool Bool2
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

`First`����

```code
F(Program) = {'int'}
F(Block) = {'{'}
F(Stmts) = {epsilon, F(Stmt)}
F(Stmt)	 = {epsilon, id, 'char', 'short', 'int', ''}
```



���Ե�C���Գ���

```c
void main(){
    
    int f_i = 1;
    int sum = 0;
    while(f_i <= 100){
        sum = sum + f_i * f_i;
        f_i = f_i + 1; 
    }
    return 0;
}
```

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


