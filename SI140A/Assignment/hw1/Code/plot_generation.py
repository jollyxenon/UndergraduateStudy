import matplotlib.pyplot as plt
from itertools import combinations

p_unequal = [0.05, 0.15, 0.20, 0.25, 0.35]
p_equal = [0.20, 0.20, 0.20, 0.20, 0.20]

def get_subsets(p_list):
    subsets = []
    for k in range(len(p_list) + 1):
        for combo in combinations(p_list, k):
            subsets.append((k, sum(combo)))
    return subsets

subsets_unequal = get_subsets(p_unequal)
subsets_equal = get_subsets(p_equal)

def compute_Q(n, subsets):
    return sum(((-1) ** k) * ((1.0 - p_sum) ** n) for k, p_sum in subsets)

n_values = list(range(1, 101))
Q_unequal = [compute_Q(n, subsets_unequal) for n in n_values]
Q_equal = [compute_Q(n, subsets_equal) for n in n_values]

plt.figure(figsize=(10, 6))
plt.plot(n_values, Q_equal, label='Uniform Case ($p_i = 0.2$)', color='tab:blue', linewidth=2)
plt.plot(n_values, Q_unequal, label='Unequal Case ($p = [0.05, 0.15, 0.2, 0.25, 0.35]$)', color='tab:red', linewidth=2)

plt.axhline(0.90, color='gray', linestyle='--', label='Threshold $Q_n = 0.90$')
plt.scatter([19], [compute_Q(19, subsets_equal)], color='tab:blue', zorder=5)
plt.annotate(f'Uniform: n=19 (Q≈{compute_Q(19, subsets_equal):.3f})', 
             xy=(19, compute_Q(19, subsets_equal)), xytext=(22, 0.82),
             arrowprops=dict(arrowstyle='->', color='tab:blue'))

plt.scatter([56], [compute_Q(56, subsets_unequal)], color='tab:red', zorder=5)
plt.annotate(f'Unequal: n=56 (Q≈{compute_Q(56, subsets_unequal):.3f})', 
             xy=(56, compute_Q(56, subsets_unequal)), xytext=(40, 0.70),
             arrowprops=dict(arrowstyle='->', color='tab:red'))

plt.title('$Q_n$ as a Function of $n$ (Uniform vs Unequal Probabilities)', fontsize=14)
plt.xlabel('Number of Draws ($n$)', fontsize=12)
plt.ylabel('Probability of Collecting All Types ($Q_n$)', fontsize=12)
plt.grid(True, linestyle=':', alpha=0.6)
plt.legend(loc='lower right', fontsize=11)
plt.tight_layout()
plt.savefig('contrast_plot.png')
plt.show()