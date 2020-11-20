#include <stdio.h>
#include <string.h>
#define ull unsigned long long

#define MAXBUF (1 << 17)

static char buf[MAXBUF];
int pbuf;


static inline void Init() {
	pbuf = MAXBUF;
}

static inline const char NextCh() {
	if(pbuf == MAXBUF) {
		fread(buf, 1, MAXBUF, stdin);
		pbuf = 0;
	}
	return buf[pbuf++];
}

static inline const short GetNr() { 
	char ch = NextCh();
	while(!isdigit(ch) && ch != ']') {
		ch = NextCh();
	}

	if(isdigit(ch)) {
		short ans = 0;

		do {
			ans = ans * 10 + ch - '0';
			ch = NextCh();
		} while(isdigit(ch));

		return ans;
	}

	return -1;
}

static inline int max(int a, int b) {
	return a > b ? a : b;
}


#define MAXN 100000
#define MAXM 5000000

static int x[MAXM], y[MAXM];
static int degree[2][MAXN];
static int edges[2][MAXM]; // graful normal si inversat

int n, m;

static inline void ReadInput() {
	while(1) {
		++m;
		x[m] = GetNr();
		if(x[m] > -1) {
			y[m] = GetNr();
			degree[0][x[m]]++;
			degree[1][y[m]]++;
		}
		else {
			break;
		}
	}	
	m--;
	
	for(int i = 1; i < MAXN; ++i) {
		if(degree[0][i] || degree[1][i]) {
			n = max(n, i);
		}
		degree[0][i] += degree[0][i - 1];
		degree[1][i] += degree[1][i - 1];
	}
	for(int i = m; i >= 1; --i) {
		degree[0][x[i]]--;
		edges[0][degree[0][x[i]]] = y[i];

		degree[1][y[i]]--;
		edges[1][degree[1][y[i]]] = x[i];
	}
}


#include <assert.h>

static int dist[2][MAXN];
static ull ways[2][MAXN];
static int Q[MAXN];
static char visited[MAXN];
static double g[MAXN];
static double answer[MAXN];

static inline void Solve() {
	for(int s = 0; s <= n; s++) {
		#pragma omp parallel for
		for(int i = 0; i <= n; i++) {
			dist[0][i] = ways[0][i] = 0;
		}
	
		int sz = 1, cur_dist = 1;
		dist[0][s] = ways[0][s] = 1;

		while(sz) {
			cur_dist++;
			sz = 0;

			#pragma omp parallel for reduction(+:sz)
			for(int i = 0; i <= n; ++i) {
				if(dist[0][i] == 0) {
					for(int j = degree[1][i]; j < degree[1][i + 1]; ++j) {
						if(dist[0][edges[1][j]] == cur_dist - 1) {
							sz += (dist[0][i] == 0);
							dist[0][i] = cur_dist;
							ways[0][i] += ways[0][edges[1][j]];
						}
					}
				}
			}
		}

		int l = 0, r = 1;
		Q[0] = s;
		visited[s] = 1;
		while(l < r) {
			int nod = Q[l++];

			for(int i = degree[0][nod]; i < degree[0][nod + 1]; ++i) {
				int v = edges[0][i];
				if(visited[v] == 0) {
					Q[r++] = v;
					visited[v] = 1;
				}
			}
		}

		l = 0;
		visited[s] = 0;
		while(l < r) {
			int nod = Q[l++];

			for(int i = degree[0][nod]; i < degree[0][nod + 1]; ++i) {
				int v = edges[0][i];
				if(visited[v] == 1) {

					#pragma omp parallel for
					for(int i = 0; i < r; ++i) {
						dist[1][Q[i]] = ways[1][Q[i]] = 0;
					}

					visited[v] = 0;
					
					cur_dist = sz = 1;
					dist[1][v] = ways[1][v] = 1;

					while(sz) {
						cur_dist++;
						sz = 0;		
				
						#pragma omp parallel for reduction(+:sz)
						for(int i = 0; i < r; ++i) {
							int t = Q[i];

							/*if(v == 0 && s == 1 && t == 5) {
								printf("%d %d %d %d\n", s, v, t, dist[1][t]);
							}*/

							if(t != s && t != v && dist[1][t] == 0) {
								for(int j = degree[1][t]; j < degree[1][t + 1]; ++j) {
									if(dist[1][edges[1][j]] == cur_dist - 1) {
										sz += (dist[1][t] == 0);
										dist[1][t] = cur_dist;
										ways[1][t] += ways[1][edges[1][j]];
									}
								}	

								if(dist[1][t]) {	
									if(dist[0][v] + dist[1][t] == dist[0][t] + 1) {
										assert(ways[0][v] * ways[1][t] <= ways[0][t]);

										/*if(v == 0)
										printf("%d %d %d %llu %llu %llu\n", s, v, t, ways[0][v], ways[1][t], ways[0][t]);
										*/

										g[v] += 1.0 * (1.0 * ways[0][v] / ways[0][t]) * ways[1][t];
									}
								}
							}
						}
					}
				}
			}
		}
	}

	double mx = 0, mn = 1LL << 62;
	#pragma omp parallel for reduction(min:mn) reduction(max:mx)
	for(int i = 0; i <= n; i++) {
		if(mx < g[i]) mx = g[i];
		if(mn > g[i]) mn = g[i];
	}

	if(mx - mn > 0) {
		for(int i = 0; i <= n; i++) {
			answer[i] = 1.0 * (g[i] - mn) / (mx - mn);
		}
	}

	printf("[");
	for(int i = 0; i <= n; i++) {
		if(i) {
			printf(",");
		}
		printf("(%d,%.2lf)", i, answer[i]);
	}
	printf("]");
}



int main() {
	Init();

	ReadInput();

	Solve();

	return 0;
}


