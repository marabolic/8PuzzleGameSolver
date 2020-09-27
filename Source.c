#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif // _MSC_VER

#include <stdio.h>
#include <stdlib.h>
#define DIM 3
#define QMAX 360000

int root[DIM][DIM], leaf[DIM][DIM];

typedef struct node {
	int a[DIM][DIM];
	int flag;
	int prev_i;
	int prev_j;
	struct node *down;
	struct node *right;
} Node;

Node Q[QMAX];
Node *p = NULL, *r = NULL, *s = NULL, *t = NULL,*q = NULL;
//implementacija reda
void insertQ(Node Q[], Node *x, int n, int *rear, int *front) {
	*rear = (*rear + 1) % n;
	if (*front == *rear)
		printf("ne moze se dodati element\n");
	else Q[*rear] = *x;
	if (*front == -1) 
		*front = 0;
}
Node *deleteQ(Node Q[], int cnt, int *front, int *rear) {
	Node *x = (Node*)malloc(sizeof(Node));
	if (*front == -1) 
		printf("ne moze se obrisati element\n");
	else {
		*x = Q[*front];
		if (*front == *rear) 
			*front = *rear = -1;
		else 
			*front = *front % cnt + 1;
	}
	return x;
}
_Bool QueueIsEmpty(Node Q[], int front, int rear) {
	if (front == -1) return 1;
	else return 0;
}
//kraj impl reda

void rootMat(int a[DIM][DIM]) {
	printf("unesi koren\n");
	for (int i = 0; i < DIM; i++) 
		for (int j = 0; j < DIM; j++) 
			scanf("%d", &a[i][j]);
}

void leafMat(int a[DIM][DIM]) {
	printf("unesi cilj\n");
	for (int i = 0; i < DIM; i++) 
		for (int j = 0; j < DIM; j++) 
			scanf("%d", &a[i][j]);
}

void connect(Node *temp, Node *next,  int val) {
	temp->right = next; 
	temp->flag = val;
}

_Bool isEqual(Node *q, int leaf[DIM][DIM]) {
	_Bool flag = 1;
	for (int i = 0; i < DIM; i++) {
		for (int j = 0; j < DIM; j++)
			if (q->a[i][j] != leaf[i][j]) {
				flag = 0;
				break;
			}
	}
	return flag;
}

void exchange(int *a, int *b) {
	int p = *a;
	*a = *b; 
	*b = p;
}

int randArr(int i) {
	double k = ((double)rand() / (RAND_MAX + 1.0)) * i;
	k = (int)k; 
	return k;
}

void randFY(int a[DIM]) {
	for (int i = DIM * DIM - 1; i > 0; i--) {
		int r = randArr(i); 
		exchange(&a[i], &a[r]);
	}
}

void arrToMat(int mat[DIM][DIM]) {
	int a[DIM*DIM], k = 0;
	for (int i = 0; i < DIM*DIM; i++) 
		a[i] = i;
	randFY(a);
	for (int i = 0; i < DIM; i++)
		for (int j = 0; j < DIM; j++) { 
			mat[i][j] = a[k]; 
			k++; 
		}
}

_Bool accessibleTrue(int root[DIM][DIM], int leaf[DIM][DIM]) {
	int l, k, m, n;
	int num = 0;
	int temp[DIM][DIM];
	for (int i = 0; i < DIM; i++)
		for (int j = 0; j < DIM; j++) temp[i][j] = leaf[i][j];

	for (int i = 0; i < DIM; i++) {
		for (int j = 0; j < DIM; j++)
		{
			if (root[i][j] == 0) {
				l = i;
				k = j;
			}
			if (temp[i][j] == 0) {
				m = i;
				n = j;
			}
		}
	}
	while (root[l][k] != temp[l][k]) {
		if (l > m) {
			exchange(&temp[m][n], &temp[m + 1][n]);
			m++;
		}
		else if (l < m) {
			exchange(&temp[m][n], &temp[m - 1][n]);
			m--;
		}
		if (k > n) {
			exchange(&temp[m][n], &temp[m][n + 1]);
			n++;
		}
		else if (k < n) {
			exchange(&temp[m][n], &temp[m][n - 1]);
			n--;
		}
	}
	
	for (int i = 0; i < DIM; i++) {
		for (int j = 0; j < DIM; j++)
			if (root[i][j] != temp[i][j])
				for (int p = 0; p < DIM; p++)
					for (int q = 0; q < DIM; q++)
						if (temp[p][q] == root[i][j]) {
							exchange(&temp[i][j], &temp[p][q]);
							num++;
						}
	}
					
	if (num % 2 == 0) return 1;
	else return 0;
}

void copyMat(Node *p, Node *q) {
	for (int i = 0; i < DIM; i++)
		for (int j = 0; j < DIM; j++) p->a[i][j] = q->a[i][j];
}

