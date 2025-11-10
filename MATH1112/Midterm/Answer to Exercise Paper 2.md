# 练习卷 2 答案

## 第一题

### 思路

转化为矩阵，高斯消元法求解

### 解答

$$
\begin{pmatrix}
1 & 2 & 4 & -3 \\
3 & 5 & 6 & -4 \\
4 & 5 & -2 & 3 \\
3 & 8 & 24 & -19
\end{pmatrix}
\begin{pmatrix}
x_1 \\ x_2 \\ x_3 \\ x_4
\end{pmatrix}
=O
$$

$$
\begin{bmatrix}
1 & 2 & 4 & -3 \\
3 & 5 & 6 & -4 \\
4 & 5 & -2 & 3 \\
3 & 8 & 24 & -19
\end{bmatrix}
\sim
\begin{bmatrix}
1 & 2 & 4 & -3 \\
0 & 1 & 6 & -5 \\
0 & 1 & 6 & -5 \\
0 & 1 & 6 & -5
\end{bmatrix}
\sim
\begin{bmatrix}
1 & 2 & 4 & -3 \\
0 & 1 & 6 & -5 \\
0 & 0 & 0 & 0 \\
0 & 0 & 0 & 0
\end{bmatrix}
$$

令$x_3$，$x_4$为自由变量，则对于$x_3=1$，$x_4=0$，有$u_1=\begin{pmatrix} 8 \\ -6 \\ 1 \\ 0 \end{pmatrix}$，对于$x_3=0$，$x_4=1$，有$u_2=\begin{pmatrix} -7 \\ 5 \\ 0 \\ 1 \end{pmatrix}$。
故基础解系为$x=\begin{pmatrix} 8 \\ -6 \\ 1 \\ 0 \end{pmatrix}x_3+\begin{pmatrix} -7 \\ 5 \\ 0 \\ 1 \end{pmatrix}x_4$。

### 答案

$$
x=
\begin{pmatrix}
8 \\
-6 \\
1 \\
0
\end{pmatrix}
x_3
+
\begin{pmatrix}
-7 \\
5 \\
0 \\
1
\end{pmatrix}
x_4
$$

## 第二题

### 思路

观察$X$的形式，发现$X$是由一个对角线矩阵进行列变换后得来的。观察发现列变换矩阵是正交的，由此易得$X^{-1}$。

### 解答

$$
X=
\begin{pmatrix}
0 & a_1 & 0 & \cdots & 0 & 0 \\
0 & 0 & a_2 & \cdots & 0 & 0 \\
\cdots & \cdots & \cdots & \cdots & \cdots & \cdots \\
0 & 0 & 0 & \cdots & 0 & a_{n-1} \\
a_n & 0 & 0 & \cdots & 0 & 0
\end{pmatrix}
$$

$$
X=
\begin{pmatrix}
a_1 & 0 & \cdots & 0 & 0 & 0 \\
0 & a_2 & \cdots & 0 & 0 & 0 \\
\cdots & \cdots & \cdots & \cdots & \cdots & \cdots \\
0 & 0 & \cdots & 0 & a_{n-1} & 0 \\
0 & 0 & \cdots & 0 & 0 & a_n
\end{pmatrix}
\begin{pmatrix}
0 & 1 & 0 & \cdots & 0 & 0 \\
0 & 0 & 1 & \cdots & 0 & 0 \\
\cdots & \cdots & \cdots & \cdots & \cdots & \cdots \\
0 & 0 & 0 & \cdots & 0 & 1 \\
1 & 0 & 0 & \cdots & 0 & 0
\end{pmatrix}
$$

由于$diag\{a_1,a_2,\cdots,a_n\}$的逆矩阵是$diag\{\frac{1}{a_1},\frac{1}{a_2},\cdots,\frac{1}{a_n}\}$，且

