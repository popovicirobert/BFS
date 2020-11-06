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


const int MAXN = (1 << 15); // consideram ca numerele sunt pe short

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
static int last[MAXM], nxt[MAXM];
int n = 0;


static inline void GetEdges() {
	notVisited.set();

	int l = 0, r = 1;
	Q[0] = 0;
	notVisited[0] = 0;

	n = 0;

	while(l < r) {
		short nod = Q[l];
		l++;
	
		temp = (edge[nod] & notVisited);
		int pos = temp._Find_first();

		while(pos != MAXN) {
			notVisited[pos] = 0;
			Q[r++] = pos;
			
			smallX[n] = nod;
			smallY[n] = pos;
			n++;

			pos = temp._Find_next(pos);
		}
	}
	
	for(int i = 0; i < MAXN; ++i) {
		edge[i].reset();
	}
	for(int i = 0; i < n; ++i) {
		edge[smallX[i]][smallY[i]] = 1;
	}
	/*int sz = n;
	for(int i = m; i >= 1; --i) {
		if(edge[x[i]][y[i]]) {
			edge[x[i]][y[i]] = 0;

			nxt[sz] = last[x[i]];
			last[x[i]] = sz;

			smallY[sz] = y[i];
			sz--;
		}
	}
	n++;*/

	for(int i = m; i >= 1; --i) {
		nxt[i] = last[x[i]];
		last[x[i]] = i;
	}
}


/*static inline void Solve() {
	int l = 0, r = 1;

	Q[0] = 0;
	printf("[0");

	while(l < r) {
		short nod = Q[l];
		l++;
	
		int pos = last[nod];
		while(pos) {
			Q[r++] = smallY[pos];
			printf(",%hd", smallY[pos]);
			if(r == n) {
				printf("]");
				return;
			}
			pos = nxt[pos];
		}
	}
}*/

static char visited[MAXN];
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

	GetEdges();

	Solve();

	return 0;
}

