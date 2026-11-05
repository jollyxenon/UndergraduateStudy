"""用本次单摆实验原始数据计算统计量、粗差筛查、线性回归、重力加速度及其不确定度，生成 results.json 与 results.tex。"""
import json
import math
import statistics
from pathlib import Path

root = Path(__file__).resolve().parent
data = json.loads((root / "data.json").read_text(encoding="utf-8"))

periods = data["periods_per_measurement"]  # 2.5
prob = data["probability"]                 # 0.95
c_n5 = data["chauvenet_factor_n5"]         # 1.65
t_by_n = data["course_t_factors_by_n"]
t_by_df = data["course_t_factors_by_df"]

err_timer = data["instruments"]["timer_error_s"]
err_ruler = data["instruments"]["ruler_error_cm"]
err_caliper = data["instruments"]["caliper_error_cm"]
g_ref = data["reference_g_m_s2"]

delta_B_t = prob * err_timer               # 0.00095 s
delta_B_L1 = prob * err_ruler              # 0.0475 cm
delta_B_L2 = prob * err_caliper            # 0.0019 cm
U_L_cm = math.hypot(delta_B_L1, 0.5 * delta_B_L2)
U_L_m = U_L_cm / 100.0

tex = []


def command(name, value):
    """将计算结果写为 LaTeX 命令。"""
    tex.append("\\newcommand{\\" + name + "}{" + str(value) + "}")


def table_rows(name, rows):
    """将表格行写为 LaTeX 命令。"""
    command(name, "\n" + "\n".join(" & ".join(row) + r" \\" for row in rows))


def process_group(raw_times):
    """对单组5次测量值做一次肖维涅判别（c=1.65），剔除越界值后重算均值、标准差与P=0.95不确定度。"""
    raw_mean = statistics.mean(raw_times)
    raw_s = statistics.stdev(raw_times)
    half_width = c_n5 * raw_s
    lower = raw_mean - half_width
    upper = raw_mean + half_width
    excluded = [i + 1 for i, v in enumerate(raw_times) if v < lower or v > upper]
    kept = [v for i, v in enumerate(raw_times, start=1) if i not in excluded]
    n = len(kept)
    mean_t = statistics.mean(kept)
    s_t = statistics.stdev(kept)
    sem_t = s_t / math.sqrt(n)
    t_factor = t_by_n[str(n)]
    delta_A_t = t_factor * sem_t
    U_t = math.hypot(delta_A_t, delta_B_t)

    mean_2T = mean_t * (2.0 / periods)
    U_2T = U_t * (2.0 / periods)
    mean_T = mean_t / periods
    U_T = U_t / periods
    T2 = mean_T ** 2
    U_T2 = 2.0 * mean_T * U_T

    return {
        "raw_times": raw_times,
        "raw_mean": raw_mean,
        "raw_s": raw_s,
        "screening_lower": lower,
        "screening_upper": upper,
        "excluded_indices": excluded,
        "kept_times": kept,
        "n": n,
        "mean_t": mean_t,
        "s_t": s_t,
        "sem_t": sem_t,
        "t_factor": t_factor,
        "delta_A_t": delta_A_t,
        "delta_B_t": delta_B_t,
        "U_t": U_t,
        "mean_2T": mean_2T,
        "U_2T": U_2T,
        "mean_T": mean_T,
        "U_T": U_T,
        "T2": T2,
        "U_T2": U_T2,
    }


def linear_regression(xs, ys):
    """普通最小二乘线性回归 y = k x + b。"""
    m = len(xs)
    df = m - 2
    mx = statistics.mean(xs)
    my = statistics.mean(ys)
    sxx = sum((x - mx) ** 2 for x in xs)
    sxy = sum((x - mx) * (y - my) for x, y in zip(xs, ys))
    syy = sum((y - my) ** 2 for y in ys)
    k = sxy / sxx
    b = my - k * mx
    residuals = [y - (k * x + b) for x, y in zip(xs, ys)]
    sse = sum(r ** 2 for r in residuals)
    ssr = syy - sse
    se = math.sqrt(sse / df)
    sk = se / math.sqrt(sxx)
    sb = se * math.sqrt(1.0 / m + (mx ** 2) / sxx)
    r2 = (sxy ** 2) / (sxx * syy)
    r = math.copysign(math.sqrt(r2), sxy)
    t_df = t_by_df[str(df)]
    return {
        "m": m,
        "df": df,
        "mean_x": mx,
        "mean_y": my,
        "sxx": sxx,
        "sxy": sxy,
        "syy": syy,
        "k": k,
        "b": b,
        "sk": sk,
        "sb": sb,
        "se": se,
        "sse": sse,
        "ssr": ssr,
        "r2": r2,
        "r": r,
        "t_factor_df": t_df,
        "delta_A_k": t_df * sk,
        "delta_A_b": t_df * sb,
        "residuals": residuals,
    }


