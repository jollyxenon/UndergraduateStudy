#include <bits/stdc++.h>
using namespace std;

int T;
int n, k;

inline int inj(char c) {
  if ('a' <= c && c <= 'z')
    return c - 'a';
  if ('A' <= c && c <= 'Z')
    return c - 'A' + 26;
  if ('0' <= c && c <= '9')
    return c - '0' + 52;
  return -1;
}

char s[1000005];
int ori[1000005];
int pres[1000005][62];
queue<pair<int, int>> avail_intervals;

int main() {
  scanf("%d", &T);

  while (T--) {
    scanf("%d%d", &n, &k);
    scanf("%s", s + 1);

    for (int j = 0; j < 62; j++) {
      pres[0][j] = 0;
    }

    for (int i = 1; i <= n; i++) {
      ori[i] = inj(s[i]);
      for (int j = 0; j < 62; j++) {
        pres[i][j] = pres[i - 1][j];
      }
      if (ori[i] != -1) {
        pres[i][ori[i]]++;
      }
    }

    queue<pair<int, int>>().swap(avail_intervals);
    avail_intervals.push(make_pair(1, n));

    int max_len = 0;

    while (!avail_intervals.empty()) {
      pair<int, int> cur = avail_intervals.front();
      avail_intervals.pop();

      int l = cur.first;
      int r = cur.second;

      if (r - l + 1 <= max_len) {
        continue;
      }

      bool is_bad[62] = {false};
      bool has_bad = false;

      for (int j = 0; j < 62; j++) {
        int count = pres[r][j] - pres[l - 1][j];
        if (count > 0 && count < k) {
          is_bad[j] = true;
          has_bad = true;
        }
      }

      if (!has_bad) {
        max_len = max(max_len, r - l + 1);
        continue;
      }

      int seg_start = l;
      for (int i = l; i <= r; i++) {
        if (ori[i] == -1 || is_bad[ori[i]]) {
          if (i - 1 - seg_start + 1 > max_len) {
            avail_intervals.push(make_pair(seg_start, i - 1));
          }
          seg_start = i + 1;
        }
      }
      if (r - seg_start + 1 > max_len) {
        avail_intervals.push(make_pair(seg_start, r));
      }
    }

    printf("%d\n", max_len);
  }

  return 0;
}