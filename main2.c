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
static short degree[MAXN];
static short edges[MAXM];

int m;

static inline void ReadInput() {
	while(1) {
		++m;
		x[m] = GetNr();
		if(x[m] > -1) {
			y[m] = GetNr();
			degree[y[m]]++;
		}
		else {
			break;
		}
	}	
	m--;
	
	for(int i = 1; i < MAXN; ++i) {
		degree[i] += degree[i - 1];	
		if(degree[i] > degree[i - 1]) {
			n = i;
		}
	}
	for(int i = m; i >= 1; --i) {
		--degree[y[i]];
		edges[degree[y[i]]] = x[i];
	}
}

unsigned long long visited[1 << 22];
static unsigned short distance[MAXN];
static short from[MAXN];
static short Q[MAXN];

static inline void Solve() {

	short l = 0, r = 1;
	unsigned short cur_dist = 1, count = 1;
	distance[0] = 1;

	while(count) {
		count = 0;

		#pragma omp parallel for reduction(+:count)
		for(unsigned short nod = 0; nod <= n; nod++) {
			if(distance[nod] == 0) {
				for(int i = degree[nod]; i < degree[nod + 1]; i++) {
					if(distance[edges[i]] == cur_dist) {
						distance[nod] = cur_dist + 1;

						

						count++;
						break;
					}
				}
			}
		}

		cur_dist++;	
	}
}



int main() {

	Init();

	ReadInput();

	Solve();

	return 0;
}

