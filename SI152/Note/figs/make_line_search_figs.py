# -*- coding: utf-8 -*-
"""
为 LinearSearch.md 生成配图：
  1. Armijo 线搜索示意图
  2. Wolfe  线搜索示意图

约定：phi(alpha) = f(x_k + alpha p_k)，当前迭代点位于 alpha = 0。
两个图都在 alpha = 0 处画切线（斜率 phi'(0)），
并画一条过同一点、斜率只有 phi'(0)/5 的 Armijo 线，
然后把函数曲线上落在可接受区间内的部分标红。
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
    "font.size": 13,
    "axes.titlesize": 15,
    "axes.labelsize": 14,
    "legend.fontsize": 11.5,
    "savefig.bbox": "tight",
    "savefig.dpi": 220,
})

HERE = os.path.dirname(os.path.abspath(__file__))

# ---------------------------------------------------------------- 目标函数
# phi(alpha) = (alpha - 1)^2  ->  phi(0) = 1, phi'(0) = -2
def phi(a):
    return (a - 1.0) ** 2


def dphi(a):
    return 2.0 * (a - 1.0)


PHI0 = phi(0.0)          # 1.0
D0 = dphi(0.0)           # -2.0
C1 = 1.0 / 5.0           # Armijo 常数 = 1/5  (题目要求)

COL_CURVE = "#2b2b2b"
COL_TAN = "#8a8a8a"
COL_ARM = "#1f77b4"
COL_RED = "#d62728"
COL_ORANGE = "#ff9f1c"
COL_CURV = "#7d3cf5"

alpha = np.linspace(0.0, 2.0, 800)
y = phi(alpha)


def armijo_line(a):
    return PHI0 + C1 * D0 * a


def tangent0(a):
    return PHI0 + D0 * a


# ================================================================ Armijo 图
def make_armijo():
    # 第二条线与曲线的交点:(a-1)^2 - (1 + c*d0*a) = a^2 + (-2 - c*d0) a = 0
    # 非零根 a = -( -2 - C1*D0 ) = 1.6
    a_star = -(-2.0 - C1 * D0)

    fig, ax = plt.subplots(figsize=(8.4, 5.4))

    # 曲线
    ax.plot(alpha, y, color=COL_CURVE, lw=2.4, zorder=3,
            label=r"$\phi(\alpha_k)=f(x_k+\alpha_k p_k)$")

    # 切线 (斜率 phi'(0))
    ax.plot(alpha, tangent0(alpha), color=COL_TAN, ls="--", lw=1.8, zorder=2,
            label=r"切线, 斜率 $\phi'(0)$")

    # 第二条线 (斜率 c*phi'(0), 数值上 c = 1/5)
    ax.plot(alpha, armijo_line(alpha), color=COL_ARM, lw=2.2, zorder=2,
            label=r"Armijo 线, 斜率 $c\,\phi'(0)$")

    # 可接受区域浅红填充
    ax.fill_between(alpha, y, armijo_line(alpha), where=(alpha <= a_star),
                    color=COL_RED, alpha=0.09, zorder=1)

    # 曲线上的可接受部分标红
    m = alpha <= a_star
    ax.plot(alpha[m], y[m], color=COL_RED, lw=3.6, solid_capstyle="round",
            zorder=4, label="满足 Armijo 的步长区间")

    # 交点与当前点
    ax.plot([0.0], [PHI0], "o", color="black", ms=6.5, zorder=6)
    ax.annotate(r"当前点 $\alpha_k=0$", xy=(0.0, PHI0), xytext=(0.12, 1.34),
                arrowprops=dict(arrowstyle="->", color="black", lw=1.1),
                fontsize=12)

    ax.plot([a_star], [phi(a_star)], "o", color=COL_RED, ms=6.5, zorder=6)
    ax.vlines(a_star, 0, phi(a_star), color=COL_RED, ls=":", lw=1.4, zorder=2)
    ax.annotate(r"$\phi(\alpha_k)=$ Armijo 线" "\n" r"（临界点）",
                xy=(a_star, phi(a_star)), xytext=(1.62, 0.86),
                arrowprops=dict(arrowstyle="->", color=COL_RED, lw=1.2),
                color=COL_RED, fontsize=11.5, ha="left")

    ax.annotate("两条线都过该点", xy=(0.02, PHI0), xytext=(0.55, 1.28),
                fontsize=11.5, color=COL_ARM,
                arrowprops=dict(arrowstyle="->", color=COL_ARM, lw=1.1))

    ax.set_xlim(0, 2.0)
    ax.set_ylim(-0.55, 1.6)
    ax.set_xlabel(r"步长 $\alpha_k$")
    ax.set_ylabel(r"$\phi(\alpha_k)$")
    ax.set_title("Armijo 线搜索：充足下降条件")
    ax.axhline(0, color="black", lw=0.8, alpha=0.35)
    ax.grid(alpha=0.25, ls=":")
    ax.legend(loc="lower right", framealpha=0.95)

    out = os.path.join(HERE, "armijo_line_search")
    fig.savefig(out + ".png")
    fig.savefig(out + ".svg")
    plt.close(fig)
    return a_star


# ================================================================ Wolfe 图
def make_wolfe():
    C2 = 0.7
    # Armijo 区间的右端: a = 1.6
    a_arm = -(-2.0 - C1 * D0)                       # 1.6
    # 曲率条件 phi'(a) >= C2*phi'(0) 的左端:
    # 2a - 2 = C2*(-2)  ->  a = 1 - C2 = 0.3
    a_curv = 1.0 - C2

    fig, ax = plt.subplots(figsize=(8.4, 5.4))

    # 曲线
    ax.plot(alpha, y, color=COL_CURVE, lw=2.4, zorder=3,
            label=r"$\phi(\alpha_k)=f(x_k+\alpha_k p_k)$")

    # 切线 (斜率 phi'(0))
    ax.plot(alpha, tangent0(alpha), color=COL_TAN, ls="--", lw=1.8, zorder=2,
            label=r"切线, 斜率 $\phi'(0)$")

    # Armijo 线 (斜率 c1*phi'(0), 数值上 c1 = 1/5)
    ax.plot(alpha, armijo_line(alpha), color=COL_ARM, lw=2.2, zorder=2,
            label=r"Armijo 线, 斜率 $c_1\phi'(0)$")

    # 曲率条件的临界切线：过 a_curv，斜率 C2*phi'(0)
    # 只画点附近的一小段, 避免铺满整幅图
    seg = (alpha >= 0.10) & (alpha <= 0.90)
    tc = phi(a_curv) + C2 * D0 * (alpha - a_curv)
    ax.plot(alpha[seg], tc[seg], color=COL_CURV, ls="-.", lw=1.9, zorder=2,
            label=r"临界切线, 斜率 $c_2\phi'(0)$")

    # --- 曲线上的区间着色 ---
    # 满足 Wolfe（两条件同时成立）: [a_curv, a_arm]  -> 红色
    m_w = (alpha >= a_curv) & (alpha <= a_arm)
    # 只满足 Armijo、步长过小（曲率条件不满足）: [0, a_curv] -> 橙色
    m_o = alpha <= a_curv
    ax.plot(alpha[m_o], y[m_o], color=COL_ORANGE, lw=3.6,
            solid_capstyle="round", zorder=4,
            label="步长过小，不满足曲率条件")
    ax.plot(alpha[m_w], y[m_w], color=COL_RED, lw=3.8,
            solid_capstyle="round", zorder=5, label="满足 Wolfe 的步长区间")

    # 浅红填充 (Armijo 可行域)
    ax.fill_between(alpha, y, armijo_line(alpha),
                    where=(alpha >= a_curv) & (alpha <= a_arm),
                    color=COL_RED, alpha=0.10, zorder=1)

    # 关键点
    ax.plot([0.0], [PHI0], "o", color="black", ms=6.5, zorder=7)
    ax.annotate(r"当前点 $\alpha_k=0$", xy=(0.0, PHI0), xytext=(0.06, 1.4),
                arrowprops=dict(arrowstyle="->", color="black", lw=1.1),
                fontsize=12)

    ax.plot([a_curv], [phi(a_curv)], "o", color=COL_CURV, ms=6.5, zorder=7)
    ax.vlines(a_curv, 0, phi(a_curv), color=COL_CURV, ls=":", lw=1.4, zorder=2)
    ax.annotate(r"曲率条件 $\phi'(\alpha_k)\geq c_2\phi'(0)$" "\n" "恰好开始满足",
                xy=(a_curv, phi(a_curv)), xytext=(0.60, 1.06),
                arrowprops=dict(arrowstyle="->", color=COL_CURV, lw=1.2),
                color=COL_CURV, fontsize=11.5, ha="left")

    ax.plot([a_arm], [phi(a_arm)], "o", color=COL_RED, ms=6.5, zorder=7)
    ax.annotate(r"$\phi(\alpha_k)=$ Armijo 线", xy=(a_arm, phi(a_arm)),
                xytext=(1.42, 0.95),
                arrowprops=dict(arrowstyle="->", color=COL_RED, lw=1.2),
                color=COL_RED, fontsize=11.5, ha="left")

    ax.set_xlim(0, 2.0)
    ax.set_ylim(-0.55, 1.62)
    ax.set_xlabel(r"步长 $\alpha_k$")
    ax.set_ylabel(r"$\phi(\alpha_k)$")
    ax.set_title(r"Wolfe 线搜索：$0<c_1<c_2<1$")
    ax.axhline(0, color="black", lw=0.8, alpha=0.35)
    ax.grid(alpha=0.25, ls=":")
    ax.legend(loc="lower right", framealpha=0.95)

    out = os.path.join(HERE, "wolfe_line_search")
    fig.savefig(out + ".png")
    fig.savefig(out + ".svg")
    plt.close(fig)
    return a_arm, a_curv


if __name__ == "__main__":
    a = make_armijo()
    print("Armijo 临界点 alpha =", a)
    print("Wolfe (armijo右端, 曲率左端) =", make_wolfe())
    print("done ->", HERE)
