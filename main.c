#include <stdio.h>
#include <ctype.h>

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


#define MAXN 13000
#define MAXM 90000

static short x[MAXM], y[MAXM];
static int degree[MAXN];
static short edges[MAXM];

static char degree_rev[MAXN];
static short goodNodes[MAXN];
int size;

static char startPoint[MAXN];
static char endPoint[MAXN];

int n, m;

static inline void ReadInput() {
	while(1) {
		m++;
		x[m] = GetNr();
		if(x[m] > -1) {
			y[m] = GetNr();
		}
		else {
			break;
		}
	}	
	m--;

	for(int i = 0; i <= m; ++i) {
		degree[x[i]]++;
		degree_rev[y[i]] = 1;
	}

	n = MAXN - 1;
	while(degree[n] == 0 && degree_rev[n] == 0) {
		--n;
	}

	for(int i = 0; i <= n; ++i) {
		if(degree[i] && degree_rev[i]) {
			goodNodes[size++] = i;
		}
	}
	for(int i = 1; i <= n + 1; ++i) {
		degree[i] += degree[i - 1];
	}

	for(int i = 1; i <= m; ++i) {
		degree[x[i]]--;
		edges[degree[x[i]]] = y[i];
	}

	for(int i = 0; i <= n; i++) {
		for(int j = degree[i]; j < degree[i + 1]; j++) {
			const int nod = edges[j];
			const int len = degree[nod + 1] - degree[nod];
			if(len > 1 || (len == 1 && edges[degree[nod]] != i)) {
				startPoint[i] = 1;
				break;
			}
		}
	}
}

const int INF = 17; // merge aparent si cu 10

static float g[MAXN];

#include <string.h>

static inline void Solve() {

	#pragma omp parallel for schedule(dynamic) num_threads(44)
	for(int s = 0; s <= n; s++) {

		if(!startPoint[s]) continue;

		const float inv[32] = {0, 1, 0.5, 0.333333,
							   0.25, 0.2, 0.166667,
							   0.142857, 0.125, 0.111111,
							   0.1, 0.0909091, 0.0833333, 0.0769231,
							   0.0714286, 0.0666667, 0.0625, 0.0588235,
							   0.0555556, 0.0526316, 0.05, 0.047619,
							   0.0454545, 0.0434783, 0.0416667, 0.04,
							   0.0384615, 0.037037, 0.0357143, 0.0344828,
							   0.0333333, 0.0322581};

		unsigned char dist[MAXN];
		short ways0[MAXN];
		float ways1[MAXN];
		short Q[MAXN];

		memset(dist, 0, sizeof(unsigned char) * (n + 1));

		int l = 0, r = 1;

		Q[0] = s;
		dist[s] = ways0[s] = 1;

		unsigned short last[MAXN];
		unsigned short nxt[1 << 15];
		short node[1 << 15];
		int sz = 1;

		memset(last, 0, sizeof(unsigned short) * (n + 1));

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

					ways0[nei] = ways0[nod];

					nxt[sz] = last[nod];
					node[sz] = nei;
					last[nod] = sz++;
				}
				else if(dist[nei] == dist[nod] + 1) {
					ways0[nei] += ways0[nod];

					nxt[sz] = last[nod];
					node[sz] = nei;
					last[nod] = sz++;
				}
			}
		}

		if(size < r) {
			memset(ways1, 0, sizeof(float) * (n + 1));
		}

		for(int i = r - 1; i >= 1; --i) {
			const int v = Q[i];
			float sum = 0;
			
			int pos = last[v];
			while(pos) {
				sum += ways1[node[pos]];
				pos = nxt[pos];
			}

			ways1[v] = sum + inv[ways0[v]];
		}

		int pos = 0;
		for(int i = 1; i < r; ++i) {
			if(!endPoint[Q[i]]) {
				Q[pos++] = Q[i];
			}
		}
		r = pos;

		if(size < r) {
			for(int i = 0; i < size; ++i) {
				const int nod = goodNodes[i];
				
				if(ways1[nod]) {
					ways1[nod] *= ways0[nod];
					ways1[nod]--;
				}
			}

			#pragma omp critical
			{
				for(int i = 0; i < size; ++i) {
					const int nod = goodNodes[i];
					g[nod] += ways1[nod];
				}
			}
		}
		else {
			for(int i = 1; i < r; ++i) {
				const int nod = Q[i];
				
				ways1[nod] *= ways0[nod];
				ways1[nod]--;
			}

			#pragma omp critical
			{
				for(int i = 1; i < r; ++i) {
					const int nod = Q[i];
					g[nod] += ways1[nod];
				}
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


