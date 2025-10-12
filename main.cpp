#include <iostream>
#include <vector>
using namespace std;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n, m;
  cin >> n >> m;
  vector<vector<int>> g(n + 1);
  vector<int> deg(n + 1);
  for (int i = 0; i < m; ++i) {
    int u, v;
    cin >> u >> v;
    g[u].push_back(v);
    g[v].push_back(u);
    deg[u]++;
    deg[v]++;
  }

  vector<bool> rm(n + 1, false);
  int edges = m;
  vector<int> best;
  int best_e = -1, best_v = 1;

  for (int step = 0; step < n; ++step) {
    int cur_v = n - step;
    if (best_e == -1 || edges * best_v > best_e * cur_v) {
      best_e = edges;
      best_v = cur_v;
      best.clear();
      for (int i = 1; i <= n; ++i)
        if (!rm[i])
          best.push_back(i);
    }

    int min_d = n + 1, to_rm = -1;
    for (int i = 1; i <= n; ++i)
      if (!rm[i] && deg[i] < min_d)
        min_d = deg[i], to_rm = i;

    if (to_rm == -1)
      break;
    rm[to_rm] = true;
    edges -= deg[to_rm];
    for (int v : g[to_rm])
      if (!rm[v])
        deg[v]--;
  }

  cout << best.size() << '\n';
  for (int x : best)
    cout << x << '\n';
  return 0;
}