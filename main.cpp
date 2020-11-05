#include <cstdio>
#include <iostream>

#ifdef HOME
	#include <cassert>
#endif


using namespace std;


constexpr int MAXBUF = (1 << 17);

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

static inline const int GetNr() {
	char ch = NextCh();
	while(!isdigit(ch) && ch != ']') {
		ch = NextCh();
	}

	if(isdigit(ch)) {
		int ans = 0;

		do {
			ans = ans * 10 + ch - '0';
			ch = NextCh();
		} while(isdigit(ch));

		return ans;
	}

	return -1;
}



constexpr int MAXN = (1 << 16);
constexpr int MAXM = (int)5e6 + 5;

static int degree[MAXN];
static unsigned short* graph[MAXN];
static pair<unsigned short, unsigned short> edge[MAXM];


static inline void ReadInput(int& n, int& m) {
	int a, b;

	n = m = 0;
	a = 0;

	while(a > -1) {
		a = GetNr();
		if(a > -1) {
			b = GetNr();
			
			#ifdef HOME
				assert(b > -1);	
			#endif

			++degree[a];

			edge[m] = {a, b};
			++m;

			n = max(n, max(a, b));
		}
	}	
	
	#pragma omp parallel for
	for(int i = 0; i <= n; ++i) {
		if(degree[i]) {
			graph[i] = new unsigned short[degree[i]];
			degree[i] = 0;
		}
	}

	for(int i = 0; i < m; ++i) {
		graph[edge[i].first][degree[edge[i].first]] = edge[i].second;
		++degree[edge[i].first];
	}
}


static bool visited[MAXN];
static unsigned short Q[MAXN];


static inline void Solve(int n, int m) {

	
	printf("%d %d\n", n, m);

	unsigned short l = 0, r = 1;
	Q[0] = 0;
	visited[0] = 1;

	printf("[0");
	
	unsigned short nod, it;
	int i;

	while(l < r) {
		nod = Q[l++];

		for(i = 0; i < degree[nod]; i++) {
			it = graph[nod][i];
			if(visited[it] == 0) {	
				visited[it] = 1;
				Q[r++] = it;
				printf(",%hd", it);
			}
		}
	}
	printf("]");
}



int main() {

	Init();

	int n, m;
	ReadInput(n, m);

	Solve(n, m);

	return 0;
}