# ==================== 第一部分：固定摆长，改变摆角 ====================
p1_cfg = data["part1_fixed_length"]
L1_p1_cm = p1_cfg["L1_cm"]
L2_p1_cm = p1_cfg["L2_cm"]
L_p1_rec_cm = p1_cfg["L_recorded_cm"]
L_p1_exact_cm = p1_cfg["L_exact_cm"]
L_p1_m = L_p1_rec_cm / 100.0

p1_groups = []
p1_raw_rows = []
p1_stat_rows = []
fit1_xs, fit1_ys, fit1_Uys = [], [], []

for item in p1_cfg["series"]:
    theta = item["theta_deg"]
    theta_str = f"{theta:.1f}"
    stats = process_group(item["time_2p5T_s"])
    sin2 = math.sin(math.radians(theta / 2.0)) ** 2
    x_disp_cm = L1_p1_cm * math.sin(math.radians(theta))
    in_fit = theta_str not in p1_cfg["fit_excluded_angles_deg"]
    stats.update({
        "theta_deg": theta,
        "sin2_half_theta": sin2,
        "horizontal_x_cm": x_disp_cm,
        "included_in_fit": in_fit,
    })
    p1_groups.append(stats)

    raw_cells = [
        f"{v:.3f}" + (r"$^{*}$" if (i + 1) in stats["excluded_indices"] else "")
        for i, v in enumerate(stats["raw_times"])
    ]
    note = (
        f"剔除第{stats['excluded_indices'][0]}次"
        if stats["excluded_indices"]
        else "全部保留"
    )
    p1_raw_rows.append([
        theta_str,
        f"{sin2:.5f}",
        *raw_cells,
        f"[{stats['screening_lower']:.4f}, {stats['screening_upper']:.4f}]",
        note,
    ])
    p1_stat_rows.append([
        theta_str,
        str(stats["n"]),
        f"{stats['mean_t']:.5f}",
        f"{stats['s_t']:.5f}",
        f"{stats['sem_t']:.5f}",
        f"{stats['t_factor']:.2f}",
        f"{stats['delta_A_t']:.5f}",
        f"{stats['U_t']:.5f}",
        f"{stats['mean_T']:.5f}",
        f"{stats['U_T']:.5f}",
    ])
    if in_fit:
        fit1_xs.append(sin2)
        fit1_ys.append(stats["mean_t"])
        fit1_Uys.append(stats["U_t"])

reg1 = linear_regression(fit1_xs, fit1_ys)
b1 = reg1["b"]  # 2.5 T0
k1 = reg1["k"]
U_b1 = math.hypot(reg1["delta_A_b"], delta_B_t)
U_k1 = reg1["delta_A_k"]
T0_p1 = b1 / periods
s_T0_p1 = reg1["sb"] / periods
U_T0_p1 = U_b1 / periods
two_T0_p1 = 2.0 * T0_p1
U_two_T0_p1 = 2.0 * U_T0_p1

g1 = 4.0 * (math.pi ** 2) * L_p1_m / (T0_p1 ** 2)
g1_exact_L = 4.0 * (math.pi ** 2) * (L_p1_exact_cm / 100.0) / (T0_p1 ** 2)
rel_U_L_p1 = U_L_m / L_p1_m
rel_U_T0_p1 = U_T0_p1 / T0_p1
rel_U_g1 = math.hypot(rel_U_L_p1, 2.0 * rel_U_T0_p1)
U_g1 = g1 * rel_U_g1
u_g1_from_reg_sd = g1 * math.hypot(rel_U_L_p1, 2.0 * (s_T0_p1 / T0_p1))
delta_g1 = g1 - g_ref
rel_delta_g1 = delta_g1 / g_ref
k1_theoretical = b1 / 4.0

