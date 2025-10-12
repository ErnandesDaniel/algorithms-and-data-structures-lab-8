#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

const int MAXN = 100000;
const int LOG = 17;

vector<int> adj[MAXN];
int depth[MAXN];
int up[MAXN][LOG];

void dfs(int v, int parent) {
  depth[v] = (parent == -1 ? 0 : depth[parent] + 1);
  up[v][0] = parent;

  for (int i = 1; i < LOG; ++i) {
    if (up[v][i - 1] != -1) {
      up[v][i] = up[up[v][i - 1]][i - 1];
    } else {
      up[v][i] = -1;
    }
  }

  for (int to : adj[v]) {
    if (to != parent) {
      dfs(to, v);
    }
  }
}

int lca(int u, int v) {
  if (depth[u] < depth[v])
    swap(u, v);

  int diff = depth[u] - depth[v];
  for (int i = 0; i < LOG; ++i) {
    if (diff & (1 << i)) {
      u = up[u][i];
    }
  }

  if (u == v)
    return u;

  for (int i = LOG - 1; i >= 0; --i) {
    if (up[u][i] != up[v][i]) {
      u = up[u][i];
      v = up[v][i];
    }
  }
  return up[u][0];
}

int dist(int u, int v) {
  int w = lca(u, v);
  return depth[u] + depth[v] - 2 * depth[w];
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int N;
  cin >> N;

  for (int i = 0; i < N; ++i) {
    fill(up[i], up[i] + LOG, -1);
  }

  for (int i = 0; i < N - 1; ++i) {
    int u, v;
    cin >> u >> v;
    adj[u].push_back(v);
    adj[v].push_back(u);
  }

  dfs(0, -1);  // корень — 0

  int Q;
  cin >> Q;

  while (Q--) {
    int u, v, T;
    cin >> u >> v >> T;
    int d = dist(u, v);
    if (d <= T) {
      cout << "Yes\n";
    } else {
      cout << "No\n";
    }
  }

  return 0;
}