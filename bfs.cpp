#include <cstdio>
#include <iostream>
#include <queue>

#ifdef HOME
	#include <cassert>
#endif


using namespace std;


constexpr int MAXBUF = (1 << 17);
//constexpr int SIGMA = 256;

static char buf[MAXBUF];
int pbuf;

//static bool chars[SIGMA];

static inline void Init() {
	pbuf = MAXBUF;
	/*for(char ch = '0'; ch <= '9'; ch++) {
		chars[ch] = true;
	}*/
}

/*static inline const bool isdigit(char ch) {
	return chars[ch];
}*/

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


/*static inline const int max(int a, int b) {
	if(a < b) return b;
	return a;
}*/

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


static inline void Solve(int n, int m) {

	queue<unsigned short> Q;
	Q.push(0);
	visited[0] = 1;

	printf("[0");

	while(Q.size()) {
		int nod = Q.front();
		Q.pop();

		for(int i = 0; i < degree[nod]; i++) {
			int it = graph[nod][i];
			if(visited[it] == 0) {	
				visited[it] = 1;
				printf(",%hd" ,it);
			}
		}
	}

	printf("]");
}



static inline void PrintSol(int n, int m) {
	bool first = true;

	printf("[");
	
	for(int i = 0; i <= n; i++) {
		if(visited[i]) {
			if(first) {
				first = false;
				printf("%hd", i);
			}
			else {
				printf(",%hd", i);
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

	//PrintSol(n, m);

	return 0;
}
