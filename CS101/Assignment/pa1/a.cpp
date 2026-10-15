#include <bits/stdc++.h>

using namespace std;

const int MAXN = 1000001;

struct Interval {
  int st;
  int len;
  int nxt;
} itv_pool[MAXN];
int itv_cnt = 0;

struct School {
  char color;
  int q_head, q_tail;
  int prev, next;
} schools[MAXN];
int school_cnt = 0;
int head = 0;

void add_school(char color, int st, int len) {
  school_cnt++;
  itv_pool[++itv_cnt] = {st, len, 0};
  schools[school_cnt].color = color;
  schools[school_cnt].q_head = schools[school_cnt].q_tail = itv_cnt;
  if (head == 0) {
    head = school_cnt;
  } else {
    schools[school_cnt - 1].next = school_cnt;
    schools[school_cnt].prev = school_cnt - 1;
  }
}

int n;

int main() {
  scanf("%d", &n);

  getchar();

  char prev_c = -1;
  int st = 1;

  for (int i = 1; i <= n; i++) {
    char c;
    c = getchar();
    if (i != n)
      getchar();
    if (i == 1) {
      prev_c = c;
    } else if (c != prev_c) {
      add_school(prev_c, st, i - st);
      st = i;
      prev_c = c;
    }
  }
  add_school(prev_c, st, n - st + 1);

  while (head != 0) {
    int mnl = INT_MAX;
    long long ps = 0;
    int ums = 0;

    for (int it = head; it != 0; it = schools[it].next) {
      ums++;
      ps += itv_pool[schools[it].q_head].st;
      if (itv_pool[schools[it].q_head].len < mnl) {
        mnl = itv_pool[schools[it].q_head].len;
      }
    }

    long long current_sum = ps;
    for (int r = 0; r < mnl; r++) {
      printf("%lld\n", current_sum);
      current_sum += ums;
    }

    for (int it = head; it != 0;) {
      int nxt_it = schools[it].next;
      Interval &front_itv = itv_pool[schools[it].q_head];
      front_itv.st += mnl;
      front_itv.len -= mnl;

      if (front_itv.len == 0) {
        schools[it].q_head = front_itv.nxt;
      }

      if (schools[it].q_head == 0) {
        int p = schools[it].prev;
        int nx = schools[it].next;
        if (p != 0)
          schools[p].next = nx;
        else
          head = nx;
        if (nx != 0)
          schools[nx].prev = p;
      }
      it = nxt_it;
    }

    for (int it = head; it != 0;) {
      int nxt = schools[it].next;
      if (nxt != 0 && schools[it].color == schools[nxt].color) {
        itv_pool[schools[it].q_tail].nxt = schools[nxt].q_head;
        schools[it].q_tail = schools[nxt].q_tail;

        schools[it].next = schools[nxt].next;
        if (schools[nxt].next != 0) {
          schools[schools[nxt].next].prev = it;
        }
      } else {
        it = schools[it].next;
      }
    }
  }

  return 0;
}