$$
\begin{pmatrix}
0 & 1 & 0 & \cdots & 0 & 0 \\
0 & 0 & 1 & \cdots & 0 & 0 \\
\cdots & \cdots & \cdots & \cdots & \cdots & \cdots \\
0 & 0 & 0 & \cdots & 0 & 1 \\
1 & 0 & 0 & \cdots & 0 & 0
\end{pmatrix}
\begin{pmatrix}
0 & 1 & 0 & \cdots & 0 & 0 \\
0 & 0 & 1 & \cdots & 0 & 0 \\
\cdots & \cdots & \cdots & \cdots & \cdots & \cdots \\
0 & 0 & 0 & \cdots & 0 & 1 \\
1 & 0 & 0 & \cdots & 0 & 0
\end{pmatrix}^T
=E
$$

故

$$
X^{-1}=
\begin{pmatrix}
0 & 0 & \cdots & 0 & 1 \\
1 & 0 & \cdots & 0 & 0 \\
0 & 1 & \cdots & 0 & 0 \\
\cdots & \cdots & \cdots & \cdots & \cdots \\
0 & 0 & \cdots & 0 & 0 \\
0 & 0 & \cdots & 1 & 0
\end{pmatrix}
\begin{pmatrix}
\frac{1}{a_1} & 0 & \cdots & 0 & 0 & 0 \\
0 & \frac{1}{a_2} & \cdots & 0 & 0 & 0 \\
\cdots & \cdots & \cdots & \cdots & \cdots & \cdots \\
0 & 0 & \cdots & 0 & \frac{1}{a_{n-1}} & 0 \\
0 & 0 & \cdots & 0 & 0 & \frac{1}{a_n}
\end{pmatrix}
$$

$$
X^{-1}=\begin{pmatrix}
0 & 0 & \cdots & 0 & 0 & \frac{1}{a_n} \\
\frac{1}{a_1} & 0 & \cdots & 0 & 0 & 0 \\
0 & \frac{1}{a_2} & \cdots & 0 & 0 & 0 \\
\cdots & \cdots & \cdots & \cdots & \cdots & \cdots \\
0 & 0 & \cdots & 0 & \frac{1}{a_{n-1}} & 0
\end{pmatrix}
$$

### 答案

$$
X^{-1}=\begin{pmatrix}
0 & 0 & \cdots & 0 & 0 & \frac{1}{a_n} \\
\frac{1}{a_1} & 0 & \cdots & 0 & 0 & 0 \\
0 & \frac{1}{a_2} & \cdots & 0 & 0 & 0 \\
\cdots & \cdots & \cdots & \cdots & \cdots & \cdots \\
0 & 0 & \cdots & 0 & \frac{1}{a_{n-1}} & 0
\end{pmatrix}
$$

## 第三题

### 思路

用反证法证$\Rightarrow$，用数学归纳法证$\Leftarrow$。

### 解答

**证$\Rightarrow$：**

若$v_1 = 0$，则$\exist c \neq 0$使得$c \cdot v_1=0$，与$S$线性无关矛盾，故$v_1 \neq 0$。

若$v_k \in < v_1,v_2,\cdots,v_{k-1} >$，则$\exist c_1,c_2,\cdots,c_{k-1}$不全为零使$v_k=c_1 \cdot v_1 + c_2 \cdot v_2 + \cdots +c_{k-1} \cdot v_{k-1}$。
可以构造出

$$
c'_1=c_1 , c'_2=c_2 , \cdots ,c'_{k-1}=c_{k-1} , \\
c'_k=-1 , \\
c'_{k+1}=0 , \cdots , c'_m=0
$$

使$c'_1 \cdot v_1 + c'_2 \cdot v_2 + \cdots +c'_m \cdot v_m=0$
与 S 线性无关矛盾，故$v_k \notin < v_1,v _2,\cdots,v_{k-1} >$

**证$\Leftarrow$：**

基例：当 $m=1$ 时，$S = \{ v_1 \}$，由于 $v_1 \neq 0$，$S$ 线性无关。

归纳假设：假设 ${v_1, \dots, v_{k-1}}$ 线性无关（$2 \leq k \leq m$）。