void printMat(Node *q) {
	static int t = 0;
	printf("%d. ", t); t++;
	for (int i = 0; i < DIM; i++) 
		for (int j = 0; j < DIM; j++) printf("%d ", q->a[i][j]);
}

Node *makeNode(Node *p, Node *q, int l, int k, int u, int v) {
	p = malloc(sizeof(Node));
	copyMat(p, q);
	exchange(&p->a[l][k], &p->a[u][v]); 
	p->prev_i = l; p->prev_j = k;
	printMat(p); 
	printf("\n");
	return p;
}

void printFather(Node *r, Node *l) {
	if (accessibleTrue(root, leaf)) {
		while (l != r) {
			if (l->flag == 1) 
				printMat(l->right);
			l = l->right;
		}
	}
}

void game(Node *q, int *prev_i, int *prev_j, Node Q[QMAX], int n, int *rear, int *front) {
	int l, k;
	for (int i = 0; i < DIM; i++) 
		for (int j = 0; j < DIM; j++)
			if (q->a[i][j] == 0) {
				l = i; 
				k = j; 
			}

	
	*prev_i = q->prev_i;
	*prev_j = q->prev_j;

	if (l == 0 && k == 0) {
		if (l + 1 != *prev_i || k != *prev_j) {
			p = makeNode(p, q, l, k, l + 1, k);
			insertQ(Q, p, n, rear, front);
		}
		if (l != *prev_i || k + 1 != *prev_j) {
			r = makeNode(r, q, l, k, l , k+1);
			insertQ(Q, r, n, rear, front);
		}
	}
	else if (l == 0 && k == 1) {
		if (l + 1 != *prev_i || k != *prev_j) {
			p = makeNode(p, q, l, k, l + 1 , k);
			insertQ(Q, p, n, rear, front);
		}
		if (l != *prev_i || k + 1 != *prev_j) {
			r = makeNode(r, q, l, k, l, k+1);
			insertQ(Q, r, n, rear, front);
		}
		if (l != *prev_i || k - 1!= *prev_j) {
			s = makeNode(s, q, l, k, l, k-1);
			insertQ(Q, s, n, rear, front);
		}
	}
	else if (l == 0 && k == 2) {
		if (l + 1 != *prev_i || k != *prev_j) {
			p = makeNode(p, q, l, k, l+1, k);
			insertQ(Q, p, n, rear, front);
		}
		if (l != *prev_i || k - 1!= *prev_j) {
			r = makeNode(r, q, l, k, l, k-1);
			insertQ(Q, r, n, rear, front);
		}
	}
	else if (l == 1 && k == 0) {
		if (l != *prev_i || k + 1 != *prev_j) {
			p = makeNode(p, q, l, k, l, k+1);
			insertQ(Q, p, n, rear, front);
		}
		if (l - 1 != *prev_i || k != *prev_j) {
			r = makeNode(r, q, l, k, l-1 ,k);
			insertQ(Q, r, n, rear, front);
		}
		if (l + 1 != *prev_i || k != *prev_j) {
			s = makeNode(s, q, l, k, l+1, k);
			insertQ(Q, s, n, rear, front);
		}
	}
	else if (l == 1 && k == 1) {
		if (l != *prev_i || k + 1!= *prev_j) {
			p = makeNode(p, q, l, k,l, k+1);
			insertQ(Q, p, n, rear, front);
		}
		if (l != *prev_i || k - 1!= *prev_j) {
			r = makeNode(r, q, l, k, l,k-1);
			insertQ(Q, r, n, rear, front);
		}
		if (l + 1 != *prev_i || k != *prev_j) {
			s = makeNode(s, q, l, k,l+1, k);
			insertQ(Q, s, n, rear, front);
		}
		if (l - 1 != *prev_i || k != *prev_j) {
			t = makeNode(t, q, l, k, l-1, k);
			insertQ(Q, t, n, rear, front);
		}
	}
	else if (l == 1 && k == 2) {
		if (l  != *prev_i || k - 1!= *prev_j) {
			p = makeNode(p, q, l, k, l, k-1);
			insertQ(Q, p, n, rear, front);
		}
		if (l - 1 != *prev_i || k != *prev_j) {
			r = makeNode(r, q, l, k, l-1, k);
			insertQ(Q, r, n, rear, front);
		}
		if (l + 1 != *prev_i || k != *prev_j) {
			s = makeNode(s, q, l, k, l+1, k);
			insertQ(Q, s, n, rear, front);
		}
	}
	else if (l == 2 && k == 0) {
		if (l != *prev_i || k + 1!= *prev_j) {
			p = makeNode(p, q, l, k, l, k+1);
			insertQ(Q, p, n, rear, front);
		}
		if (l - 1 != *prev_i || k != *prev_j) {
			r = makeNode(r, q, l, k, l-1, k);
			insertQ(Q, r, n, rear, front);
		}
	}
	else if (l == 2 && k == 1) {
		if (l != *prev_i || k + 1!= *prev_j) {
			p = makeNode(p, q, l, k, l, k+1);
			insertQ(Q, p, n, rear, front);
		}
		if (l != *prev_i || k - 1!= *prev_j) {
			r = makeNode(r, q, l, k, l, k-1);
			insertQ(Q, r, n, rear, front);
		}
		if (l - 1 != *prev_i || k != *prev_j) {
			s = makeNode(s, q, l, k, l-1, k);
			insertQ(Q, s, n, rear, front);
		}
	}
	else if (l == 2 && k == 2) {
		if (l != *prev_i || k - 1!= *prev_j) {
			p = makeNode(p, q, l, k, l, k-1);
			insertQ(Q, p, n, rear, front);
		}
		if (l - 1 != *prev_i || k != *prev_j) {
			r = makeNode(r, q, l, k, l-1, k);
			insertQ(Q, r, n, rear, front);
		}
	}
	
	if (t != NULL) {
		if (s != NULL) {
			connect(t, s, 0);
			if (r != NULL) {
				connect(s, r, 0);
				if (p != NULL) {
					connect(r, p, 0);
					connect(p, q, 1);
				}
				else connect(r, q, 1);
			}
			else {
				if (p != NULL) {
					connect(s, p, 0);
					connect(p, q, 1);
				}
				else connect(s, q, 1);
			}
		}
		else {
			if (r != NULL) {
				connect(t, r, 0);
				if (p != NULL) {
					connect(r, p, 0);
					connect(p, q, 1);
				}
				else connect(r, q, 1);
			}
			else {
				if (p != NULL) {
					connect(t, p, 0);
					connect(p, q, 1);
				}
				else connect(t, q, 1);
			}
		}
		q->down = t;
	}
	else {
		if (s != NULL) {
			if (r != NULL) {
				connect(s, r, 0);
				if (p != NULL) {
					connect(r, p, 0);
					connect(p, q, 1);
				}
				else {
					connect(r, q, 1);
				}
			}
			else {
				if (p != NULL) {
					connect(s, p, 0);
					connect(p, q, 1);
				}
				else connect(s, q, 1); 
			}
			q->down = s;
		}
		else {
			if (r != NULL) {
				if (p != NULL) {
					connect(r, p, 0);
					connect(p, q, 1);
				}
				else connect(r, q, 1);
				q->down = r;
			}
			else {
				if (p != NULL) {
					connect(p, q, 1);
					q->down = p;
				}
			}
		}
	}

}

