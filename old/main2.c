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


#define MAXN (1 << 16) // consideram ca numerele sunt pe short
#define MAXM (int)(5e6 + 5)

static short x[MAXM], y[MAXM];
static int degree[MAXN];
static int edges[MAXM];

int n, m;

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
		if(degree[i] > degree[i - 1]) {
			n = i;
		}
	}
	for(int i = m; i >= 1; --i) {
		degree[x[i]]--;
		edges[degree[x[i]]] = i;
	}
}

static unsigned short distance[MAXN];
static short Q[MAXN];

static int index[MAXN];

void QSort(unsigned short l, unsigned short r) {
	int piv = index[(l + r) / 2];
	unsigned short beg = l, end = r;
	while(beg <= end) {
		while(index[beg] < piv) beg++;
		while(index[end] > piv) end--;
		if(beg <= end) {
			int aux = index[beg];
			index[beg] = index[end];
			index[end] = aux;
			++beg;
			--end;
		}
	}
	if(beg < r) QSort(beg, r);
	if(l < end) QSort(l, end);
}


//#include <assert.h>

static inline void GetEdges() {
	short l = 0, r = 1;
	Q[0] = 0;
	distance[0] = 1;

	int ord = 1;
	printf("[0");

	while(l < r) {
		short nod = Q[++l];

		int cur_ord = ord;

		#pragma omp parallel for reduction(+:ord)
		for(int i = degree[nod]; i < degree[nod + 1]; ++i) {
			if(!distance[y[edges[i]]]) {
				++ord;
				distance[y[edges[i]]] = ord;
			}
		}

		while(cur_ord < ord) {
			++cur_ord;
			#pragma omp parallel for
			for(int i = 1; i <= n; ++i) {
				if(distance[i] == cur_ord) {
					printf(",%hd", i);
					Q[++r] = i;
				}
			}
		}
	}
	printf("]");

		
	/*QSort(1, n);

	#pragma omp parallel for
	for(unsigned short i = 0; i <= n + 1; i++) {
		degree[i] = 0;
	}
	for(unsigned short i = 1; i <= n; i++) {
		if(index[i]) {
			degree[x[index[i]]]++;
		}
	}
	for(unsigned short i = 1; i <= n + 1; i++) {
		degree[i] += degree[i - 1];
	}
	for(unsigned short i = n; i >= 1; i--) {
		if(index[i]) {	
			degree[x[index[i]]]--;
			edges[degree[x[index[i]]]] = y[index[i]];
		}
	}*/
}


//static char visited[MAXN];

/*static inline void Solve() {
	short l = 0, r = 1;
	Q[0] = 0;
	visited[0] = 1;

	printf("[0");

	while(l < r) {
		short nod = Q[l];
		++l;

		for(int i = degree[nod]; i < degree[nod + 1]; ++i) {	
			if(!visited[edges[i]]) {
				visited[edges[i]] = 1;
				Q[r] = edges[i];
				printf(",%hd", Q[r]);
				++r;
			}
		}
	}

	printf("]");
	
}*/



int main() {

	Init();

	ReadInput();

	GetEdges();

	//Solve();

	return 0;
}

