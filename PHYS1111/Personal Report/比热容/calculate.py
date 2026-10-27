"""用本次原始数据计算课程口径的比热容与计时不确定度，生成报告表格。"""
import json
import math
import statistics
from pathlib import Path

root = Path(__file__).resolve().parent
data = json.loads((root / "data.json").read_text(encoding="utf-8"))
samples = data["samples"]
result = {"policy": data["parameter_policy"], "samples": {}}
tex = []


def command(name, value):
    """将同源计算结果写为 LaTeX 命令，供正文直接引用。"""
    tex.append("\\newcommand{\\" + name + "}{" + str(value) + "}")


def table_rows(name, rows):
    """写出可编辑表格的行内容。"""
    command(name, "\n" + "\n".join(" & ".join(row) + r" \\" for row in rows))


# 先用每组原始六次读数作一次肖维涅判别；不循环删点。
raw_rows, screening_rows, time_rows = [], [], []
delta_B = data["probability"] * data["instrument_time_error_s"]
for symbol, sample in samples.items():
    raw = sample["time_s"]
    raw_mean, raw_s = statistics.mean(raw), statistics.stdev(raw)
    half_width = data["chauvenet_factor_n6"] * raw_s
    excluded = [i + 1 for i, value in enumerate(raw) if abs(value - raw_mean) > half_width]
    kept = [value for i, value in enumerate(raw, start=1) if i not in excluded]
    n = len(kept)
    mean, s = statistics.mean(kept), statistics.stdev(kept)
    sem = s / math.sqrt(n)
    t_factor = data["course_t_factors"][str(n)]
    delta_A = t_factor * sem
    U = math.hypot(delta_A, delta_B)
    delta_T = data["temperature_start_C"] - data["temperature_end_C"]
    rate = delta_T / mean
    rate_U = delta_T * U / mean**2
    values = dict(raw_mean=raw_mean, raw_s=raw_s,
                  screening_lower=raw_mean-half_width, screening_upper=raw_mean+half_width,
                  excluded_indices=excluded, kept_times=kept, n=n, mean=mean, s=s, sem=sem,
                  t_factor=t_factor, delta_A=delta_A, delta_B=delta_B, U=U,
                  rate=rate, rate_U=rate_U, mass_g=sample["mass_g"])
    result["samples"][symbol] = values
    raw_rows.append([symbol] + [f"{v:.2f}" + (r"$^{*}$" if i+1 in excluded else "") for i, v in enumerate(raw)])
    screening_rows.append([symbol, f"{raw_mean:.5f}", f"{raw_s:.5f}",
                           f"[{raw_mean-half_width:.5f}, {raw_mean+half_width:.5f}]",
                           "剔除第2次" if excluded else "全部保留"])
    time_rows.append([symbol, str(n), f"{mean:.5f}", f"{s:.5f}", f"{sem:.5f}",
                      f"{t_factor:.2f}", f"{delta_A:.5f}", f"{U:.5f}"])
    command(symbol + "Mean", f"{mean:.5f}")
    command(symbol + "U", f"{U:.5f}")
    command(symbol + "TimeFinal", f"({mean:.2f}\\pm {U:.2f})")
    command(symbol + "RateFinal", f"({rate:.4f}\\pm {rate_U:.4f})")

# 在共同温区内由时间比求比热容。质量以 g 同单位代入，最终输出 SI 单位。
cu = result["samples"]["Cu"]
c_ref = data["copper_specific_heat_cal_gK"] * data["joule_per_cal"] * 1000
command("CuReference", f"{c_ref:.3f}")
derived_rows, budget_rows = [], []
for symbol in ("Fe", "Al"):
    v = result["samples"][symbol]
    ratio = v["mean"] / cu["mean"]
    ratio_U = ratio * math.hypot(v["U"] / v["mean"], cu["U"] / cu["mean"])
    c = c_ref * cu["mass_g"] / v["mass_g"] * ratio
    sensitivity_x = c / v["mean"]
    sensitivity_cu = -c / cu["mean"]
    contribution_x = abs(sensitivity_x) * v["U"]
    contribution_cu = abs(sensitivity_cu) * cu["U"]
    U = math.hypot(contribution_x, contribution_cu)
    capacity = c * v["mass_g"] / 1000
    capacity_U = U * v["mass_g"] / 1000
    result[symbol] = dict(ratio=ratio, ratio_U=ratio_U, c=c, c_cal_gK=c/4184,
                          U=U, relative_U=U/c, capacity=capacity, capacity_U=capacity_U,
                          sensitivity_x=sensitivity_x, sensitivity_cu=sensitivity_cu,
                          contribution_x=contribution_x, contribution_cu=contribution_cu,
                          variance_share_x=contribution_x**2/U**2,
                          variance_share_cu=contribution_cu**2/U**2)
    command(symbol + "C", f"{c:.4f}")
    command(symbol + "CU", f"{U:.4f}")
    command(symbol + "CFinal", f"({c:.0f}\\pm {U:.0f})")
    command(symbol + "CRel", f"{100*U/c:.2f}\\%")
    command(symbol + "Capacity", f"({capacity:.2f}\\pm {capacity_U:.2f})")
    command(symbol + "RatioFinal", f"({ratio:.3f}\\pm {ratio_U:.3f})")
    derived_rows.append([symbol, "$\\" + symbol + "RateFinal$", "$\\" + symbol + "RatioFinal$",
                         f"{c/4184:.5f}", "$\\" + symbol + "CFinal$"])
    for input_name, input_value, input_U, sensitivity, contribution in (
        (symbol, v["mean"], v["U"], sensitivity_x, contribution_x),
        ("Cu", cu["mean"], cu["U"], sensitivity_cu, contribution_cu),
    ):
        budget_rows.append([symbol, "$\\bar t_{\\mathrm{"+input_name+"}}$",
                            f"{input_value:.5f}", f"{input_U:.5f}", f"{sensitivity:.3f}",
                            f"{contribution:.3f}", f"{100*contribution**2/U**2:.2f}\\%"])

# 铜时间是两个结果共用的测量量，保存其引入的相关性。
fe, al = result["Fe"], result["Al"]
covariance = fe["sensitivity_cu"] * al["sensitivity_cu"] * cu["U"]**2
result["shared_copper_course_covariance"] = covariance
result["shared_copper_course_correlation"] = covariance / (fe["U"] * al["U"])
result["Fe_all_six_c"] = c_ref * cu["mass_g"] / samples["Fe"]["mass_g"] * result["samples"]["Fe"]["raw_mean"] / cu["mean"]
command("FeAllSixC", f"{result['Fe_all_six_c']:.2f}")
command("CuCapacity", f"{c_ref * cu['mass_g'] / 1000:.3f}")
table_rows("RawRows", raw_rows)
table_rows("ScreeningRows", screening_rows)
table_rows("TimeRows", time_rows)
table_rows("DerivedRows", derived_rows)
table_rows("BudgetRows", budget_rows)
(root / "results.json").write_text(json.dumps(result, ensure_ascii=False, indent=2) + "\n", encoding="utf-8")
(root / "results.tex").write_text("% calculate.py 自动生成；请修改 data.json 后重新运行。\n" + "\n".join(tex) + "\n", encoding="utf-8")
print(json.dumps(result, ensure_ascii=False, indent=2))
