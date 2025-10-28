#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXN 105

int n, m;
int adj[MAXN][MAXN];  // матрица смежности
int current[MAXN];    // текущее подмножество (1 = включена)
int best_set[MAXN];   // лучшее найденное подмножество
int best_size = 0;
double best_density = -1.0;

// Подсчёт плотности текущего подмножества
double compute_density(int* subset, int size) {
  if (size == 0)
    return 0.0;
  int edges = 0;
  for (int i = 1; i <= n; i++) {
    if (!subset[i])
      continue;
    for (int j = i + 1; j <= n; j++) {
      if (subset[j] && adj[i][j]) {
        edges++;
      }
    }
  }
  return (double)edges / size;
}

// Подсчёт степени вершины внутри текущего подмножества
int degree_in_subgraph(int v, int* subset) {
  int deg = 0;
  for (int u = 1; u <= n; u++) {
    if (u != v && subset[u] && adj[v][u])
      deg++;
  }
  return deg;
}

int main() {
  scanf("%d %d", &n, &m);

  // Инициализация
  memset(adj, 0, sizeof(adj));
  for (int i = 0; i < m; i++) {
    int a, b;
    scanf("%d %d", &a, &b);
    adj[a][b] = adj[b][a] = 1;
  }

  // Начинаем с полного множества
  int subset[MAXN];
  for (int i = 1; i <= n; i++) {
    subset[i] = 1;
  }

  int remaining = n;

  // Итеративно удаляем вершины
  while (remaining > 0) {
    double dens = compute_density(subset, remaining);
    if (dens > best_density) {
      best_density = dens;
      best_size = remaining;
      memcpy(best_set, subset, sizeof(subset));
    }

    // Найти вершину с минимальной степенью в текущем подграфе
    int min_deg = n + 1;
    int to_remove = -1;
    for (int v = 1; v <= n; v++) {
      if (!subset[v])
        continue;
      int deg = degree_in_subgraph(v, subset);
      if (deg < min_deg || (deg == min_deg && v < to_remove)) {
        min_deg = deg;
        to_remove = v;
      }
    }

    // Удаляем её
    subset[to_remove] = 0;
    remaining--;
  }

  // Вывод результата
  printf("%d\n", best_size);
  for (int i = 1; i <= n; i++) {
    if (best_set[i]) {
      printf("%d\n", i);
    }
  }

  return 0;
}