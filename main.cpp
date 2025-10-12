#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n, m;
  cin >> n >> m;

  vector<vector<int>> graph(n + 1);
  vector<pair<int, int>> edges;

  for (int i = 0; i < m; ++i) {
    int u, v;
    cin >> u >> v;
    graph[u].push_back(v);
    graph[v].push_back(u);
    edges.emplace_back(u, v);
  }

  vector<int> degree(n + 1);
  for (int i = 1; i <= n; ++i) {
    degree[i] = graph[i].size();
  }

  vector<bool> removed(n + 1, false);
  int current_edge_count = m;
  vector<int> best_subset;
  double best_density = -1.0;

  for (int step = 0; step < n; ++step) {
    int current_size = n - step;
    double density = static_cast<double>(current_edge_count) / current_size;

    if (density > best_density) {
      best_density = density;
      best_subset.clear();
      for (int i = 1; i <= n; ++i) {
        if (!removed[i]) {
          best_subset.push_back(i);
        }
      }
    }

    // Найти вершину с минимальной степенью среди оставшихся
    int min_degree = n + 1;
    int to_remove = -1;
    for (int i = 1; i <= n; ++i) {
      if (!removed[i] && degree[i] < min_degree) {
        min_degree = degree[i];
        to_remove = i;
      }
    }

    if (to_remove == -1)
      break;

    // Удаляем вершину
    removed[to_remove] = true;
    current_edge_count -= degree[to_remove];

    // Уменьшаем степень соседей
    for (int neighbor : graph[to_remove]) {
      if (!removed[neighbor]) {
        degree[neighbor]--;
      }
    }
  }

  cout << best_subset.size() << "\n";
  for (int v : best_subset) {
    cout << v << "\n";
  }

  return 0;
}