#include <algorithm>
#include <iomanip>
#include <iostream>
#include <limits>
#include <queue>
#include <vector>

using namespace std;

struct Edge {
  int to, rev;
  double cap;
  Edge(int t, double c, int r) : to(t), rev(r), cap(c) {
  }  // Поменяли местами rev и cap
};

struct Dinic {
  vector<vector<Edge>> adj;
  vector<int> level, ptr;
  int n;

  Dinic(int n) : n(n) {
    adj.resize(n);
    level.resize(n);
    ptr.resize(n);
  }

  void add_edge(int u, int v, double cap) {
    adj[u].push_back(Edge(v, cap, adj[v].size()));
    adj[v].push_back(Edge(u, 0, adj[u].size() - 1));
  }

  bool bfs(int s, int t) {
    fill(level.begin(), level.end(), -1);
    queue<int> q;
    q.push(s);
    level[s] = 0;
    while (!q.empty()) {
      int u = q.front();
      q.pop();
      for (auto& e : adj[u]) {
        if (e.cap > 0 && level[e.to] == -1) {
          level[e.to] = level[u] + 1;
          q.push(e.to);
        }
      }
    }
    return level[t] != -1;
  }

  double dfs(int u, int t, double flow) {
    if (u == t)
      return flow;
    for (int& i = ptr[u]; i < (int)adj[u].size(); ++i) {  // Привели к int
      auto& e = adj[u][i];
      if (e.cap > 0 && level[e.to] == level[u] + 1) {
        double f = dfs(e.to, t, min(flow, e.cap));
        if (f > 0) {
          e.cap -= f;
          adj[e.to][e.rev].cap += f;
          return f;
        }
      }
    }
    return 0;
  }

  double max_flow(int s, int t) {
    double flow = 0;
    while (bfs(s, t)) {
      fill(ptr.begin(), ptr.end(), 0);
      while (double f = dfs(s, t, numeric_limits<double>::max())) {
        flow += f;
      }
    }
    return flow;
  }
};

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);

  int n, m;
  cin >> n >> m;

  if (m == 0) {
    cout << 1 << "\n1\n";
    return 0;
  }

  vector<vector<int>> edges(n + 1);
  vector<int> deg(n + 1, 0);
  vector<pair<int, int>> edge_list(m);

  for (int i = 0; i < m; ++i) {
    int a, b;
    cin >> a >> b;
    edges[a].push_back(b);
    edges[b].push_back(a);
    deg[a]++;
    deg[b]++;
    edge_list[i] = {a, b};
  }

  // Binary search on density
  double left = 0.0, right = m;

  auto check = [&](double gamma) -> int {
    int N = n + 2;
    int s = n, t = n + 1;
    Dinic dinic(N);

    for (int u = 1; u <= n; ++u) {
      dinic.add_edge(s, u, n);
      dinic.add_edge(u, t, n + 2 * gamma - deg[u]);
    }

    for (auto& e : edge_list) {
      dinic.add_edge(e.first, e.second, 1);
      dinic.add_edge(e.second, e.first, 1);
    }

    double flow = dinic.max_flow(s, t);
    // double expected = n * (n - (flow / n));  // Удалили
    // Check if there's a subset S with density >= gamma
    // We check if mincut is less than n * n
    if (flow < n * n) {
      return 1;
    }
    return -1;
  };

  // Binary search
  for (int iter = 0; iter < 30; ++iter) {
    double mid = (left + right) / 2.0;
    if (check(mid) >= 0)
      left = mid;
    else
      right = mid;
  }

  // Now, left is approximately the max density
  double max_gamma = left;

  // Build graph for max_gamma and find min-cut
  int N = n + 2;
  int s = n, t = n + 1;
  Dinic dinic(N);

  for (int u = 1; u <= n; ++u) {
    dinic.add_edge(s, u, n);
    dinic.add_edge(u, t, n + 2 * max_gamma - deg[u]);
  }

  for (auto& e : edge_list) {
    dinic.add_edge(e.first, e.second, 1);
    dinic.add_edge(e.second, e.first, 1);
  }

  dinic.max_flow(s, t);

  // Find reachable from s in residual graph
  vector<bool> vis(N, false);
  queue<int> q;
  q.push(s);
  vis[s] = true;

  while (!q.empty()) {
    int u = q.front();
    q.pop();
    for (auto& e : dinic.adj[u]) {
      if (e.cap > 1e-6 && !vis[e.to]) {
        vis[e.to] = true;
        q.push(e.to);
      }
    }
  }

  vector<int> result;
  for (int u = 1; u <= n; ++u) {
    if (vis[u]) {
      result.push_back(u);
    }
  }

  cout << result.size() << "\n";
  for (int v : result) {
    cout << v << "\n";
  }

  return 0;
}