# ==================== 第二部分：改变摆长 ====================
p2_cfg = data["part2_varied_length"]
p2_groups = []
p2_raw_rows = []
p2_stat_rows = []
fit2_xs, fit2_ys, fit2_Uys = [], [], []

for item in p2_cfg["series"]:
    L1_cm = item["L1_cm"]
    L_cm = item["L_recorded_cm"]
    L_m = L_cm / 100.0
    stats = process_group(item["time_2p5T_s"])
    stats.update({
        "L1_cm": L1_cm,
        "L_cm": L_cm,
        "L_m": L_m,
    })
    p2_groups.append(stats)

    raw_cells = [
        f"{v:.3f}" + (r"$^{*}$" if (i + 1) in stats["excluded_indices"] else "")
        for i, v in enumerate(stats["raw_times"])
    ]
    note = (
        f"剔除第{stats['excluded_indices'][0]}次"
        if stats["excluded_indices"]
        else "全部保留"
    )
    p2_raw_rows.append([
        f"{L1_cm:.3f}",
        f"{L_cm:.3f}",
        *raw_cells,
        f"[{stats['screening_lower']:.4f}, {stats['screening_upper']:.4f}]",
        note,
    ])
    p2_stat_rows.append([
        f"{L_cm:.3f}",
        str(stats["n"]),
        f"{stats['mean_t']:.5f}",
        f"{stats['s_t']:.5f}",
        f"{stats['sem_t']:.5f}",
        f"{stats['t_factor']:.2f}",
        f"{stats['delta_A_t']:.5f}",
        f"{stats['U_t']:.5f}",
        f"{stats['mean_T']:.5f}",
        f"{stats['T2']:.5f}",
        f"{stats['U_T2']:.5f}",
    ])
    fit2_xs.append(L_m)
    fit2_ys.append(stats["T2"])
    fit2_Uys.append(stats["U_T2"])

reg2 = linear_regression(fit2_xs, fit2_ys)
k2 = reg2["k"]
a2 = reg2["b"]
U_k2 = reg2["delta_A_k"]
U_a2 = reg2["delta_A_b"]
g2 = 4.0 * (math.pi ** 2) / k2
rel_U_g2 = U_k2 / k2
U_g2 = g2 * rel_U_g2
u_g2_from_reg_sd = g2 * (reg2["sk"] / k2)
delta_g2 = g2 - g_ref
rel_delta_g2 = delta_g2 / g_ref

# ==================== 汇总写出 JSON 与 LaTeX 宏 ====================
table_rows("PartOneRawRows", p1_raw_rows)
table_rows("PartOneStatRows", p1_stat_rows)
table_rows("PartTwoRawRows", p2_raw_rows)
table_rows("PartTwoStatRows", p2_stat_rows)

command("PartOneSlope", f"{k1:.4f}")
command("PartOneSlopeSD", f"{reg1['sk']:.4f}")
command("PartOneSlopeU", f"{U_k1:.4f}")
command("PartOneIntercept", f"{b1:.5f}")
command("PartOneInterceptSD", f"{reg1['sb']:.5f}")
command("PartOneInterceptU", f"{U_b1:.5f}")
command("PartOneRTwo", f"{reg1['r2']:.4f}")
command("PartOneR", f"{reg1['r']:.4f}")
command("PartOneTZero", f"{T0_p1:.5f}")
command("PartOneTZeroU", f"{U_T0_p1:.5f}")
command("PartOneTZeroFinal", f"({T0_p1:.4f}\\pm {U_T0_p1:.4f})")
command("PartOneTwoTZeroFinal", f"({two_T0_p1:.4f}\\pm {U_two_T0_p1:.4f})")
command("PartOneG", f"{g1:.4f}")
command("PartOneGU", f"{U_g1:.4f}")
command("PartOneGFinal", f"({g1:.2f}\\pm {U_g1:.2f})")
command("PartOneGRelU", f"{100.0 * rel_U_g1:.2f}\\%")
command("PartOneGDiff", f"+{delta_g1:.3f}")
command("PartOneGRelDiff", f"+{100.0 * rel_delta_g1:.2f}\\%")