归纳步骤：
设线性组合 $c_1 v_1 + \dots + c_{k-1} v_{k-1} + c_k v_k = 0$。
若 $c_k \neq 0$，则 $v_k = -\frac{c_1}{c_k} v_1 - \dots - \frac{c_{k-1}}{c_k} v_{k-1}$，即 $v_k \in \langle v_1, \dots, v_{k-1} \rangle$，与条件矛盾。故 $c_k = 0$。
于是方程化为 $c_1 v_1 + \dots + c_{k-1} v_{k-1} = 0$。由归纳假设，${v_1, \dots, v_{k-1}}$ 线性无关，故 $c_1 = \dots = c_{k-1} = 0$。
所有系数为零，故${v_1, \dots, v_k}$ 线性无关。

## 第四题

### 思路

由$|A|=1$得到$A^{-1}=A^*$，后续易证。

### 解答

$$
A^{-1}=\frac{A^*}{|A|}=A^*
$$

由$AX=B$，得

$$
X=A^{-1}B=A^*B
$$

因为$A$中的元素都为整数，故$A^*$中的元素都为整数。又$B$中的元素都为整数，故$X$中的元素都为整数。

## 第五题

### 思路

（i）证明加性和齐性
（ii）根据题意解答

### 解答

**（i）**

$$
\varphi(1,0)=(1,1,2)
$$

$$
\varphi(0,1)=(1,-1,3)
$$

故

$$
A=
\begin{pmatrix}
1 & 1 \\
1 & -1 \\
2 & 3
\end{pmatrix}
$$

证明加性：

$$
\begin{aligned}
\varphi(x_1,y_1)+\varphi(x_2,y_2)
&=(x_1+y_1,x_1-y_1,2x_1+3y_1)+(x_2+y_2,x_2-y_2,2x_2+3y_2)\\
&=((x_1+x_2)+(y_1+y_2),(x_1+x_2)-(y_1+y_2),2(x_1+x_2)+3(y_1+y_2))\\
&=\varphi(x_1+x_2,y_1+y_2)
\end{aligned}
$$

证明齐性：

$$
\begin{aligned}
c \cdot \varphi(x,y)
&=c \cdot (x+y,x-y,2x+3y) \\
&=(cx+cy,cx-cy,2cx+3cy) \\
&=\varphi(cx,cy)
\end{aligned}
$$

**（ii）**
若存在$2 \times 3$矩阵$B$使$BA=E$，则$\varphi$为单射。

$$
B=(A^TA)^{-1}A^T=
\begin{pmatrix}
\frac{1}{6} & \frac{17}{30} & \frac{2}{15} \\
0 & -\frac{2}{5} & \frac{1}{5}
\end{pmatrix}
$$

检验易证 BA=E，故$\varphi$是单射。

## 第六题

### 思路

按行拆开，递归求解。

### 解答

$$
\begin{aligned}
D_n
&=
\begin{vmatrix}
2 & -1 & 0 & 0 & \cdots & 0 & 0 & 0 \\
-1 & 2 & -1 & 0 & \cdots & 0 & 0 & 0 \\
0 & -1 & 2 & -1 & \cdots & 0 & 0 & 0 \\
\cdots & \cdots & \cdots & \cdots & \cdots & \cdots & \cdots & \cdots \\
0 & 0 & 0 & 0 & \cdots & -1 & 2 & -1\\
0 & 0 & 0 & 0 & \cdots & 0 & -1 & 2
\end{vmatrix}\\
&=
2D_{n-1}+
\begin{vmatrix}
-1 & -1 & 0 & \cdots & 0 & 0 & 0 \\
0 & 2 & -1 & \cdots & 0 & 0 & 0 \\
\cdots & \cdots & \cdots & \cdots & \cdots & \cdots & \cdots \\
0 & 0 & 0 & \cdots & -1 & 2 & -1\\
0 & 0 & 0 & \cdots & 0 & -1 & 2
\end{vmatrix}\\
&=
2D_{n-1}-D_{n-2}
\end{aligned}
$$

