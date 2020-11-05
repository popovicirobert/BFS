#include <stdio.h>

#define MAXBUF (1 << 18) // am pus 2 ^ 18
#define SIGMA 256

static char buf[MAXBUF];
int pbuf;

static char notDigit[SIGMA];
static char isDigit[SIGMA];

static inline void Init() {
	pbuf = MAXBUF;
	
	for(int ch = 0; ch < SIGMA; ch++) {
		notDigit[ch] = 1;
	}
	notDigit[']'] = 0;
	for(int ch = '0'; ch <= '9'; ch++) {
		isDigit[ch] = 1;
		notDigit[ch] = 0;
	}	
}

static inline const char NextCh() {
	if(pbuf == MAXBUF) {
		fread(buf, 1, MAXBUF, stdin);
		pbuf = 0;
	}
	return buf[pbuf++];
}

static inline const short GetNr() { // am inlocuit isdigit cu isDigit si notDigit
	char ch = NextCh();
	while(notDigit[ch]) {
		ch = NextCh();
	}

	if(isDigit[ch]) {
		short ans = 0;

		do {
			ans = ans * 10 + ch - '0';
			ch = NextCh();
		} while(isDigit[ch]);

		return ans;
	}

	return -1;
}


#define MAXN (1 << 15) // consideram ca numerele sunt pe short
#define MAXM (int)(5e6 + 5)

static short x[MAXM], y[MAXM];
static int nxt[MAXM], last[MAXN];

int n, m;

static inline void ReadInput() {

	while(1) {
		++m;
		x[m] = GetNr();
		if(x[m] > -1) {
			y[m] = GetNr();
			if(n < x[m]) n = x[m]; // daca nu folosim n putem sa scoatem if-urile
			if(n < y[m]) n = y[m];
		}
		else {
			break;
		}
	}	
	m--;
	
	for(int i = m; i >= 1; --i) {
		nxt[i] = last[x[i]];
		last[x[i]] = i;
	}
}


static char visited[MAXN];
static short Q[MAXN];


static inline void Solve() {

	short l = 0, r = 1;
	Q[0] = 0;
	visited[0] = 1;

	printf("[0");
	
	int pos;

	while(l < r) {
		pos = last[Q[l]];
		++l;

		while(pos) {
			if(visited[y[pos]] == 0) {
				visited[y[pos]] = 1;
				Q[r] = y[pos];
				++r;

				printf(",%hd", y[pos]);

				if(r == n + 1) { // s-ar putea sa mareasca timpul ????
					printf("]");
					return ;
				}
			}
			pos = nxt[pos];
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

