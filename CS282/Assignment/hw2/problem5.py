from pathlib import Path

import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np


def make_data(seed, n):
    """Generate the data with the specified seed."""
    rng = np.random.default_rng(seed)
    x = rng.uniform(-1.0, 1.0, n)
    y = np.sin(np.pi * x) + rng.normal(0.0, 0.2, n)
    return x, y


def fit(x_train, y_train, p, lam):
    """Solve augmented least squares; leave the intercept unpenalized."""
    phi = np.vander(x_train, p + 1, increasing=True)
    penalty = np.diag([0.0] + [1.0] * p)
    a = np.vstack((phi, np.sqrt(len(x_train) * lam) * penalty))
    b = np.concatenate((y_train, np.zeros(p + 1)))
    return np.linalg.lstsq(a, b, rcond=None)[0]


def predict(x, w):
    """Predict with raw polynomial features."""
    return np.vander(x, len(w), increasing=True) @ w


def mse(x, y, w):
    """Compute MSE without a regularization term."""
    return np.mean((predict(x, w) - y) ** 2)


if __name__ == "__main__":
    out = Path(__file__).resolve().parent / "problem5_figures"
    out.mkdir(exist_ok=True)
    x, y = make_data(282, 40)
    order = np.random.default_rng(2026).permutation(40)
    train_idx, val_idx = order[:25], order[25:]
    degrees = range(1, 11)
    lams = [0.0, 1e-4, 1e-3, 1e-2, 1e-1, 1.0]

    # (ii): Use the fixed training and validation split.
    models = [fit(x[train_idx], y[train_idx], p, 0) for p in degrees]
    errors = np.array([
        [mse(x[train_idx], y[train_idx], w),
         mse(x[val_idx], y[val_idx], w)] for w in models
    ])
    print("(ii): p, training MSE, validation MSE")
    for p, row in zip(degrees, errors):
        print(f"{p:2d}  {row[0]:.8f}  {row[1]:.8f}")
    print("Minimizing degrees:", np.argmin(errors, axis=0) + 1)

    plt.rcParams.update({"font.size": 11, "axes.spines.top": False,
                         "axes.spines.right": False})
    fig, ax = plt.subplots(figsize=(7, 3.5), layout="constrained")
    ax.plot(list(degrees), errors[:, 0], "o-", label="Training")
    ax.plot(list(degrees), errors[:, 1], "s-", label="Validation")
    ax.set(xlabel="Degree p", ylabel="MSE", xticks=list(degrees))
    ax.grid(alpha=0.2)
    ax.legend()
    fig.savefig(out / "errors.pdf")
    plt.close(fig)

    grid = np.linspace(-1, 1, 1000)
    fig, axes = plt.subplots(2, 2, figsize=(8, 5.5), layout="constrained")
    for ax, p in zip(axes.flat, [1, 3, 5, 10]):
        ax.scatter(x[train_idx], y[train_idx], s=22, label="Training")
        ax.plot(grid, predict(grid, models[p - 1]), color="C1",
                label="Fitted polynomial")
        ax.plot(grid, np.sin(np.pi * grid), "--", color="0.4",
                linewidth=1, label=r"$\sin(\pi x)$")
        ax.set(title=f"p = {p}", xlabel="x", ylabel="y", xlim=(-1, 1))
        ax.grid(alpha=0.2)
    axes[0, 0].legend(fontsize=8)
    fig.savefig(out / "fits.pdf")
    plt.close(fig)

    # (iii): Use the same five folds for every candidate.
    cv_order = np.random.default_rng(1024).permutation(40)
    folds = np.array_split(cv_order, 5)
    cv_errors = np.empty((10, 6))
    for i, p in enumerate(degrees):
        for k, lam in enumerate(lams):
            fold_errors = []
            for j in range(5):
                tr = np.concatenate([folds[t] for t in range(5) if t != j])
                va = folds[j]
                w = fit(x[tr], y[tr], p, lam)
                fold_errors.append(mse(x[va], y[va], w))
            cv_errors[i, k] = np.mean(fold_errors)
    print("(iii): columns lambda =", lams)
    for p, row in zip(degrees, cv_errors):
        print(f"{p:2d}  " + "  ".join(f"{v:.8f}" for v in row))
    best = min((cv_errors[i, k], p, lam)
               for i, p in enumerate(degrees) for k, lam in enumerate(lams))
    cv_mse, p_star, lam_star = best
    print(f"Selected: p={p_star}, lambda={lam_star}, CV MSE={cv_mse:.8f}")

    # (iv): Select and refit before generating the test set.
    w_star = fit(x, y, p_star, lam_star)
    x_test, y_test = make_data(2026282, 1000)
    print("Final w:", np.array2string(w_star, precision=10))
    print(f"Test MSE: {mse(x_test, y_test, w_star):.10f}")