$D_1=2$，$D_2=3$，
$D_n-D_{n-1}=D_{n-1}-D_{n-2}=\cdots=D_2-D_1=1$，
故$D_n=n+1$。

### 答案

$$
D_n=n+1
$$

## 第七题

### 思路

根据秩通过子式的定义求解

### 解答

证$\Rightarrow$：
$A^*=O$，即$A^*$中任意元素为$0$，即$A$的任意$n-1$阶子式为 0。根据秩的子式定义，矩阵的秩是矩阵最大阶非零子式的阶数，$rank(A) < n-1$。
证$\Leftarrow$：
$rank(A) < n-1$，即$rank(A) \leq n-2$，即$A$的最大阶非零子式是$n-2$阶的，即$A$的任意$n-1$阶子式都为$0$，故$A^*$中各元素都为$0$，即$A^*=O$。

## 第八题

### 思路

（i）$A^{-1}=(A^{-1})^T=(A^T)^{-1}$
（ii）$A$可逆$\iff |A| \neq 0$

### 解答

**（i）**
$A=A^T$，故$AA^{-1}=A^TA^{-1}$，$A^{-1}A=A^{-1}A^T$，即$A^TA^{-1}=A^{-1}A^T=E$。
根据逆矩阵的定义，$(A^T)^{-1}=A^{-1}$。
又$(A^T)^{-1}=(A^{-1})^T$，故$A^{-1}=(A^{-1})^T$。

$A=-A^T$，故$AA^{-1}=-A^TA^{-1}$，$A^{-1}A=-A^{-1}A^T$，即$-A^TA^{-1}=-A^{-1}A^T=E$。
根据逆矩阵的定义，$(A^T)^{-1}=-A^{-1}$。
又$(A^T)^{-1}=(A^{-1})^T$，故$-A^{-1}=(A^{-1})^T$。

**（ii）**
$A=-A^T$，故$|A|=|-A^T|$，故$|A|=(-1)^n|A^T|=(-1)^n|A|$。
因为$n$为奇数，故$|A|=-|A|$，即$|A|=0$，此时$A$不可逆。
故不存在奇数阶的可逆斜对称矩阵。

## 第九题

### 解答

**（a）**
错误。如$det(2E)=2^n$，而$det(E)+det(E)=2$。
**（b）**
错误。设想$A$是一个非满秩矩阵，而$B$和$C$的列向量都属于 A 的零空间。此时$AB=AC=O$，而$B$和$C$不同。
**（c）**
错误。$ker(A)$是$\mathbb{R}^n$的子空间。
**（d）**
错误。矩阵乘法不交换。
如果$A$可逆，则有$det(M)=det(A)det(D-CA^{-1}B)$
**（e）**
正确。由秩零化度定理，

$$
rank(A)=rank(A^TA)  \iff n-nullity(A)=n-nullity(A^TA) \Leftarrow ker(A)=ker(A^TA)
$$

下面只需证明$ker(A)=ker(A^TA)$。

证明 1：$ker(A) \subseteq ker(A^TA)$
令$x \in ker(A)$，则$Ax=0$。

$$
A^TAx=A^Tx=0
$$

故$x \in ker(A^TA)$。
即$ker(A) \subseteq ker(A^TA)$。

证明 2：$ker(A^TA) \subseteq ker(A)$
令$x \in ker(A^TA)$，则$A^TAx=0$。

$$
x^TA^TAx=x^T(A^TAx)=x^T0=0
$$

从另一个角度看，

$$
x^TA^TAx=(x^TA^T)Ax=(Ax)^TAx=||Ax||^2
$$

故$||Ax||^2=0$，即$Ax=0$。
即$ker(A^TA) \subseteq ker(A)$。

综上，$ker(A)=ker(A^TA)$，即证得$rank(A)=rank(A^TA)$。
