#include <stdbool.h>
#include <stdio.h>

#define MAX_N 101

int main() {
  int n, m;
  scanf("%d %d", &n, &m);

  bool adj[MAX_N][MAX_N] = {false};
  struct Edge {
    int a, b;
  };
  struct Edge edges[m];  // Массив для хранения ребер

  for (int i = 0; i < m; i++) {
    int a, b;
    scanf("%d %d", &a, &b);
    edges[i].a = a;
    edges[i].b = b;
    adj[a][b] = true;
    adj[b][a] = true;
  }

  double best_ratio = -1.0;
  int best_set[MAX_N] = {0};
  int best_size = 0;

  // Обработка начальных множеств из одной вершины
  for (int start_v = 1; start_v <= n; start_v++) {
    bool in_set[MAX_N] = {false};
    in_set[start_v] = true;
    int E = 0;
    int V = 1;

    int count_edges[MAX_N] = {0};
    for (int w = 1; w <= n; w++) {
      if (w == start_v)
        continue;
      count_edges[w] = adj[start_v][w];
    }

    bool improved = true;
    while (improved) {
      improved = false;
      int best_vertex = -1;
      double best_ratio_candidate = (double)E / V;

      for (int w = 1; w <= n; w++) {
        if (in_set[w])
          continue;
        int new_E = E + count_edges[w];
        int new_V = V + 1;
        if (new_V == 0)
          continue;
        double new_ratio = (double)new_E / new_V;
        if (new_ratio > best_ratio_candidate) {
          best_ratio_candidate = new_ratio;
          best_vertex = w;
        }
      }

      if (best_vertex != -1) {
        improved = true;
        in_set[best_vertex] = true;
        E += count_edges[best_vertex];
        V += 1;
        // Обновляем count_edges для всех w не в in_set
        for (int w = 1; w <= n; w++) {
          if (!in_set[w]) {
            count_edges[w] += adj[best_vertex][w];
          }
        }
      }
    }

    double current_ratio = (V == 0) ? 0 : ((double)E / V);
    if (current_ratio > best_ratio || (current_ratio == best_ratio && V > best_size) ||
        (current_ratio == best_ratio && V == best_size && best_size == 0)) {
      best_ratio = current_ratio;
      best_size = V;
      // Копируем in_set в best_set
      int k = 0;
      for (int w = 1; w <= n; w++) {
        if (in_set[w]) {
          best_set[k++] = w;
        }
      }
    }
  }

  // Обработка начальных множеств из ребер
  for (int i = 0; i < m; i++) {
    int a = edges[i].a;
    int b = edges[i].b;

    bool in_set[MAX_N] = {false};
    in_set[a] = true;
    in_set[b] = true;
    int E = adj[a][b] ? 1 : 0;
    int V = 2;

    int count_edges[MAX_N] = {0};
    for (int w = 1; w <= n; w++) {
      if (w == a || w == b)
        continue;
      count_edges[w] = adj[a][w] + adj[b][w];
    }

    bool improved = true;
    while (improved) {
      improved = false;
      int best_vertex = -1;
      double best_ratio_candidate = (double)E / V;

      for (int w = 1; w <= n; w++) {
        if (in_set[w])
          continue;
        int new_E = E + count_edges[w];
        int new_V = V + 1;
        if (new_V == 0)
          continue;
        double new_ratio = (double)new_E / new_V;
        if (new_ratio > best_ratio_candidate) {
          best_ratio_candidate = new_ratio;
          best_vertex = w;
        }
      }

      if (best_vertex != -1) {
        improved = true;
        in_set[best_vertex] = true;
        E += count_edges[best_vertex];
        V += 1;
        for (int w = 1; w <= n; w++) {
          if (!in_set[w]) {
            count_edges[w] += adj[best_vertex][w];
          }
        }
      }
    }

    double current_ratio = (V == 0) ? 0 : ((double)E / V);
    if (current_ratio > best_ratio || (current_ratio == best_ratio && V > best_size) ||
        (current_ratio == best_ratio && V == best_size && best_size == 0)) {
      best_ratio = current_ratio;
      best_size = V;
      int k = 0;
      for (int w = 1; w <= n; w++) {
        if (in_set[w]) {
          best_set[k++] = w;
        }
      }
    }
  }

  // Вывод результата
  printf("%d\n", best_size);
  for (int i = 0; i < best_size; i++) {
    printf("%d\n", best_set[i]);
  }

  return 0;
}