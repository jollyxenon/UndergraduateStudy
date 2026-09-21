from itertools import combinations

# 各卡片出现概率
p = [0.05, 0.15, 0.20, 0.25, 0.35]

# 预先计算所有子集的 (|I|, sum_{i in I} p_i)
subsets = []
for k in range(len(p) + 1):
    for combo in combinations(p, k):
        subsets.append((k, sum(combo)))

def compute_Q(n):
    """根据容斥原理公式计算 Q_n"""
    return sum(((-1) ** k) * ((1.0 - p_sum) ** n) for k, p_sum in subsets)

# 寻找满足 Q_n >= 0.90 的最小整数 n
n = 1
while True:
    q_n = compute_Q(n)
    if q_n >= 0.90:
        print(f"minimal n = {n}, and the corresponding Q_n = {q_n:.6f}")
        break
    n += 1