# 2025 秋学期期中考试

## 1

$$
\left\{
    \begin{align*}
        \begin{matrix}
            x_1+x_2-x_3&=1\\
            x_1+ \lambda x_2+3x_3&=2 \\
            2x_1+3x_2+ \lambda x_3&=3
        \end{matrix}
    \end{align*}
\right.
$$

$\lambda$取何值时方程组无解，有唯一解与有无穷解？并求出方程组有无穷解时的通解。

## 2

$\alpha_1=(1,2,-1,1)$，$\alpha_2=(2,4,-2,2)$，$\alpha_3=(3,7,2,0)$，$\alpha_4=(1,3,4,-2)$。$V$是由$\{\alpha_1 , \alpha_2 , \alpha_3 , \alpha_4\}$组成的线性子空间。求$V$的维数，并找出$\{\alpha_1 , \alpha_2 , \alpha_3 , \alpha_4\}$的一个子集使这个子集构成 V 的一个基。

## 3

有线性映射$f: \mathbb{R}^n \rightarrow \mathbb{R}^m$，$v_i=f(u_i)$。

### (i)

证明：$v_1,v_2,\cdots,v_k$线性无关时，$u_1,u_2,\cdots,u_k$线性无关。

### (ii)

$u_1,u_2,\cdots,u_k$线性无关时，$v_1,v_2,\cdots,v_k$线性无关吗？若成立，给出证明；若不成立，举一个反例。

## 4

$\varphi(x_1,x_2,x_3,x_4)=[x_1+3x_3+x_4,4x_2+5x_4,2x_2+x_3]$

### (i)

求$\varphi$对应的矩阵$A$。

### (i)

求$ker \enspace \varphi$和$im \enspace \varphi$的维数。

## 5

$$
M(a_1,a_2,\cdots,a_{n-1})=
\begin{pmatrix}
    1 & a_1 & a_2 & a_3 & \cdots & a_{n-2} & a_{n-1} \\
    0 & 1 & 0 & 0 & \cdots & 0 & 0 \\
    0 & 0 & 1 & 0 & \cdots & 0 & 0 \\
    0 & 0 & 0 & 1 & \cdots & 0 & 0 \\
    \cdots & \cdots & \cdots & \cdots & \cdots & \cdots & \cdots \\
    0 & 0 & 0 & 0 & \cdots & 1 & 0 \\
    0 & 0 & 0 & 0 & \cdots & 0 & 1
\end{pmatrix}
$$

令$G=\{M(a_1,a_2,\cdots,a_{n-1}) | a_i \in \mathbb{R}\}$。证明$G$对乘法与取逆封闭。

## 6

求出下列行列式的值。

### (i)

$$
\begin{vmatrix}
    1 & 1 \\
    1 & 6
\end{vmatrix}
$$

### (ii)

忘了

### (iii)

$$
\begin{vmatrix}
    a_0 & -1 & 0 & 0 & \cdots & 0 & 0 \\
    a_1 & x & -1 & 0 & \cdots & 0 & 0 \\
    a_2 & 0 & x & -1 & \cdots & 0 & 0 \\
    \cdots & \cdots & \cdots & \cdots & \cdots & \cdots & \cdots \\
    a_{n-2} & 0 & 0 & 0 & \cdots & x & -1 \\
    a_{n-1} & 0 & 0 & 0 & \cdots & 0 & x
\end{vmatrix}
$$

## 7

$A$和$B$都是$n \times n$的矩阵。

### (i)

证明：$max \{ rank(A) , rank(B)\} \leq rank(A \enspace B) \leq rank(A) + rank(B)$

### (ii)

$rank(A \enspace B)$总等于$rank \begin{pmatrix}A\\B\end{pmatrix}$吗？若成立，给出证明；若不成立，举一个反例。

## 8

$$
A=
\begin{pmatrix}
    1 & 2 & 3 & \cdots & n-1 & n \\
    0 & 2 & 3 & \cdots & n-1 & n \\
    0 & 0 & 3 & \cdots & n-1 & n \\
    \cdots & \cdots & \cdots & \cdots & \cdots & \cdots \\
    0 & 0 & 0 & \cdots & n-1 & n \\
    0 & 0 & 0 & \cdots & 0 & n \\
\end{pmatrix}
$$

求$A^{-1}$与$\sum_{i=1}^{n}\sum_{j=1}^{n}A_{ij}$，其中$A_{ij}$是代数余子式。

## 9

$A$是$(n+1) \times n$矩阵，$B$是$(n+1) \times 1$矩阵。$rank(A)=n$。证明：$AX=B$有解当且仅当$det(A | B)=0$。
