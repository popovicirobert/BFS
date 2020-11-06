#include <stdio.h>
#include <string.h>
#include <bitset>

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


#define MAXN (1 << 14) // consideram ca numerele sunt pe short

static std::bitset<MAXN> edge[MAXN];

const int MAXM = (int) 5e5 + 5;

static short x[MAXM], y[MAXM];
int m;

static inline void ReadInput() {
	while(1) {
		x[++m] = GetNr();
		if(x[m] > -1) {
			y[m] = GetNr();
			edge[x[m]][y[m]] = 1;
		}
		else {
			break;
		}
	}		
}


static std::bitset<MAXN> notVisited, temp;
static short Q[MAXN];

static short smallX[MAXN], smallY[MAXN];
static short last[MAXN], nxt[MAXN];
short n = 0;


static inline void GetEdges() {
	notVisited.set();

	short l = 0, r = 1;
	Q[0] = 0;
	notVisited[0] = 0;

	n = 0;

	while(l < r) {
		short nod = Q[l];
		l++;

		temp = (edge[nod] & notVisited);
		short pos = temp._Find_first();

		while(pos != MAXN) {
			notVisited[pos] = 0;
			Q[r++] = pos;
			
			smallX[n] = nod;
			smallY[n] = pos;
			n++;

			pos = temp._Find_next(pos);
		}
	}
	
	for(short i = 0; i < MAXN; i++) {
		edge[i].reset();
	}
	for(short i = 0; i < n; i++) {
		edge[smallX[i]][smallY[i]] = 1;
	}
	short sz = n;
	for(int i = m; i >= 1; i--) {
		if(edge[x[i]][y[i]]) {
			nxt[sz] = last[x[i]];
			last[x[i]] = sz;

			smallY[sz] = y[i];
			sz--;
		}
	}
	n++;
}


static inline void Solve() {
	short l = 0, r = 1;

	Q[0] = 0;
	printf("[0");

	while(l < r) {
		short nod = Q[l];
		l++;
	
		short pos = last[nod];
		while(pos) {
			Q[r++] = smallY[pos];
			printf(",%d", smallY[pos]);
			if(r == n) {
				printf("]");
				return;
			}
			pos = nxt[pos];
		}
	}
}



int main() {

	Init();

	ReadInput();

	GetEdges();

	Solve();

	return 0;
}

