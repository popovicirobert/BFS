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

static inline const unsigned short GetNr() {
	char ch = NextCh();
	while(!isdigit(ch) && ch != ']') {
		ch = NextCh();
	}

	if(isdigit(ch)) {
		unsigned short ans = 0;

		do {
			ans = ans * 10 + ch - '0';
			ch = NextCh();
		} while(isdigit(ch));

		return ans;
	}

	return -1;
}



#define MAXN (1 << 16)
#define MAXM (int)(5e6 + 5)

static unsigned short x[MAXM], y[MAXM];
static int nxt[MAXM], last[MAXN];

static inline const unsigned short max(unsigned short a,unsigned short b) {
	if(a < b) return b;
	return a;
}


static inline void ReadInput(int* n, int* m) {
	unsigned short a, b;

	*n = *m = 0;
	a = 0;

	while(a < (unsigned short)-1) {
		a = GetNr();
		if(a < (unsigned short)-1) {
			b = GetNr();

			#ifdef HOME
				assert(b < (unsigned short)-1);	
			#endif

			++(*m);	
			x[*m] = a;
			y[*m] = b;
			
			*n = max(*n, max(a, b));
		}
	}	
	
	for(int i = *m; i >= 1; --i) {
		nxt[i] = last[x[i]];
		last[x[i]] = i;
	}
}


static char visited[MAXN];
static unsigned short Q[MAXN];


static inline void Solve(int n, int m) {

	unsigned short l = 0, r = 1;
	Q[0] = 0;
	visited[0] = 1;

	printf("[0");
	
	int pos;

	while(l < r && r <= n) {
		pos = last[Q[l]];
		++l;

		while(pos) {
			if(visited[y[pos]] == 0) {
				visited[y[pos]] = 1;
				Q[r] = y[pos];
				++r;

				printf(",%hd", y[pos]);
			}
			pos = nxt[pos];
		}
	}
	printf("]");
}



int main() {

	Init();

	int n, m;
	ReadInput(&n, &m);

	Solve(n, m);

	return 0;
}