Node *readRoot(int a[DIM][DIM], int val) {
	Node *root = (Node*)malloc(sizeof(Node));
	if (val) rootMat(a); else arrToMat(a);
	root->down = NULL; 
	root->right = NULL; 
	root->flag = 0; 
	root->prev_i = -1; 
	root->prev_j = -1;
	for (int i = 0; i < DIM; i++) 
		for (int j = 0; j < DIM; j++) 
			root->a[i][j] = a[i][j];
	return root;
}

Node *read(Node *root, int leaf[DIM][DIM]) {
	int rear = - 1, front = -1, n, prev_i = -1, prev_j = -1;
	
	scanf("%d", &n);
	insertQ(Q, root, n, &rear, &front);
	while (!QueueIsEmpty(Q,front, rear)) {
		q = deleteQ(Q, n, &front, &rear);
		if (isEqual(q, leaf)) {
			printf("kraj\n");
			printMat(q);
			printf("\n");
			break;
		}
		else {
			game(q, &prev_i, &prev_j, Q, n, &rear, &front);
			printf("%d\n", q->down->flag);
		}
	}
	return q;
}

void menu() {
	printf("1. unos pocetne i krajnje pozicije \n");
	printf("2. random pocetna i krajnja pozicija \n");
	printf("3. provera\n");
	printf("4. simulacija igre\n");
}

int main() {
	int w, cmd;
	Node *r = NULL, *l = NULL;
	menu();
	printf("unesite komandu: "); 
	scanf("%d", &cmd);
	while (cmd >= 1 && cmd <= 4) {
		switch (cmd) {
		case 1:  r = readRoot(root, 1); leafMat(leaf); break;
		case 2:  r = readRoot(root, 0); arrToMat(leaf); break;
		case 3:  if (accessibleTrue(root, leaf))  printf("moze\n");  
				 else printf("ne moze\n"); break;
		case 4:  if (accessibleTrue(root, leaf)) 
					l = read(r, leaf); break;
		}
		printf("unesite komandu: "); scanf("%d", &cmd);
	}
	scanf("%d", &w);
	return 0;
}