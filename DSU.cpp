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

static pair<int, int> edge[MAXM];
atomic<unsigned short> parent[MAXN];

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
				
			edge[m++] = {a, b};
			n = max(n, max(a, b));
		}	
	}

	#pragma omp parallel for
	for(int i = 0; i <= n; i++) {
		parent[i] = -1;
	}

}

constexpr int BUCKET_SIZE = 6; // 1 << 6 = 64


inline unsigned short GetRoot(unsigned short nod) {
	unsigned short root = nod;
	while(parent[root] < (unsigned short)-1) {
		root = parent[root];	
	}
	unsigned short cur;
	while(parent[nod] < (unsigned short)-1) {
		cur = parent[nod];
		parent[nod] = root;
		nod = cur;
	}
	return root;
}

/*unsigned short GetRoot(unsigned short nod) {
	return (parent[nod] == (unsigned short)-1 ? nod : parent[nod] = GetRoot(parent[nod]));
}*/

inline void Join(unsigned short x, unsigned short y) {
	x = GetRoot(x);
	y = GetRoot(y);

	if(x != y) {
		parent[x] = y;
	}
}


unsigned long long visited[MAXN >> BUCKET_SIZE];

inline void VisitNode(const unsigned short nod) {
	const unsigned short id = (nod >> BUCKET_SIZE);
	const char rem = (nod & ((1 << BUCKET_SIZE) - 1));
	visited[id] |= (1llu << rem);
}


static inline void Solve(int n, int m) {	
	#pragma omp parallel for
	for(int i = 0; i < m; i++) {
		Join(edge[i].first, edge[i].second);
	}


	unsigned short root = GetRoot(0);

	#pragma omp parallel for
	for(int i = 0; i <= n; i++) {
		unsigned short cur = GetRoot(i);
		if(cur == root) {
			VisitNode(i);
		}
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