command("PartTwoSlope", f"{k2:.4f}")
command("PartTwoSlopeSD", f"{reg2['sk']:.4f}")
command("PartTwoSlopeU", f"{U_k2:.4f}")
command("PartTwoIntercept", f"{a2:.4f}")
command("PartTwoInterceptSD", f"{reg2['sb']:.4f}")
command("PartTwoInterceptU", f"{U_a2:.4f}")
command("PartTwoRTwo", f"{reg2['r2']:.4f}")
command("PartTwoR", f"{reg2['r']:.4f}")
command("PartTwoG", f"{g2:.4f}")
command("PartTwoGU", f"{U_g2:.4f}")
command("PartTwoGSD", f"{u_g2_from_reg_sd:.4f}")
command("PartTwoGFinal", f"({g2:.1f}\\pm {U_g2:.1f})")
command("PartTwoGFinalTwoDigits", f"({g2:.2f}\\pm {U_g2:.2f})")
command("PartTwoGRelU", f"{100.0 * rel_U_g2:.2f}\\%")
command("PartTwoGDiff", f"+{delta_g2:.3f}")
command("PartTwoGRelDiff", f"+{100.0 * rel_delta_g2:.2f}\\%")

# 写出 PGFPlots 坐标宏
p1_coords = " ".join(f"({x:.6f},{y:.5f}) +- (0,{u:.5f})" for x, y, u in zip(fit1_xs, fit1_ys, fit1_Uys))
p2_coords = " ".join(f"({x:.4f},{y:.5f}) +- (0,{u:.5f})" for x, y, u in zip(fit2_xs, fit2_ys, fit2_Uys))
command("PartOnePlotCoords", p1_coords)
command("PartTwoPlotCoords", p2_coords)

result = {
    "parameters": {
        "periods_per_measurement": periods,
        "probability": prob,
        "delta_B_t_s": delta_B_t,
        "delta_B_L1_cm": delta_B_L1,
        "delta_B_L2_cm": delta_B_L2,
        "U_L_cm": U_L_cm,
        "reference_g_m_s2": g_ref,
    },
    "part1": {
        "L1_cm": L1_p1_cm,
        "L2_cm": L2_p1_cm,
        "L_recorded_cm": L_p1_rec_cm,
        "L_exact_cm": L_p1_exact_cm,
        "U_L_cm": U_L_cm,
        "groups": p1_groups,
        "regression_2p5T_vs_sin2": reg1,
        "U_b1_s": U_b1,
        "U_k1_s": U_k1,
        "k1_theoretical_s": k1_theoretical,
        "T0_s": T0_p1,
        "s_T0_s": s_T0_p1,
        "U_T0_s": U_T0_p1,
        "two_T0_s": two_T0_p1,
        "U_two_T0_s": U_two_T0_p1,
        "g_m_s2": g1,
        "g_exact_L_m_s2": g1_exact_L,
        "U_g_m_s2": U_g1,
        "u_g_from_reg_sd_m_s2": u_g1_from_reg_sd,
        "rel_U_g": rel_U_g1,
        "variance_share_L": (rel_U_L_p1 / rel_U_g1) ** 2,
        "variance_share_T0": (2.0 * rel_U_T0_p1 / rel_U_g1) ** 2,
        "delta_g_m_s2": delta_g1,
        "rel_delta_g": rel_delta_g1,
    },
    "part2": {
        "groups": p2_groups,
        "regression_T2_vs_L": reg2,
        "U_k2_s2_m": U_k2,
        "U_a2_s2": U_a2,
        "g_m_s2": g2,
        "U_g_m_s2": U_g2,
        "u_g_from_reg_sd_m_s2": u_g2_from_reg_sd,
        "rel_U_g": rel_U_g2,
        "delta_g_m_s2": delta_g2,
        "rel_delta_g": rel_delta_g2,
    },
}

(root / "results.json").write_text(json.dumps(result, ensure_ascii=False, indent=2) + "\n", encoding="utf-8")
(root / "results.tex").write_text(
    "% calculate.py 自动生成；请修改 data.json 后重新运行。\n" + "\n".join(tex) + "\n",
    encoding="utf-8",
)
print(json.dumps(result, ensure_ascii=False, indent=2))
