#include <cstdio>
#include <cstdlib>
#include <atomic>
#include <utility>
#include <cstring>

#ifdef HOME
	#include <iostream>
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
constexpr int MAXM = (1 << 19);

static atomic<int> degree[MAXN];
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

			degree[a]++;
			degree[b]++;
			edge[m++] = {a, b};
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
	
	#pragma omp parallel for
	for(int i = 0; i < m; ++i) {
		graph[edge[i].first][degree[edge[i].first]++] = edge[i].second;
		graph[edge[i].second][degree[edge[i].second]++] = edge[i].first;
	}
}


constexpr int BUCKET_SIZE = 6; // 1 << 6 = 64

static atomic<unsigned long long> visited[MAXN / (1 << BUCKET_SIZE)];
static atomic<unsigned short> nodes[2][MAXN];

static inline const bool isVisited(const unsigned short nod) {
	const unsigned short id = (nod >> BUCKET_SIZE);
	const char rem = (nod & ((1 << BUCKET_SIZE) - 1));
	return (visited[id] & (1llu << rem)) > 0;
}

static inline void VisitNode(const unsigned short nod) {
	const unsigned short id = (nod >> BUCKET_SIZE);
	const char rem = (nod & ((1 << BUCKET_SIZE) - 1));
	visited[id] |= (1llu << rem);
}


static inline void Solve(int n, int m) {
	static atomic<int> sz[2];
	sz[1] = 0;
		
	bool t = 0;

	visited[0] = 1;
	sz[0] = 1;

	while(sz[t]) {
		sz[t ^ 1] = 0;

		#pragma omp parallel for
		for(int i = 0; i < sz[t]; ++i) {
			#pragma omp parallel for
			for(int j = 0; j < degree[nodes[t][i]]; ++j) {
				unsigned short new_nod = graph[nodes[t][i]][j];

				#ifdef HOME
					assert(new_nod <= n);
				#endif

				if(!isVisited(new_nod)) {
					VisitNode(new_nod);
					nodes[t ^ 1][sz[t ^ 1]] = new_nod;
					sz[t ^ 1]++;
				}
			}
		}

		sz[t] = 0;
		t ^= 1;
	}
}


static inline void PrintSol(int n, int m) {
	bool first = true;

	printf("[");
	
	const int lim = (n >> BUCKET_SIZE) + 1;
	int bit;
	
	for(int i = 0; i < lim; ++i) {
		if(visited[i]) {
			for(bit = 0; bit < (1 << BUCKET_SIZE); ++bit) {
				if(visited[i] & (1llu << bit)) {
					if(first) {
						printf("%hd", (i << BUCKET_SIZE) + bit);
						first = false;
					}
					else {
						printf(",%hd", (i << BUCKET_SIZE) + bit);
					}
				}
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

	PrintSol(n, m);

	return 0;
}
