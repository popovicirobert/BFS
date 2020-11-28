#include <stdio.h>
#include <ctype.h>
//#include <omp.h>
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


#define MAXN 14000
#define MAXM 900000

static short x[MAXM], y[MAXM];
static int degree[MAXN];
static short edges[MAXM];

int n, m;

static inline void ReadInput() {
	while(1) {
		m++;
		x[m] = GetNr();
		if(x[m] > -1) {
			y[m] = GetNr();
			degree[x[m]]++;
		}
		else {
			break;
		}
	}	
	m--;

	for(int i = 1; i <= m; ++i) {
		n = max(n, y[i]);
	}
	
	if(MAXN < m) {
		for(int i = 1; i < MAXN; ++i) {
			if(degree[i]) {
				n = max(n, i);
			}
		}
	}
	else {
		for(int i = 1; i <= m; ++i) {
			n = max(n, x[i]);
		}
	}


	for(int i = 1; i <= n + 1; ++i) {
		degree[i] += degree[i - 1];
	}
	for(int i = 1; i <= m; ++i) {
		degree[x[i]]--;
		edges[degree[x[i]]] = y[i];
	}
}

const ull INF = 1e2;

static double g[MAXN];

#include <string.h>

static inline void Solve() {
	#pragma omp parallel for schedule(static) num_threads(32)
	for(int s = 0; s <= n; s++) {

		if(degree[s] == degree[s + 1]) continue;

		short dist[MAXN];
		ull ways0[MAXN];
		double ways1[MAXN];
		short Q[MAXN];
		double G[MAXN];

		memset(dist, 0, sizeof(int) * (n + 1));
		memset(ways0, 0, sizeof(ull) * (n + 1));
		memset(ways1, 0, sizeof(double) * (n + 1));
		memset(G, 0, sizeof(double) * (n + 1));

		int l = 0, r = 1;

		Q[0] = s;
		dist[s] = ways0[s] = 1;

		while(l < r) {
			const int nod = Q[l++];
			if(ways0[nod] > INF) {
				continue;
			}
			for(int i = degree[nod]; i < degree[nod + 1]; ++i) {
				const int nei = edges[i];
				if(!dist[nei]) {
					Q[r++] = nei;
					dist[nei] = dist[nod] + 1;
				}
				if(dist[nei] == dist[nod] + 1) {
					ways0[nei] += ways0[nod];
				}
			}
		}

		for(int i = r - 1; i >= 0; --i) {
			const int v = Q[i];
			double sum = 0;
			
			for(int j = degree[v]; j < degree[v + 1]; ++j) {
				const int nei = edges[j];
				if(dist[v] + 1 == dist[nei]) {
					sum += ways1[nei];
				}
			}

			ways1[v] += sum;

			if(s != v) {
				G[v] += ways0[v] * ways1[v];
			}

			ways1[v] += 1.0 / ways0[v];	
		}

		#pragma omp critical
		{
			for(int i = 0; i <= n; i++) {
				g[i] += G[i];
			}
		}
	}

	double mx = 0, mn = 1LL << 62;
	for(int i = 0; i <= n; ++i) {
		if(mx < g[i]) mx = g[i];
		if(mn > g[i]) mn = g[i];
	}

	if(mx - mn > 0) {
		printf("[");
		for(int i = 0; i <= n; ++i) {
			if(i) {
				printf(",");
			}
			printf("(%d,%.2lf)", i, 1.0 * (g[i] - mn) / (mx - mn));
		}
		printf("]");
	}
	else {
		printf("[");
		for(int i = 0; i <= n; ++i) {
			if(i) {
				printf(",");
			}
			printf("(%d,0.00)", i);
		}
		printf("]");
	}
}


int main() {

	Init();

	ReadInput();

	Solve();

	return 0;
}


