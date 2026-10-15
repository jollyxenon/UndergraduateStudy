#include <bits/stdc++.h>
using namespace std;
int n;
int init[200001][2], term[200001][2];
int pos[200001];
int temp[200001];
int col_of[400001];
int row_of[400001];
long long inver_pair_count(int l, int r) {
  if (r - l <= 1)
    return 0;
  int mid = (l + r) / 2;
  long long res = inver_pair_count(l, mid) + inver_pair_count(mid, r);
  int i = l, j = mid;

  while (i < mid && j < r) {
    if (pos[i] > pos[j]) {
      temp[i + j - mid] = pos[j];
      res += mid - i;
      j++;
    } else {
      temp[i + j - mid] = pos[i];
      i++;
    }
  }
  while (i < mid) {
    temp[i + j - mid] = pos[i];
    i++;
  }
  while (j < r) {
    temp[i + j - mid] = pos[j];
    j++;
  }
  for (int k = l; k < r; k++) {
    pos[k] = temp[k];
  }

  return res;
}

int main() {
  scanf("%d", &n);
  for (int i = 0; i < n; i++) {
    scanf("%d", &init[i][0]);
  }
  for (int i = 0; i < n; i++) {
    scanf("%d", &init[i][1]);
  }
  for (int i = 0; i < n; i++) {
    scanf("%d", &term[i][0]);
    col_of[term[i][0]] = i;
    row_of[term[i][0]] = 0;
  }
  for (int i = 0; i < n; i++) {
    scanf("%d", &term[i][1]);
    col_of[term[i][1]] = i;
    row_of[term[i][1]] = 1;
  }

  for (int i = 0; i < n; i++) {
    int u = init[i][0];
    int v = init[i][1];

    if (col_of[u] != col_of[v]) {
      printf("-1\n");
      return 0;
    }

    int target_col = col_of[u];
    bool is_rev = (row_of[u] == 1);

    if (abs(target_col - i) % 2 != (int)is_rev) {
      printf("-1\n");
      return 0;
    }

    pos[i] = target_col;
  }

  printf("%lld\n", inver_pair_count(0, n));
  return 0;
}