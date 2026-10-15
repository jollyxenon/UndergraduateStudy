# -*- coding: utf-8 -*-
"""
为 LinearSearch.md 的「最速下降法 / 精确线搜索下的特点」生成配图。

核心想说明的三件事：
  1. 精确线搜索下 phi'(alpha_k)=0 => g_{k+1}^T p_k=0 => p_{k+1}^T p_k=0，
     即相邻两次搜索方向严格正交，轨迹必然走成 90 度折线（锯齿）。
  2. 条件数 kappa=L/mu 较小时，等高线接近圆，轨迹近似直线，收敛快。
  3. 条件数很大时，等高线是狭长椭圆，轨迹在"山谷"两侧来回横跳，
     沿谷底的推进极其缓慢，收敛停滞。

目标函数取二次型 f(x) = 1/2 x^T A x，A = R diag(lambda_1,lambda_2) R^T，
最小值在原点。梯度 g = A x，精确步长 alpha = (g^T g)/(g^T A g)。
"""
import os
import numpy as np
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt

# ---------------------------------------------------------------- 全局样式
plt.rcParams.update({
    "font.family": ["Microsoft YaHei"],
    "mathtext.fontset": "cm",
    "axes.unicode_minus": False,
    "font.size": 12,
    "axes.titlesize": 13.5,
    "axes.labelsize": 12.5,
    "legend.fontsize": 10.5,
    "savefig.bbox": "tight",
    "savefig.dpi": 220,
})

HERE = os.path.dirname(os.path.abspath(__file__))

COL_PATH = "#d62728"      # 轨迹
COL_PT = "#1f77b4"        # 迭代点
COL_ARROW = "#7d3cf5"     # 方向箭头 / 直角标记
COL_STAR = "#2b2b2b"      # 极小值点
COL_WELL = "#2ca02c"
COL_ILL = "#d62728"


# ---------------------------------------------------------------- 工具函数
def rotation(theta):
    c, s = np.cos(theta), np.sin(theta)
    return np.array([[c, -s], [s, c]])


def make_A(kappa, theta_deg=35.0, lam_min=1.0):
    """条件数 kappa、主轴倾斜 theta 的正定二次型矩阵。"""
    R = rotation(np.deg2rad(theta_deg))
    return R @ np.diag([lam_min, lam_min * kappa]) @ R.T


def steepest_descent(A, x0, n_iter):
    """最速下降 + 精确线搜索（二次型解析步长）。返回轨迹 (n_iter+1, 2)。"""
    x = np.array(x0, dtype=float)
    path = [x.copy()]
    for _ in range(n_iter):
        g = A @ x
        denom = float(g @ A @ g)
        if denom <= 1e-300:
            break
        alpha = float(g @ g) / denom          # 精确线搜索步长
        x = x - alpha * g
        path.append(x.copy())
    return np.array(path)


def quad(A, X, Y):
    return 0.5 * (A[0, 0] * X**2 + 2 * A[0, 1] * X * Y + A[1, 1] * Y**2)


def draw_field(ax, A, levels, xlim, ylim, cmap="Blues"):
    """画等高线（浅填充 + 实线）。"""
    xs = np.linspace(xlim[0], xlim[1], 600)
    ys = np.linspace(ylim[0], ylim[1], 600)
    X, Y = np.meshgrid(xs, ys)
    Z = quad(A, X, Y)
    ax.contourf(X, Y, Z, levels=levels, cmap=cmap, alpha=0.32, zorder=1)
    ax.contour(X, Y, Z, levels=levels, colors="#5b7fa6", linewidths=0.85,
               alpha=0.9, zorder=2)


def draw_path(ax, path, ms=5.0, lw=1.9, label=None, every=1):
    P = path
    ax.plot(P[:, 0], P[:, 1], "-", color=COL_PATH, lw=lw, zorder=6,
            solid_capstyle="round", label=label)
    ax.plot(P[::every, 0], P[::every, 1], "o", color=COL_PT, ms=ms,
            mec="white", mew=0.9, zorder=7)


def right_angle(ax, p_prev, p_mid, p_next, size=0.10, color=COL_ARROW):
    """在 p_mid 处画直角小方块，标记相邻方向正交。"""
    v1 = np.array(p_prev) - np.array(p_mid)
    v2 = np.array(p_next) - np.array(p_mid)
    n1 = v1 / (np.linalg.norm(v1) + 1e-12)
    n2 = v2 / (np.linalg.norm(v2) + 1e-12)
    a = np.array(p_mid) + n1 * size
    b = np.array(p_mid) + n1 * size + n2 * size
    c = np.array(p_mid) + n2 * size
    ax.plot([a[0], b[0], c[0]], [a[1], b[1], c[1]], "-", color=color, lw=1.7,
            zorder=9)


def mark_min(ax):
    ax.plot([0], [0], "*", color=COL_STAR, ms=16, zorder=8)
    ax.annotate(r"$x^\star$", xy=(0, 0), xytext=(0.10, -0.28), fontsize=13,
                color=COL_STAR)


def mark_start(ax, x0):
    ax.plot([x0[0]], [x0[1]], "o", color=COL_ARROW, ms=8, zorder=8)
    ax.annotate(r"$x_0$", xy=x0, xytext=(x0[0] + 0.10, x0[1] + 0.12),
                fontsize=13, color=COL_ARROW)


