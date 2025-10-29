#include <algorithm>
#include <iomanip>
#include <iostream>
#include <limits>
#include <queue>
#include <vector>

using namespace std;

// Структура для ребра в остаточной сети
struct Edge {
  int to;      // вершина, в которую ведет ребро
  int rev;     // индекс обратного ребра в adj[to]
  double cap;  // пропускная способность

  // Порядок инициализации соответствует порядку объявления: to, rev, cap
  Edge(int t, int r, double c) : to(t), rev(r), cap(c) {
  }
};

// Класс для реализации алгоритма Dinic
struct Dinic {
  vector<vector<Edge>> adj;  // список смежности
  vector<int> level;  // уровень вершины (расстояние от истока)
  vector<int> ptr;  // указатель на следующее ребро при обходе (для оптимизации)
  int n;            // количество вершин

  Dinic(int n) : n(n) {
    adj.resize(n);
    level.resize(n);
    ptr.resize(n);
  }

  // Добавление ребра (и обратного ребра)
  void add_edge(int u, int v, double cap) {
    adj[u].push_back(Edge(v, adj[v].size(), cap));
    adj[v].push_back(Edge(u, adj[u].size() - 1, 0.0));
  }

  // BFS для построения уровня (layered network)
  bool bfs(int s, int t) {
    fill(level.begin(), level.end(), -1);
    queue<int> q;
    q.push(s);
    level[s] = 0;
    while (!q.empty()) {
      int u = q.front();
      q.pop();
      for (auto& e : adj[u]) {
        // Проверяем, есть ли оставшаяся пропускная способность
        if (e.cap > 0 && level[e.to] == -1) {
          level[e.to] = level[u] + 1;
          q.push(e.to);
        }
      }
    }
    return level[t] != -1;  // Достижим ли сток?
  }

  // DFS для поиска блокирующего потока
  double dfs(int u, int t, double flow) {
    if (u == t)
      return flow;
    for (int& i = ptr[u]; i < (int)adj[u].size(); ++i) {
      auto& e = adj[u][i];
      // Проверяем, является ли ребро частью уровня и есть ли пропускная способность
      if (e.cap > 0 && level[e.to] == level[u] + 1) {
        double f = dfs(e.to, t, min(flow, e.cap));
        if (f > 0) {
          e.cap -= f;  // Уменьшаем пропускную способность прямого ребра
          adj[e.to][e.rev].cap += f;  // Увеличиваем пропускную способность обратного ребра
          return f;  // Возвращаем найденный поток
        }
      }
    }
    return 0;  // Не удалось найти путь
  }

