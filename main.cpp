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
  for (int i = 0; i < m; ++i) {
    int u, v;
    cin >> u >> v;
    graph[u].push_back(v);
    graph[v].push_back(u);
  }

  vector<int> degree(n + 1);
  for (int i = 1; i <= n; ++i) {
    degree[i] = static_cast<int>(graph[i].size());
  }

  vector<bool> removed(n + 1, false);
  long long current_edge_count = m;  // используем long long для безопасности
  vector<int> best_subset;
  long long best_edges = -1;
  int best_vertices = 1;

  for (int step = 0; step < n; ++step) {
    int current_size = n - step;

    // Сравниваем: current_edge_count / current_size > best_edges / best_vertices
    // ⇨ current_edge_count * best_vertices > best_edges * current_size
    if (best_edges == -1 || current_edge_count * best_vertices > best_edges * current_size) {
      best_edges = current_edge_count;
      best_vertices = current_size;
      best_subset.clear();
      for (int i = 1; i <= n; ++i) {
        if (!removed[i]) {
          best_subset.push_back(i);
        }
      }
    }

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

    removed[to_remove] = true;
    current_edge_count -= degree[to_remove];

    for (int neighbor : graph[to_remove]) {
      if (!removed[neighbor]) {
        degree[neighbor]--;
      }
    }
  }

  sort(best_subset.begin(), best_subset.end());
  cout << best_subset.size() << "\n";
  for (int v : best_subset) {
    cout << v << "\n";
  }

  return 0;
}