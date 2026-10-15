# 线搜索

## 定义

线搜索是连续无约束数值最优化中最基础的迭代策略之一。在每次迭代中，算法先选定一个搜索方向 $p_k$，然后沿该方向寻找合适的步长 $\alpha_k > 0$，以更新当前解：

$$
x_{k+1} = x_k + \alpha_k p_k
$$

## 步长

当搜索方向已知时，线搜索中确定步长的过程本质上就是将高维空间中的多变量目标函数优化问题转化为一维参数 $\alpha$ 的单变量优化问题，即

$$
\phi(\alpha) = f(x_k + \alpha p_k)
$$

### 确定步长的策略

#### 精确线搜索

最容易想到的方法当然是求解

$$
\alpha_k = \arg\min_{\alpha > 0} f(x_k + \alpha p_k)
$$

这样做理论上能获得该方向上的最大降幅；然而，求解一维极值通常涉及多次函数值和导数计算，计算成本昂贵，且参数无界时难以求解，因此很难应用。

#### 非精确线搜索

另外一种想法是通过多次迭代，每次迭代都持续下降，最终达到一个“令人满意的位置”。这种方法不追求 $\alpha_k$ 达到绝对极小值，仅要求步长满足特定收敛准则，于是也大幅减少了计算量。实际上我们大多使用这种方法。

## 常用的确定步长准则

### Armijo 条件

为了防止步长过大导致泰勒展开效果变差，同时考虑 $f(x)$ 的计算开销成本，通常采用 Armijo 条件调整步长。

要求

$$
f(x_k + \alpha_k p_k) \le f(x_k) + c \alpha_k \nabla f(x_k)^\top p_k, \quad c \in (0, 1)
$$

当不满足上述条件时，用回溯线搜索，从 $\alpha=1$ 开始，按 $\rho \in (0, 1)$ 不断等比缩短 $\alpha_\text{new} = \rho \alpha_\text{old}$，直至满足 Armijo 条件。

可以记成

$$
\phi(\alpha_k) \leq \phi(0) + c \phi'(0) \alpha_k
$$

其中 $\phi'(0) = \nabla f(x_k)^\top p_k$。

这样看，Armijo 条件本质上就是取根据出发点和方向向量唯一确定的平面，在上面画出一条以 $\alpha_k$ 为自变量、$\phi(\alpha_k) = f(x_k + p_k \alpha_k)$ 为因变量的函数曲线，然后设定一个对斜率的宽限 $c$，使得在连续可导的情况下总能截取到一个下降步长。

![Armijo 线搜索示意图](figs/armijo_line_search.png)

### Wolfe 条件

用类似的理解方式，Wolfe 法可以理解为一个不等式组：