  // Основная функция для нахождения максимального потока
  double max_flow(int s, int t) {
    double flow = 0;
    while (bfs(s, t)) {                 // Пока существует путь уровня
      fill(ptr.begin(), ptr.end(), 0);  // Сбрасываем указатели
      while (double f = dfs(s, t, numeric_limits<double>::max())) {
        flow += f;  // Суммируем поток
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

  // Обработка тривиального случая: нет рёбер
  if (m == 0) {
    cout << 1 << "\n1\n";
    return 0;
  }

  // Считываем рёбра и строим структуры данных
  vector<vector<int>> edges(n + 1);
  vector<int> deg(n + 1, 0);
  vector<pair<int, int>> edge_list(m);

  for (int i = 0; i < m; ++i) {
    int a, b;
    cin >> a >> b;
    // Граф неориентированный
    edges[a].push_back(b);
    edges[b].push_back(a);
    deg[a]++;
    deg[b]++;
    edge_list[i] = {a, b};
  }

  // Бинарный поиск по плотности gamma
  // Плотность лежит в [0, m] (максимальная плотность подграфа не превышает m)
  double left = 0.0;
  double right = m;

  // Функция проверки, существует ли подграф с плотностью >= gamma
  auto check = [&](double gamma) -> bool {
    // Размер вспомогательной сети: n вершин + 2 (исток s, сток t)
    int N = n + 2;
    int s = n;  // Исток (нумерация с 0, значит вершины 0..n-1, s = n)
    int t = n + 1;  // Сток
    Dinic dinic(N);

    // Шаг 1: Добавляем рёбра s -> v_i с пропускной способностью U (например, n)
    for (int u = 1; u <= n; ++u) {
      dinic.add_edge(s, u - 1, n);  // Индексация в Dinic с 0, поэтому u-1
    }

    // Шаг 2: Добавляем рёбра v_i -> t с пропускной способностью U + 2*gamma - deg[v_i]
    // U = n, значит n + 2*gamma - deg[u]
    for (int u = 1; u <= n; ++u) {
      dinic.add_edge(u - 1, t, n + 2 * gamma - deg[u]);  // Индексация в Dinic с 0
    }

    // Шаг 3: Добавляем рёбра между соседями с пропускной способностью 1
    for (auto& e : edge_list) {
      // Рёбра в графе нумеруются от 1, в Dinic от 0
      int u_idx = e.first - 1;
      int v_idx = e.second - 1;
      dinic.add_edge(u_idx, v_idx, 1.0);
      dinic.add_edge(v_idx, u_idx, 1.0);
    }

    double flow = dinic.max_flow(s, t);

    // Согласно теореме:
    // Существует подграф плотности >= gamma <=> min-cut < U * n = n * n
    // Мы ищем максимальную плотность, поэтому если min-cut < n*n,
    // то плотность >= gamma возможна, и мы ищем выше.
    return flow < static_cast<double>(n * n);
  };

  // Выполняем бинарный поиск
  // 50 итераций достаточно для достижения высокой точности с double
  for (int iter = 0; iter < 50; ++iter) {
    double mid = (left + right) / 2.0;
    if (check(mid)) {
      left = mid;  // Найден подграф, плотность может быть >= mid, ищем выше
    } else {
      right = mid;  // Подграфа с плотностью >= mid нет, ищем ниже
    }
  }

  // left теперь приближённо равно максимальной плотности gamma_max
  double max_gamma = left;

  // --- Построение итогового решения ---
  // Строим сеть для найденной максимальной плотности gamma_max
  int N_final = n + 2;
  int s_final = n;
  int t_final = n + 1;
  Dinic dinic_final(N_final);

  for (int u = 1; u <= n; ++u) {
    dinic_final.add_edge(s_final, u - 1, n);
    dinic_final.add_edge(u - 1, t_final, n + 2 * max_gamma - deg[u]);
  }

  for (auto& e : edge_list) {
    int u_idx = e.first - 1;
    int v_idx = e.second - 1;
    dinic_final.add_edge(u_idx, v_idx, 1.0);
    dinic_final.add_edge(v_idx, u_idx, 1.0);
  }

  // Запускаем max_flow, чтобы получить остаточную сеть
  dinic_final.max_flow(s_final, t_final);

  // Находим минимальный разрез: вершины, достижимые из s_final в остаточной сети
  vector<bool> vis(N_final, false);
  queue<int> q;
  q.push(s_final);
  vis[s_final] = true;

  while (!q.empty()) {
    int u = q.front();
    q.pop();
    for (auto& e : dinic_final.adj[u]) {
      // Ребро достижимо в остаточной сети, если его остаточная пропускная способность > 0
      if (e.cap > 0 && !vis[e.to]) {
        vis[e.to] = true;
        q.push(e.to);
      }
    }
  }

  // Собираем вершины, входящие в минимальный разрез (они соответствуют искомому подграфу S)
  vector<int> result;
  for (int u = 0; u < n; ++u) {  // Проверяем вершины 0 .. n-1 (исходные вершины 1 .. n)
    if (vis[u]) {
      result.push_back(u + 1);  // Преобразуем обратно к нумерации 1 .. n
    }
  }

  // Выводим результат
  cout << result.size() << "\n";
  for (int v : result) {
    cout << v << "\n";
  }

  return 0;
}