#include <stdio.h>
#include <string.h>

#define MAXBUF (1 << 17)
#define SIGMA 256

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
static int nxt[MAXM], last[MAXN];

int n, m;

static inline void ReadInput() {

	while(1) {
		++m;
		x[m] = GetNr();
		if(x[m] > -1) {
			y[m] = GetNr();
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

int sz = 0;

static inline void AddChar(const char ch) {
	buf[sz++] = ch;
	if(sz == MAXBUF) {
		fwrite(buf, 1, MAXBUF, stdout);
		sz = 0;
	}
}


static inline void Solve() {

	short l = 0, r = 1;
	Q[0] = 0;
	visited[0] = 1;

	AddChar('[');
	AddChar('0');
	
	int pos;

	while(l < r) {
		pos = last[Q[l]];
		++l;

		while(pos) {
			if(visited[y[pos]] == 0) {
				visited[y[pos]] = 1;
				Q[r] = y[pos];
				++r;

				AddChar(',');

				if(y[pos] == 0) {
					AddChar('0');
				}
				else {
					short cur = y[pos];
					for(short pw = 10000; pw >= 1; pw /= 10) {
						if(cur >= pw) {
							AddChar('0' + (cur / pw) % 10);
						}
					}
				}
			}
			pos = nxt[pos];
		}
	}
	AddChar(']');

	if(sz) {
		fwrite(buf, 1, sz, stdout);
	}
}



int main() {

	Init();

	ReadInput();

	Solve();

	return 0;
}