$$
\left\{
\begin{aligned}
\phi(\alpha_k) &\leq \phi(0) + c_1 \phi'(0) \alpha_k \\
\phi'(\alpha_k) &\geq c_2 \phi'(0)
\end{aligned}
\right.
$$

其中 $0 < c_1 < c_2 < 1$。

Wolfe 线搜索的优势在于它不会让步长一下子缩减太多，而是通过保证斜率至少大于 $c_2 \phi'(0)$ 来规定一个步长下限。

![Wolfe 线搜索示意图](figs/wolfe_line_search.png)

## 搜索方向

### 梯度下降法

要使 $\phi(\alpha)$ 下降，$p_k$ 方向上必须存在一个点使得 $\phi(\alpha^\star) < \phi(0)$。直觉上看，这个条件可以通过往梯度下降的方向走来解决，也就是使得 $\nabla f(x_k)^\top p_k < 0$。

在 $f(x_k)$ 的附近，$f(x_k + p_k)$ 可以用一阶泰勒展开近似表现为 $f(x_k + p_k) \approx f(x_k) + \nabla f(x_k)^\top p_k$。$f(x_k)$ 和 $\nabla f(x_k)$ 显然都是目前已知的常量，所以，既然我们希望能取到一个 $p_k$ 使 $f(x_k) + \nabla f(x_k)^\top p_k$ 最小，我们就应该让 $\nabla f(x_k)^\top p_k$ 尽可能小，也就是让 $p_k$ 的方向正好与 $\nabla f(x_k)$ 相反。

这里的 $f(x_k + p_k)$ 只是一个简化模型。而实际上，由于我们在 $f(x_k + \alpha_k p_k)$ 中还提供了一个额外的步长变量 $\alpha_k$，所以 $p_k$ 并不一定要满足 $\Vert p_k \Vert = 1$，直接取 $p_k = -\nabla f(x_k)$ 就好。

#### Lipschitz 连续性约束

Lipschitz 连续条件，又叫 L-光滑，是数学上用来限制函数变化速度的一个光滑性条件。它强于“连续性”，但又弱于“可微性”。数学上，它的定义是

$$
\vert\nabla f(x) - \nabla f(y)\vert \le L \vert x - y \vert, \quad \forall x, y \in \mathbb{R}^n \quad (L > 0)
$$

这个条件可以约束出一个所谓的“最优步长”。让我们一起来推理一下：

我们有微积分基本定理：

$$
f(y) = f(x) + \int_0^1 \nabla f^\top(x + t(y - x)) (y - x) dt
$$

改写一下，变成

$$
f(y) = f(x) + \nabla^\top f(x) (y - x) + \int_0^1 (\nabla f(x + t(y - x)) - \nabla f(x))^\top (y - x) dt
$$

由柯西不等式和 Lipschitz 条件，积分中的

$$
\begin{aligned}
&(\nabla f(x + t(y - x)) - \nabla f(x))^\top (y - x) \\
\le & \Vert \nabla f(x + t(y - x)) - \nabla f(x) \Vert \cdot \Vert y - x \Vert \\
\le & L t \Vert y-x \Vert^2
\end{aligned}
$$

于是得到

$$
f(y) \le f(x) + \nabla f^\top(x) (y - x) + \frac{L}{2} \Vert{}y - x\Vert{}^2
$$

在我们的语境中，$x = x_k$，$y = x_{k+1} = x_k - \alpha \nabla f(x_k)$，所以带入后就得到

$$
f(x_{k+1}) \le f(x_k) - \alpha_k \left( 1 - \frac{\alpha L}{2} \right) \Vert \nabla f(x_k) \Vert^2
$$

要让 $f(x_{k+1}) < f(x_k)$，就要让 $\alpha_k \left(1 - \frac{\alpha_k L}{2}\right) > 0$。已知 $\alpha_k > 0$，所以保证单调下降的步长区间就是 $0 < \alpha_k < \frac{2}{L}$。

我们也可以把 $\alpha_k$ 看作自变量，那么单步下降量就是关于 $\alpha_k$ 的二次函数 $g(\alpha_k) = \alpha_k - \frac{L}{2} \alpha_k^2$。所以，理论上来说，$\alpha_k = \frac{1}{L}$ 时，单步理论下降量达到最大。也因此，标准梯度下降算法在具有 $L$-Lipschitz 梯度的目标函数上通常直接选取 $\alpha = \frac{1}{L}$。

> Lipschitz 条件看似是一个单纯的“数学游戏”，一个莫名其妙冒出来的奇怪条件，但其实在真实场景中很常见，是现代一阶数值优化与机器学习理论中最核心、最普适的基础假设。Hessian 矩阵有界的函数、二次优化问题 $f(x) = \frac{1}{2} \Vert Ax - b \Vert _2^2$、逻辑回归等都天然满足 Lipschitz 条件。

#### 收敛速度

收敛速度有两种表示方式，一种是收敛 $k$ 步能把误差控制到多少以内，另一种是收敛到 $\epsilon$ 精度以内需要多少步。其实这两种表示方法完全可以通过 $f(k)=\epsilon$、$k=g(\epsilon)$ 的公式相互转换，比如说 $O(\frac{1}{k})$ 就是 $O(\frac{1}{\epsilon})$，$O(\frac{1}{k^2})$ 就是 $O(\frac{1}{\sqrt{\epsilon}})$，等等。

由于凸函数与 Lipschitz 条件都是很常见的条件，我们就在这两个条件下进行分析。步长就取上面提到的理论最优步长 $\alpha_k = \frac{1}{L}$。所以我们有

$$
f(x_{k+1}) \le f(x_k) - \frac{1}{2L} \Vert \nabla f(x_k) \Vert^2
$$

又因为凸性，我们有

$$
f(x^\star) \ge f(x_k) + \nabla f^\top (x_k) (x^\star - x_k)
$$

所以

$$
f(x_k) - f(x^\star) \le \nabla f^\top (x_k) (x_k - x^\star) \le \Vert \nabla f(x_k) \Vert \cdot \Vert x_k - x^\star \Vert
$$

$$
\Vert \nabla f(x_k) \Vert \ge \frac{f(x_k) - f(x^\star)}{\Vert x_k - x^\star \Vert}
$$

带入 Lipschitz 条件得到的公式，我们就得到

$$
f(x_{k+1}) \le f(x_k) - \frac{1}{2L} \left( \frac{f(x_k) -  f(x^\star)}{\Vert x_k - x^\star \Vert} \right) ^2
$$

变形得

$$
f(x_{k+1}) - f(x^\star) \le f(x_k) - f(x^\star) - \frac{1}{2L} \left( \frac{f(x_k) -  f(x^\star)}{\Vert x_k - x^\star \Vert} \right) ^2
$$

把误差记为 $\Delta_k = f(x_k) - f(x^*)$，则有

$$
\Delta_{k+1} \le \Delta_k - \frac{1}{2L \Vert x_k - x^\star \Vert^2} \Delta_k^2
$$

利用 $\Delta_{k+1} \le \Delta_k$，

$$
\frac{1}{\Delta_{k+1}} - \frac{1}{\Delta_k} \ge \frac{\Delta_k}{\Delta_{k+1} \cdot 2L \Vert x_k - x^\star \Vert^2} \ge \frac{1}{2L \Vert x_k - x^\star \Vert^2}
$$

累加，得

$$
\frac{1}{\Delta_k} \ge \frac{1}{\Delta_0} + \frac{k}{2L \max \Vert x_k - x^\star \Vert^2} \ge \frac{k}{2L \max \Vert x_k - x^\star \Vert^2}
$$

可见 $\Delta_k=O\left( \frac{1}{k} \right)$。用另外一种表示方法，就是 $O\left(\frac{1}{\epsilon}\right)$。这时的收敛阶数属于次线性收敛。

当然，一些时候函数会满足一些更强的条件。比如说，在 $\mu$-强凸且 $L$-光滑时，收敛速度就是 $f(x_k) - f^* \le O\left(\left(1 - \frac{\mu}{L}\right)^k\right)$，或者说 $O\left(\frac{L}{\mu} \log \frac{1}{\epsilon}\right)$ 的，属于线性收敛。而在更弱的非凸但 $L$-光滑条件下，收敛速度就是 $O\left( \frac{1}{\sqrt{k}} \right)$ 或者说 $O \left( \frac{1}{\epsilon^2} \right)$ 的，属于次线性收敛。（这两种条件下的证明见附录）

#### 算法瓶颈分析

##### 高条件数下的缺陷

当目标函数的等高线具有极高条件数时，圆在特征空间中被拉伸为细长椭圆，负梯度方向与通往全局最小值的方向近乎正交。

##### 精确线搜索下的特点

精确线搜索下，步长 $\alpha_k$ 满足 $\phi'(\alpha_k) = 0$，即 $\nabla f(x_{k+1})^\top p_k = 0$。

结合 $p_{k+1} = -\nabla f(x_{k+1})$ 和 $\nabla f(x_{k+1})^\top p_k = 0$，可得 $p_{k+1}^\top p_k = -\nabla f(x_{k+1})^\top p_k = 0$。

由于 $p_{k+1}^\top p_k = 0$，梯度下降轨迹在几何上很容易走出 $90^\circ$ 的直角折线。算法在狭长山谷两侧的峭壁间剧烈横跳，而沿低斜率山谷底部的推进极其缓慢，导致收敛停滞。

![最速下降法 + 精确线搜索的锯齿轨迹](figs/steepest_descent_trajectory.png)

### 牛顿法

在梯度下降法中，我们只使用了一阶的泰勒展开信息，那如果使用二阶信息呢？

$$
f(x_k + p_k) \approx f(x_k) + \nabla f(x_k)^\top p_k + \frac 1 2 p_k^\top \nabla^2 f(x_k) p_k
$$

我们将其视作一个关于 $p_k$ 的函数，那么我们得到 $\psi(p_k) = \frac 1 2 p_k^\top \nabla^2 f(x_k) p_k + \nabla f(x_k)^\top p_k + f(x_k)$。我们希望 $\psi(p_k)$ 最小，那么就肯定有 $\nabla \psi(p_k) = 0$，即

$$
\nabla^2 f(x_k) p_k + \nabla f(x_k) = 0
$$

另外，如果 $\psi(p_k)$ 存在唯一最小值，那么它的二次项系数 $\nabla^2 f(x_k)$ 必须是正定的。而正定矩阵总满秩，所以当 $\nabla^2 f(x_k)$ 正定时，搜索方向唯一确定：

$$
p_k^{\text{Newton}} = - \left( \nabla^2 f(x_k) \right)^{-1} \nabla f(x_k)
$$

#### 收敛速度

利用一阶泰勒展开和积分中值定理，梯度 $\nabla f(x^\star)$ 在 $x_k$ 处的展开式为

$$
0 = \nabla f(x^\star) = \nabla f(x_k) + \int_0^1 \nabla^2 f(x_k + t(x^\star - x_k)) (x^\star - x_k) dt
$$

将牛顿步 $\nabla f(x_k) = \nabla^2 f(x_k)(x_k - x_{k+1})$ 代入：

$$
\nabla^2 f(x_k)(x_{k+1} - x^\star) = \int_0^1 \left[ \nabla^2 f(x_k + t(x^\star - x_k)) - \nabla^2 f(x_k) \right] (x^\star - x_k) \, dt
$$

假设 Hessian 矩阵满足 $L_2$-Lipschitz 连续（即 $\Vert \nabla^2 f(x) - \nabla^2 f(y) \Vert \le L_2 \Vert x - y\Vert$），且最优解附近存在逆上界 $\Vert \nabla^2 f(x_k)^{-1} \Vert \le M_0$：

$$
\Vert x_{k+1} - x^\star \Vert \le M_0 \int_0^1 L_2 \cdot t \Vert x_k - x^\star \Vert^2 dt = \frac{M_0 L_2}{2} \Vert x_k - x^\star \Vert^2
$$

令 $C = \frac{M_0 L_2}{2}$，则有

$$
\Vert x_{k+1} - x^*\Vert \le C \Vert x_k - x^*\Vert^2
$$

所以，牛顿法的收敛速度是二阶收敛的。

#### 算法瓶颈分析

##### 时间复杂度

Hessian 矩阵有 $\frac{n(n+1)}{2}$ 个独立的元素，所以计算海森矩阵至少要 $O(n^2)$ 时间。计算单个二阶偏导数 $\frac{\partial^2 f}{\partial x_i \partial x_j}$ 的时间取决于目标函数表达式的规模 $T_{\text{eval}}$。解析求导或自动微分计算完整 Hessian 矩阵的单步耗时通常为 $O(n^2 \cdot T_{\text{eval}})$；若采用有限差分近似，计算 $n^2$ 个元素需要评估 $O(n)$ 次梯度或 $O(n^2)$ 次函数值，计算成本远高于简单的 $O(n^2)$。

另外，求解线性方程组需经过 Cholesky 分解或 LU 分解，单步耗时为 $O(n^3)$。

无论如何，时间复杂度至少是 $O(n^3)$ 的。

##### 空间复杂度

Hessian 矩阵有 $\frac{n(n+1)}{2}$ 个独立的元素，所以存储每次的 Hessian 矩阵要 $O(n^2)$ 空间。

##### 非正定性缺陷

当 $\nabla^2 f(x_k)$ 出现负特征值，甚至为不可逆矩阵时，$p_k$ 可能指向鞍点或极大值。当 $\lambda_\text{min} \leq 0$ 时，工程上一般将精确 Hessian 矩阵换为 $\nabla^2 f(x_k) + \mu I, \quad \mu > -\lambda_\text{min} + \varepsilon$，通过牺牲拟合的精细度来保证解的唯一性。

### 拟牛顿法

牛顿法要存的 Hessian 矩阵实在是太占地方了，解起来也很耗时。而且考虑到复杂地形中没法做到处处正定，牛顿法就不得不在曲率上做出很多妥协，把它硬掰成正定的，也拟合的没有很好。

改良的想法是我们干脆不算 Hessian 矩阵了，而是使用**相邻迭代点处的一阶梯度差**来积累曲率信息。反正一阶展开还是准的，二阶曲率的准确性要求倒也没那么迫切了。或者说，在工程上，我们愿意多牺牲一点二阶曲率来换取更快的时间和更少的空间。

根据这种思想，我们构造近似模型 $f(x_k + p_k) \approx m_{k}(p_k) = f(x_k) + \nabla f(x_k)^\top p_k + \frac{1}{2} p_k^\top B_k p_k$。接下来的任务就是求出一个合适的 $B_k$。

#### 割线约束

为使模型包含历史观测信息，强制要求该二次模型在 $x_{k-1}$ 处的预测梯度与当时实际计算出的真实梯度 $\nabla f(x_{k-1})$ 一致，则有

$$
\nabla m_k(p_k^\star) = \nabla f(x_{k-1}), \quad p_k^\star = x_{k-1} - x_k
$$

化简，得到

$$
\nabla f(x_k) + B_k (x_{k-1} - x_k) = \nabla f(x_{k-1})
$$

$$
\nabla f(x_k) - \nabla f(x_{k-1}) = B_k (x_k - x_{k-1})
$$

习惯记法令 $s_{k-1} = x_k - x_{k-1}$，$y_{k-1} = \nabla f(x_k) - \nabla f(x_{k-1})$，故上式也可记为：

$$
y_{k-1} = B_k s_{k-1}
$$

为了求出一个唯一的 $p_k$ 让 $m_k(p_k)$ 最小，我们肯定要保证 $B_k$ 是可逆的。记 $H_k = B_k^{-1}$，所以有

$$
s_{k-1} = H_k y_{k-1}
$$

这是最通用的一种表示方法，也是 BFGS 使用的表示方法。值得注意的是，这里的 $H_k$ 和 Hessian 矩阵一点关系都没有！

#### 最小扰动

然而，$y_{k-1} = B_k s_{k-1}$ 只表现了 $B_k$ 需要满足什么约束，却无法求出 $B_k$ 是什么。因为 $n \times n$ 对称矩阵包含 $\frac{n(n+1)}{2}$ 个独立自由度，而割线方程仅提供 $n$ 个独立等式约束，所以解空间是不定的。

于是我们再提出一个假设。我们希望算法在迭代过程中数值保持平稳变化，所以我们希望曲率信息每次不能改变太多，这就是最小扰动假设。

说是最小扰动，可我们要最小化什么扰动呢？有人认为，我们可以让 $B_k = B_{k-1} + \sigma v v^\top$，也就是加上一个对称的秩一矩阵。这样未知量就变成了 $\sigma$ 和 $v$，满足割线方程的解是唯一的。这种方法叫 SR1 法。然而，SR1 法无法保证正定性，还可能出现分母趋近于零的数值崩溃。

后来又有一种叫 DFP 的算法，通过最小化 $B_k$ 的 Frobenius 范数来求出 $B_k$。不过我们发现，最后我们是要求方向 $p_k$ 的，而 $p_k = - B_k^{-1} \nabla f(x_k) = -H_k \nabla f(x_k)$，所以为求平稳我们最小化 $\Vert H - H_k \Vert_{W}^2$ 就好。

#### BFGS

这就是 BFGS 算法，即

$$
\min_{H_k} \Vert H_k - H_{k-1} \Vert_{W}^2 \quad \text{s.t.} \quad H_k = H_k^\top, \quad H_k y_{k-1} = s_{k-1}
$$

> 有趣的是，DFP 算法要求 $\min_{B_k} \Vert B_k - B_{k-1} \Vert_{W}^2 \quad \text{s.t.} \quad B_k = B_k^\top, \quad B_k s_{k-1} = y_{k-1}$，与 BFGS 完全对偶。

我们跳过复杂的证明过程（见附录），直接给出结论：

$$
H_k= H_{k-1} - \frac{s_{k-1} y_{k-1}^\top H_{k-1} + H_{k-1} y_{k-1} s_{k-1}^\top}{y_{k-1}^\top s_{k-1}} + \frac{s_{k-1} s_{k-1}^\top}{y_{k-1}^\top s_{k-1}} + \frac{y_{k-1}^\top H_{k-1} y_{k-1}}{(y_{k-1}^\top s_{k-1})^2} s_{k-1} s_{k-1}^\top
$$

令 $\rho_{k-1} = \frac{1}{y_{k-1}^\top s_{k-1}}$，将其重组因式分解，即得

$$
H_k = (I - \rho_{k-1} s_{k-1} y_{k-1}^\top) H_{k-1} (I - \rho_{k-1} y_{k-1} s_{k-1}^\top) + \rho_{k-1} s_{k-1} s_{k-1}^\top
$$

$H_k \succ 0$ 的充要条件是 $y_{k-1}^\top s_{k-1} > 0$（证明见附录），而这个条件可以由 Wolfe 线搜索保证（证明见附录），所以 BFGS 一般配合 Wolfe 线搜索使用。

##### 收敛速度

根据 Dennis-Moré 准则（内容与证明见附录），一个拟牛顿法在极小值点邻域产生超线性收敛的充分必要条件是搜索方向渐近地逼近牛顿方向，即：

$$
\lim_{k \to \infty} \frac{\Vert (B_k - \nabla^2 f(x^\star)) s_k \Vert}{\Vert s_k \Vert} = 0
$$

在严格凸二次展开且步长通过 Wolfe 条件进行线搜索的条件下，BFGS 更新公式构造的对称正定矩阵序列 $\{B_k\}$ 严格满足 Dennis-Moré 准则，因此其收敛速度满足：

$$
\lim_{k \to \infty} \frac{\Vert x_{k+1} - x^\star \Vert}{\Vert x_k - x^\star \Vert} = 0
$$

因此，BFGS 是超线性收敛的。

##### 算法复杂度分析

###### 时间复杂度

或许你会说，这个地方要用很大的一个矩阵乘法从 $H_{k-1}$ 更新到 $H_k$，不还是 $O(n^3)$ 的复杂度吗？但这里的低秩结构帮了大忙。回看那个因式分解前的式子，你会发现它只有一个真正的矩阵 $H_{k-1}$。又考虑到矩阵的乘法结合律 $ABC = (AB)C = A(BC)$，这样的结构当然可以拆成多个矩阵乘向量的形式。也就是说，它实质上是 $O(n^2)$ 的！

###### 空间复杂度

要记录的最大的东西只有一个 $n \times n$ 的矩阵 $H$，所以空间复杂度是 $O(n^2)$。

#### L-BFGS

然而，有些时候 $n$ 实在是太大了。当它到达千万或上亿级别的时候，即使 $O(n^2)$ 的时间复杂度与空间复杂度也是不可接受的。

让我们来回顾一下因式分解后的 BFGS 逆 Hessian 更新公式：

$$
H_k = (I - \rho_{k-1} s_{k-1} y_{k-1}^\top) H_{k-1} (I - \rho_{k-1} y_{k-1} s_{k-1}^\top) + \rho_{k-1} s_{k-1} s_{k-1}^\top
$$

引入紧凑算子 $V_{k-1} = I - \rho_{k-1} s_{k-1} y_{k-1}^\top$ 来简化运算，所以 $H_k = V_{k-1}^\top H_{k-1} V_{k-1} + \rho_{k-1} s_{k-1} s_{k-1}^\top$。

我们可以向前继续追溯，把 $H_{k-1}$ 用 $H_{k-2}$ 表示，有

$$
\begin{aligned}
H_k
&= V_{k-1}^\top \left( V_{k-2}^\top H_{k-2} V_{k-2} + \rho_{k-2} s_{k-2} s_{k-2}^\top \right) V_{k-1} + \rho_{k-1} s_{k-1} s_{k-1}^\top \\
&= V_{k-1}^\top V_{k-2}^\top H_{k-2} V_{k-2} V_{k-1} + \rho_{k-2} V_{k-1}^\top s_{k-2} s_{k-2}^\top V_{k-1} + \rho_{k-1} s_{k-1} s_{k-1}^\top \\
\end{aligned}
$$

如果一直往前展开，最终会追溯到初始矩阵 $H_0$。这意味着 $H_k$ 中其实“蕴含”着之前所有步的信息。但随着迭代步数增加，历史越久远的曲率信息对当前的参考价值越低，反而占用巨量内存。

L-BFGS 的思想是：只保留最近 $m$ 步的历史向量对（通常实践中取 $m \in [3, 20]$）。在第 $k$ 步时，仅利用最近的 $m$ 个历史位移对 ${s_i, y_i},\quad i = k-1, k-2, \cdots, k-m$。当回退到 $k-m$ 时，不再回溯。展开式为：

$$
\begin{aligned}
H_k
= &\left( V_{k-1}^\top \cdots V_{k-m}^\top \right) H_k^\text{init} \left( V_{k-m} \cdots V_{k-1} \right) \\
&+ \rho_{k-m} \left( V_{k-1}^\top \cdots V_{k-m+1}^\top \right) s_{k-m} s_{k-m}^\top \left( V_{k-m+1} \cdots V_{k-1} \right) \\
&+ \cdots \\
&+ \rho_{k-1} s_{k-1} s_{k-1}^\top
\end{aligned}
$$

##### 复杂度优化

###### 逆序循环

直接计算 $H_k$ 仍然需要 $O(n^2)$ 的时间与空间，但是我们的目标是 $p_k = H_k \nabla f(x_k)$。

观察 $V_i$ 作用于任意向量 $q$ 的效果：

$$
V_i q = (I - \rho_i y_i s_i^\top) q = q - \rho_i y_i (s_i^\top q) = q - (\rho_i s_i^\top q) y_i
$$

这意味着 $V_i$ 的作用等效于在 $q$ 上减去一个沿着 $y_i$ 方向的分量，而这一操作是 $O(n)$ 的。因此，$V_{k-m} \cdots V_{k-1} \nabla f(x_k)$ 在时间上是 $O(mn)$ 的。而且，因为我们只需要存储最新 $m$ 步的 ${s_i, y_i}$，所以在空间上这也是 $O(mn)$ 的。

###### 基底变换

在经过一系列变换后，一个向量终于来到了 $H_k^\text{init}$ 前。那是不是总要维护一个 $O(n^2)$ 的矩阵呢？

不是这样的。L-BFGS 都已经为空间舍弃了这么多信息，也不在乎再多丢一点了。所以 $H_k^\text{init}$ 通常直接取为当前局部尺度的标量矩阵：

$$
H_k^\text{init} = \gamma_k I, \quad \gamma_k = \frac{s_{k-1}^\top y_{k-1}}{y_{k-1}^\top y_{k-1}}
$$

所以这一步还是 $O(n)$ 的。

###### 正序循环

类似的，$V_i^\top$ 作用于任意向量 $q$ 也是 $O(n)$ 的。所以，这里也是一个总计 $O(mn)$ 的操作。

综上所述，L-BFGS 在时间和空间上都是 $O(mn)$ 的。

##### 收敛速度

L-BFGS 丢弃了 $m$ 步之前的曲率信息。因此，近似 Hessian 矩阵无法随着 $k \to \infty$ 充分积累全空间的二阶几何特征，此时 Dennis-Moré 条件不再成立。所以理论上其渐近收敛速度退化为线性收敛，其收缩系数由保存的步数 $m$ 和函数的条件数决定。

## 附录

### 梯度下降在两种条件下的收敛速度

设 $f$ 可微，取固定步长 $\alpha_k = \frac{1}{L}$（与正文“收敛速度”一节的取法一致）。由正文的 $L$-光滑性分析已经得到下降引理

$$
f(x_{k+1}) \le f(x_k) - \frac{1}{2L} \Vert \nabla f(x_k) \Vert^2. \tag{A}
$$

下面分别讨论“$\mu$-强凸且 $L$-光滑”与“非凸但 $L$-光滑”两种情形。记 $f^\star = \min f$（对非凸情形取 $f^\star = \inf f$），$x^\star$ 为任一极小值点，并记误差 $\Delta_k = f(x_k) - f^\star \ge 0$。

#### 非凸且 $L$-光滑：$O\left(\frac{1}{\sqrt{k}}\right)$

把 (A) 从 $i = 0$ 累加到 $i = k$，左端是一个望远镜求和：

$$
\frac{1}{2L} \sum_{i=0}^k \Vert \nabla f(x_i) \Vert^2
\le \sum_{i=0}^k \big( f(x_i) - f(x_{i+1}) \big)
= f(x_0) - f(x_{k+1}) \le f(x_0) - f^\star.
$$

因此

$$
\min_{0 \le i \le k} \Vert \nabla f(x_i) \Vert^2
\le \frac{1}{k+1} \sum_{i=0}^k \Vert \nabla f(x_i) \Vert^2
\le \frac{2L \big( f(x_0) - f^\star \big)}{k+1},
$$

即

$$
\min_{0 \le i \le k} \Vert \nabla f(x_i) \Vert
\le \sqrt{\frac{2L \big( f(x_0) - f^\star \big)}{k+1}}
= O\left( \frac{1}{\sqrt{k}} \right).
$$

要让 $\min_i \Vert \nabla f(x_i) \Vert \le \epsilon$，只需 $k + 1 \ge \frac{2L (f(x_0) - f^\star)}{\epsilon^2}$，故迭代次数为 $O\left( \frac{1}{\epsilon^2} \right)$，属于次线性收敛。这也说明：在非凸情形下，梯度下降只能保证梯度范数的一个**子序列**趋于零，而无法保证函数值本身收敛到某个特定的 $f^\star$。

#### $\mu$-强凸且 $L$-光滑：线性收敛

**第一步：把函数值误差用梯度范数控制。** $\mu$-强凸性等价于对任意 $x, y$ 有

$$
f(y) \ge f(x) + \nabla f(x)^\top (y - x) + \frac{\mu}{2} \Vert y - x \Vert^2.
$$

在 $x = x_k$、$y = x^\star$ 处使用，并注意 $\nabla f(x^\star) = 0$：

$$
f(x^\star) \ge f(x_k) + \nabla f(x_k)^\top (x^\star - x_k) + \frac{\mu}{2} \Vert x^\star - x_k \Vert^2,
$$

移项得

$$
f(x_k) - f^\star \le \nabla f(x_k)^\top (x_k - x^\star) - \frac{\mu}{2} \Vert x_k - x^\star \Vert^2.
$$

令 $d = x_k - x^\star$，右端是关于 $d$ 的二次函数，其最大值为 $\frac{1}{2\mu} \Vert \nabla f(x_k) \Vert^2$（在 $d = \frac{1}{\mu} \nabla f(x_k)$ 处取得）。于是

$$
f(x_k) - f^\star \le \frac{1}{2\mu} \Vert \nabla f(x_k) \Vert^2,
\qquad\text{即}\qquad
\Vert \nabla f(x_k) \Vert^2 \ge 2\mu \, \Delta_k. \tag{B}
$$

**第二步：递推收缩。** 把 (B) 代入 (A)：

$$
\Delta_{k+1} \le \Delta_k - \frac{1}{2L} \Vert \nabla f(x_k) \Vert^2
\le \Delta_k - \frac{\mu}{L} \Delta_k
= \left( 1 - \frac{\mu}{L} \right) \Delta_k.
$$

由于 $\mu \le L$，收缩因子满足 $1 - \frac{\mu}{L} \in [0, 1)$。递推展开得

$$
\Delta_k \le \left( 1 - \frac{\mu}{L} \right)^k \Delta_0,
\qquad\text{即}\qquad
f(x_k) - f^\star = O\left( \left( 1 - \frac{\mu}{L} \right)^k \right).
$$

**第三步：迭代复杂度。** 要求 $\Delta_k \le \epsilon$，只需

$$
\left( 1 - \frac{\mu}{L} \right)^k \Delta_0 \le \epsilon
\quad\Longleftrightarrow\quad
k \ge \frac{\log (\Delta_0 / \epsilon)}{\log \left( \frac{1}{1 - \mu / L} \right)}.
$$

利用 $\log \frac{1}{1 - t} \ge t \ (0 \le t < 1)$，有 $\frac{1}{\log (1 / (1 - \mu / L))} \le \frac{L}{\mu}$，于是

$$
k = O\left( \frac{L}{\mu} \log \frac{1}{\epsilon} \right).
$$

这正是正文所说的线性收敛。其中比值 $\kappa = \frac{L}{\mu}$ 即条件数：条件数越大，收缩因子 $1 - \frac{1}{\kappa}$ 越接近 $1$，迭代次数越多。

> **补充：** $\mu$-强凸性也蕴含梯度的强单调性，即对任意 $x, y$ 有 $(\nabla f(x) - \nabla f(y))^\top (x - y) \ge \mu \Vert x - y \Vert^2$。取 $y = x^\star$ 并用柯西不等式，可得 $\Vert \nabla f(x) \Vert \ge \mu \Vert x - x^\star \Vert$；它与 $L$-光滑给出的 $\Vert \nabla f(x) \Vert \le L \Vert x - x^\star \Vert$ 一起，把函数值误差、梯度范数与到极小值点的距离三者互相控制起来。

### BFGS 的 $H_k$ 推导

BFGS 的目标是在所有满足对称性与割线约束

$$
H_k = H_k^\top, \qquad H_k y_{k-1} = s_{k-1}
$$

的矩阵中，挑出一个与上一步的 $H_{k-1}$ 差别最小的。这里的“差别”用带权 Frobenius 范数度量：

$$
\Vert A \Vert_W^2 = \operatorname{tr}\!\left( W^{1/2} A W A W^{1/2} \right) = \Vert W^{1/2} A W^{1/2} \Vert_F^2
$$

权重 $W$ 取任意满足 $W s_{k-1} = y_{k-1}$ 的对称正定矩阵（可以理解为割线意义下曲率的“替身”）。于是优化问题写成

$$
\min_{H_k} \Vert H_k - H_{k-1} \Vert_W^2 \quad \text{s.t.} \quad H_k = H_k^\top, \quad H_k y_{k-1} = s_{k-1}
$$

#### 坐标变换

记增量矩阵 $E = H_k - H_{k-1}$，并作无量纲化变换

$$
\hat E = W^{1/2} E W^{1/2}, \qquad \hat H_{k-1} = W^{1/2} H_{k-1} W^{1/2}
$$

目标函数就化为普通的 Frobenius 范数平方 $\Vert \hat E \Vert_F^2 = \operatorname{tr}(\hat E^2)$。再处理约束：把 $E y_{k-1} = s_{k-1} - H_{k-1} y_{k-1}$ 两边左乘 $W^{1/2}$，并在左边插入 $I = W^{-1/2} W^{1/2}$，得

$$
W^{1/2} E W^{1/2} \, W^{-1/2} y_{k-1} = W^{1/2} s_{k-1} - W^{1/2} H_{k-1} y_{k-1}
$$

由 $W s_{k-1} = y_{k-1}$ 可知 $W^{1/2} s_{k-1} = W^{-1/2} y_{k-1}$。令

$$
v = W^{-1/2} y_{k-1} = W^{1/2} s_{k-1}, \qquad r = v - \hat H_{k-1} v
$$

则约束变为 $\hat E v = r$，原问题简化为对称矩阵空间上的标准问题

$$
\min_{\hat E = \hat E^\top} \frac{1}{2} \operatorname{tr}\!\left( \hat E^2 \right) \quad \text{s.t.} \quad \hat E v = r
$$

#### 求解子问题

构造拉格朗日函数，其中 $\lambda \in \mathbb{R}^n$ 为乘子向量：

$$
\mathcal{L}(\hat E, \lambda) = \frac{1}{2} \operatorname{tr}\!\left( \hat E^2 \right) - \lambda^\top (\hat E v - r)
$$

利用 $\lambda^\top \hat E v = \operatorname{tr}\!\left( \hat E \cdot \frac{v \lambda^\top + \lambda v^\top}{2} \right)$，对对称矩阵 $\hat E$ 求梯度并令其为零：

$$
\frac{\partial \mathcal{L}}{\partial \hat E} = \hat E - \frac{1}{2}(v \lambda^\top + \lambda v^\top) = 0
\quad \Longrightarrow \quad
\hat E = \frac{1}{2}(v \lambda^\top + \lambda v^\top)
$$

代回约束：

$$
\frac{1}{2}\left( (\lambda^\top v) v + \Vert v \Vert^2 \lambda \right) = r
$$

两边左乘 $v^\top$，得 $\lambda^\top v = \dfrac{v^\top r}{\Vert v \Vert^2}$，再回代解出

$$
\lambda = \frac{2}{\Vert v \Vert^2} r - \frac{v^\top r}{\Vert v \Vert^4} v
$$

因此

$$
\hat E = \frac{r v^\top + v r^\top}{\Vert v \Vert^2} - \frac{v^\top r}{\Vert v \Vert^4} v v^\top
$$

将 $r = v - \hat H_{k-1} v$ 代入并整理，即

$$
\hat E = \frac{v v^\top}{\Vert v \Vert^2} - \frac{\hat H_{k-1} v v^\top + v v^\top \hat H_{k-1}}{\Vert v \Vert^2} + \frac{v^\top \hat H_{k-1} v}{\Vert v \Vert^4} v v^\top
$$

由 $\hat H_k = \hat H_{k-1} + \hat E$，得到变换空间中的更新式

$$
\hat H_k = \hat H_{k-1} - \frac{\hat H_{k-1} v v^\top + v v^\top \hat H_{k-1}}{\Vert v \Vert^2} + \frac{v v^\top}{\Vert v \Vert^2} + \frac{v^\top \hat H_{k-1} v}{\Vert v \Vert^4} v v^\top
$$

#### 逆变换还原

由 $H_k = W^{-1/2} \hat H_k W^{-1/2}$，逐项还原：

$$
\Vert v \Vert^2 = y_{k-1}^\top s_{k-1}, \qquad
W^{-1/2} v = s_{k-1}, \qquad
W^{-1/2} \hat H_{k-1} v = H_{k-1} y_{k-1}, \qquad
v^\top \hat H_{k-1} v = y_{k-1}^\top H_{k-1} y_{k-1}
$$

代回后即得

$$
H_k = H_{k-1} - \frac{s_{k-1} y_{k-1}^\top H_{k-1} + H_{k-1} y_{k-1} s_{k-1}^\top}{y_{k-1}^\top s_{k-1}} + \frac{s_{k-1} s_{k-1}^\top}{y_{k-1}^\top s_{k-1}} + \frac{y_{k-1}^\top H_{k-1} y_{k-1}}{(y_{k-1}^\top s_{k-1})^2} s_{k-1} s_{k-1}^\top
$$

整个过程里权重 $W$ 被完全消去，说明结果与 $W$ 的选取无关。代入 $\rho_{k-1} = \frac{1}{y_{k-1}^\top s_{k-1}}$ 并重组因式分解，就得到正文中使用的紧凑形式

$$
H_k = (I - \rho_{k-1} s_{k-1} y_{k-1}^\top) H_{k-1} (I - \rho_{k-1} y_{k-1} s_{k-1}^\top) + \rho_{k-1} s_{k-1} s_{k-1}^\top
$$

### $H_k \succ 0$ 的充要条件

设 $H_{k-1} \succ 0$，则 $H_k \succ 0$ 当且仅当 $y_{k-1}^\top s_{k-1} > 0$。

#### 割线方程恒成立

先验证 $H_k y_{k-1} = s_{k-1}$。由 $\rho_{k-1} y_{k-1} s_{k-1}^\top y_{k-1} = \rho_{k-1} (y_{k-1}^\top s_{k-1}) y_{k-1} = y_{k-1}$ 可知

$$
(I - \rho_{k-1} y_{k-1} s_{k-1}^\top) y_{k-1} = 0
$$

将这一结果代入因式分解式：

$$
H_k y_{k-1} = (I - \rho_{k-1} s_{k-1} y_{k-1}^\top) H_{k-1} \cdot 0 + \rho_{k-1} s_{k-1} (s_{k-1}^\top y_{k-1}) = s_{k-1}
$$

可见因式分解式自动满足割线方程。

#### 必要性

若 $H_k \succ 0$，取 $z = y_{k-1}$（由 $H_k y_{k-1} = s_{k-1}$ 且 $H_k$ 可逆可知 $y_{k-1} \neq 0$），有

$$
y_{k-1}^\top H_k y_{k-1} = y_{k-1}^\top s_{k-1}
$$

左边是正定矩阵的二次型，恒为正，于是 $y_{k-1}^\top s_{k-1} > 0$。

#### 充分性

设 $y_{k-1}^\top s_{k-1} > 0$，则 $\rho_{k-1} > 0$。对任意非零向量 $z$，由因式分解式

$$
z^\top H_k z = w^\top H_{k-1} w + \rho_{k-1} (s_{k-1}^\top z)^2, \qquad w = (I - \rho_{k-1} y_{k-1} s_{k-1}^\top) z
$$

因为 $H_{k-1} \succ 0$ 且 $\rho_{k-1} > 0$，右边两项均非负。若 $z^\top H_k z = 0$，则两项必须同时为零：

1. $\rho_{k-1} (s_{k-1}^\top z)^2 = 0 \Longrightarrow s_{k-1}^\top z = 0$；
2. $w^\top H_{k-1} w = 0 \Longrightarrow w = 0$（因 $H_{k-1}$ 严格正定）。

把条件 1 代回 $w$ 的表达式，得 $w = z - \rho_{k-1} y_{k-1}(s_{k-1}^\top z) = z$；再结合条件 2 的 $w = 0$，推出 $z = 0$，与假设矛盾。因此对任意 $z \neq 0$ 都有 $z^\top H_k z > 0$，即 $H_k \succ 0$。

### Wolfe 条件保证曲率条件

现在说明为什么 $y_{k-1}^\top s_{k-1} > 0$ 可以由 Wolfe 线搜索保证。第 $k-1$ 步的方向取为 $p_{k-1} = -H_{k-1} \nabla f(x_{k-1})$。因为 $H_{k-1} \succ 0$，当 $\nabla f(x_{k-1}) \neq 0$ 时

$$
\nabla f(x_{k-1})^\top p_{k-1} = -\nabla f(x_{k-1})^\top H_{k-1} \nabla f(x_{k-1}) < 0
$$

即 $p_{k-1}$ 确实是下降方向。此时 $x_k = x_{k-1} + \alpha_{k-1} p_{k-1}$，故

$$
s_{k-1} = x_k - x_{k-1} = \alpha_{k-1} p_{k-1}
$$

Wolfe 条件中的曲率准则为 $\phi'(\alpha_{k-1}) \ge c_2 \phi'(0)$，其中 $0 < c_2 < 1$，而 $\phi'(\alpha) = \nabla f(x_{k-1} + \alpha p_{k-1})^\top p_{k-1}$，所以

$$
\nabla f(x_k)^\top p_{k-1} \ge c_2 \nabla f(x_{k-1})^\top p_{k-1}
$$

两边减去 $\nabla f(x_{k-1})^\top p_{k-1}$：

$$
y_{k-1}^\top p_{k-1} \ge (c_2 - 1) \nabla f(x_{k-1})^\top p_{k-1}
$$

再乘以正步长 $\alpha_{k-1} > 0$，并利用 $s_{k-1} = \alpha_{k-1} p_{k-1}$：

$$
y_{k-1}^\top s_{k-1} \ge (c_2 - 1) \alpha_{k-1} \nabla f(x_{k-1})^\top p_{k-1}
$$

由于 $0 < c_2 < 1$ 有 $c_2 - 1 < 0$，而 $\nabla f(x_{k-1})^\top p_{k-1} < 0$、$\alpha_{k-1} > 0$，两个负因子相乘得正，因此

$$
y_{k-1}^\top s_{k-1} > 0
$$

这正好是 $H_k \succ 0$ 所需要的条件，所以 BFGS 一般配合 Wolfe 线搜索使用。强 Wolfe 条件对导数绝对值额外施加了 $\vert \nabla f(x_k)^\top p_{k-1} \vert \le c_2 \vert \nabla f(x_{k-1})^\top p_{k-1} \vert$ 的约束，其曲率准则与标准 Wolfe 一致，同样保证 $y_{k-1}^\top s_{k-1} > 0$。

### Dennis–Moré 准则

#### 内容

设 $f$ 二次连续可微，$x^\star$ 满足 $\nabla f(x^\star) = 0$，且 $\nabla^2 f(x^\star)$ 对称正定（为书写方便，下文记 $G = \nabla^2 f(x^\star)$）。沿用正文的记号，第 $k$ 步的拟牛顿方向为

$$
p_k = - B_k^{-1} \nabla f(x_k) = - H_k \nabla f(x_k),
$$

迭代点为 $x_{k+1} = x_k + \alpha_k p_k$，故位移

$$
s_k = x_{k+1} - x_k = \alpha_k p_k
$$

（这与正文的 $s_{k-1} = x_k - x_{k-1} = \alpha_{k-1} p_{k-1}$ 是同一套下标约定）。在考察极小值点邻域的超线性收敛时，可设线搜索最终接受单位步长 $\alpha_k = 1$，于是

$$
s_k = - B_k^{-1} \nabla f(x_k), \qquad\text{等价地}\qquad B_k s_k = - \nabla f(x_k). \tag{DM-0}
$$

假设迭代序列 $x_k \to x^\star$。**Dennis–Moré 准则**指出：

$$
\lim_{k \to \infty} \frac{\Vert x_{k+1} - x^\star \Vert}{\Vert x_k - x^\star \Vert} = 0
\quad\Longleftrightarrow\quad
\lim_{k \to \infty} \frac{\Vert (B_k - \nabla^2 f(x^\star)) s_k \Vert}{\Vert s_k \Vert} = 0. \tag{DM}
$$

也就是说，拟牛顿法在极小值点邻域**超线性收敛**的充分必要条件是 $B_k$ 沿迭代方向 $s_k$ 渐近地趋近于真实 Hessian $G$。

#### 直观解释

条件 (DM) 只要求矩阵 $B_k$ 在**搜索方向 $s_k$ 上**等于 $G$ 的越来越好的近似，而不要求在整个 $\mathbb{R}^n$ 上接近 $G$。由于 $s_k = - B_k^{-1} \nabla f(x_k)$，若 $B_k s_k \approx G s_k$，则 $p_k = -B_k^{-1}\nabla f(x_k) \approx -G^{-1}\nabla f(x_k)$，即搜索方向渐近地逼近正文中的牛顿方向

$$
p_k^{\text{Newton}} = - \left( \nabla^2 f(x_k) \right)^{-1} \nabla f(x_k).
$$

由于 $x_k \to x^\star$ 且 Hessian 连续，$\nabla^2 f(x_k) \to \nabla^2 f(x^\star) = G$，因此该方向在极限意义下就是 $-G^{-1}\nabla f(x_k)$，这正是正文“搜索方向渐近地逼近牛顿方向”的含义。

#### 证明

记位移误差 $e_k = x_k - x^\star$（以区别于正文中表示函数值误差的 $\Delta_k$）。因为 $\nabla f$ 在 $x^\star$ 处可微、$\nabla f(x^\star) = 0$、$\nabla^2 f(x^\star) = G$，由导数的定义（Fréchet 导数）有

$$
\nabla f(x_k) = G e_k + r_k, \qquad \Vert r_k \Vert = o(\Vert e_k \Vert). \tag{1}
$$

又由拟牛顿步关系式 (DM-0) 可得

$$
G s_k = B_k s_k - (B_k - G) s_k = - \nabla f(x_k) - (B_k - G) s_k. \tag{2}
$$

**充分性。** 设 (DM) 成立，即 $(B_k - G) s_k = o(\Vert s_k \Vert)$。把 (1) 代入 (2)：

$$
G s_k = - G e_k - r_k - (B_k - G) s_k.
$$

两边左乘 $G^{-1}$ 并取范数，利用 $\Vert r_k \Vert = o(\Vert e_k \Vert)$：

$$
\Vert s_k \Vert \le \Vert e_k \Vert + \Vert G^{-1} \Vert \Vert r_k \Vert + \Vert G^{-1} \Vert \, o(\Vert s_k \Vert)
= O(\Vert e_k \Vert) + o(\Vert s_k \Vert),
$$

由此推出 $\Vert s_k \Vert = O(\Vert e_k \Vert)$，从而 $o(\Vert s_k \Vert) = o(\Vert e_k \Vert)$。将其代回上式：

$$
s_k = - e_k + G^{-1} o(\Vert e_k \Vert) = - e_k + o(\Vert e_k \Vert).
$$

于是

$$
e_{k+1} = e_k + s_k = o(\Vert e_k \Vert),
\qquad\text{即}\qquad
\frac{\Vert x_{k+1} - x^\star \Vert}{\Vert x_k - x^\star \Vert} \to 0,
$$

所以迭代超线性收敛。

**必要性。** 设迭代超线性收敛，即 $e_{k+1} = o(\Vert e_k \Vert)$。由 $s_k = e_{k+1} - e_k$ 得

$$
\Vert s_k \Vert \ge \Vert e_k \Vert - \Vert e_{k+1} \Vert = \big( 1 - o(1) \big) \Vert e_k \Vert,
$$

所以 $\Vert e_k \Vert \le \big( 1 + o(1) \big) \Vert s_k \Vert$。另一方面，由 (1) 与 (2)：

$$
(B_k - G) s_k = - \nabla f(x_k) - G s_k = - G e_k - r_k - G s_k = - G (e_k + s_k) - r_k = - G e_{k+1} - r_k.
$$

取范数并利用 $e_{k+1} = o(\Vert e_k \Vert)$、$\Vert r_k \Vert = o(\Vert e_k \Vert)$：

$$
\Vert (B_k - G) s_k \Vert \le \Vert G \Vert \Vert e_{k+1} \Vert + \Vert r_k \Vert
= o(\Vert e_k \Vert) \le o(\Vert s_k \Vert),
$$

这正是 (DM)。$\square$

#### 几点说明

- 该定理把“超线性收敛”这一关于迭代点序列的性质，完全转化为关于矩阵 $B_k$ 的一个可检验条件：只需 $B_k$ 在迭代方向 $s_k$ 上趋近于 $\nabla^2 f(x^\star)$，而无需在整个空间上趋近。BFGS、DFP、SR1 等拟牛顿法的经典超线性收敛定理，正是在适当线搜索下证明其 $\{B_k\}$ 满足 (DM)。
- 对 L-BFGS 而言，由于只保留最近 $m$ 步的曲率信息，$B_k$（或 $H_k$）无法在任意方向上恢复全空间二阶信息，(DM) 一般不成立，因此渐近收敛速度退化为线性收敛，收缩率依赖于保存步数 $m$ 与函数条件数。
- 与拟牛顿法的联系：BFGS 维护的是逆 Hessian 近似 $H_k$，而定理中的 $B_k = H_k^{-1}$；由于 (DM) 用 $B_k$ 表述，实际验证时也可等价地考察 $H_k^{-1}$ 在方向 $s_k$ 上是否收敛到 $\nabla^2 f(x^\star)$。