# ================================================================ 参数
KAPPA_WELL = 2.5
KAPPA_ILL = 20.0
THETA = 35.0
LAM_MIN = 1.0
X0 = (1.60, 1.20)                  # 略微偏离低谷主轴，触发最坏情形锯齿

A_well = make_A(KAPPA_WELL, THETA, LAM_MIN)
A_ill = make_A(KAPPA_ILL, THETA, LAM_MIN)

N_WELL = 5
N_ILL = 16
N_CURVE = 40
path_well = steepest_descent(A_well, X0, N_WELL)
path_ill = steepest_descent(A_ill, X0, N_ILL)
path_ill_long = steepest_descent(A_ill, X0, N_CURVE)

fig, axes = plt.subplots(1, 3, figsize=(15.8, 5.3))
ax_a, ax_b, ax_c = axes

# ------------------------------------------------- (a) 良态：轨迹接近直线
xlim = (-0.60, 1.95)
ylim = (-0.75, 1.55)
draw_field(ax_a, A_well, levels=np.linspace(0.06, 2.4, 13), xlim=xlim, ylim=ylim)
draw_path(ax_a, path_well, label="精确线搜索轨迹")
mark_min(ax_a)
mark_start(ax_a, X0)

ax_a.set_title(r"(a) 良态：$\kappa=2.5$" "\n" "等高线近圆，轨迹近似直达")
ax_a.set_xlabel(r"$x_1$")
ax_a.set_ylabel(r"$x_2$")
ax_a.set_xlim(*xlim)
ax_a.set_ylim(*ylim)
ax_a.set_aspect("equal", adjustable="box")
ax_a.legend(loc="lower right", framealpha=0.95)

# ------------------------------------------------- (b) 病态：锯齿 / 直角折线
draw_field(ax_b, A_ill, levels=np.linspace(0.06, 2.4, 18), xlim=xlim, ylim=ylim)
draw_path(ax_b, path_ill, label=r"精确线搜索轨迹（相邻方向正交）")

# 选相邻两段都较长的顶点，直角看得最清楚
P = path_ill
seg = np.linalg.norm(np.diff(P, axis=0), axis=1)
k0 = 1 + int(np.argmax(np.minimum(seg[:-1], seg[1:])[:8]))
right_angle(ax_b, P[k0 - 1], P[k0], P[k0 + 1], size=0.14, color=COL_ARROW)
ax_b.annotate(r"$p_{k+1}^\top p_k = 0$" "\n" r"相邻方向正交（$90^\circ$ 折线）",
              xy=(P[k0][0], P[k0][1]),
              xytext=(0.02, 1.08),
              arrowprops=dict(arrowstyle="->", color=COL_ARROW, lw=1.2),
              color=COL_ARROW, fontsize=11.5, ha="left")

mark_min(ax_b)
mark_start(ax_b, X0)

ax_b.set_title(r"(b) 病态：$\kappa=20$" "\n" "狭长山谷间来回横跳，沿谷底推进极慢")
ax_b.set_xlabel(r"$x_1$")
ax_b.set_ylabel(r"$x_2$")
ax_b.set_xlim(*xlim)
ax_b.set_ylim(*ylim)
ax_b.set_aspect("equal", adjustable="box")
ax_b.legend(loc="lower right", framealpha=0.95)

# ------------------------------------------------- (c) 误差收敛对比
path_well_long = steepest_descent(A_well, X0, N_CURVE)
k_well = np.arange(len(path_well_long))
err_well = quad(A_well, path_well_long[:, 0], path_well_long[:, 1])
k_ill = np.arange(len(path_ill_long))
err_ill = quad(A_ill, path_ill_long[:, 0], path_ill_long[:, 1])

ax_c.semilogy(k_well, np.maximum(err_well, 1e-16), "-o", color=COL_WELL,
              ms=5.0, lw=1.9, label=r"$\kappa=2.5$（快）")
ax_c.semilogy(k_ill, np.maximum(err_ill, 1e-16), "-s", color=COL_ILL,
              ms=3.8, lw=1.9, label=r"$\kappa=20$（慢，近似线性收敛）")

ax_c.set_title("(c) 目标函数误差收敛曲线" "\n" "条件数越大，直线越平，收敛越慢")
ax_c.set_xlabel(r"迭代次数 $k$")
ax_c.set_ylabel(r"$f(x_k)-f(x^\star)$")
ax_c.grid(alpha=0.28, ls=":", which="both")
ax_c.legend(loc="upper right", framealpha=0.95)
ax_c.set_xlim(-0.5, N_CURVE)
ax_c.set_ylim(1e-10, 1e1)

fig.suptitle("最速下降法 + 精确线搜索：相邻方向正交导致的锯齿轨迹问题",
             fontsize=15.5, y=1.02)
fig.tight_layout()

out = os.path.join(HERE, "steepest_descent_trajectory")
fig.savefig(out + ".png")
fig.savefig(out + ".svg")
plt.close(fig)

print("saved ->", out + ".png / .svg")
print("right-angle vertex k0 =", k0)
