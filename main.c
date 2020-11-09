#include <stdio.h>
#include <string.h>

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


#define MAXN (1 << 15) // consideram ca numerele sunt pe short
#define MAXM (int)(5e6 + 5)

static short x[MAXM], y[MAXM];
static int degree[MAXN];
static short edges[MAXM];

int m;

static inline void ReadInput() {
	while(1) {
		++m;
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
	
	for(int i = 1; i < MAXN; ++i) {
		degree[i] += degree[i - 1];	
	}
	for(int i = m; i >= 1; --i) {
		--degree[x[i]];
		edges[degree[x[i]]] = y[i];
	}
}


static char visited[MAXN];
static short Q[MAXN];

static inline void Solve() {

	short l = 0, r = 1;
	Q[0] = 0;
	visited[0] = 1;

	printf("[0");

	while(l < r) {
		int posl = degree[Q[l]];
		int posr = degree[Q[l] + 1];
		++l;
	
		for(int i = posl; i < posr; i++) {
			if(!visited[edges[i]]) {
				visited[edges[i]] = 1;
				Q[r] = edges[i];
				printf(",%hd", Q[r]);
				++r;
			}
		}
		
	}
	printf("]");
}



int main() {

	Init();

	ReadInput();

	Solve();

	return 0;
}


