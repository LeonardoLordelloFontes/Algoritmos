#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define Max 100
#define Size 10
#define Free 0
#define Used 1
#define Del 2

typedef struct bucket {
    int status; // Free | Used | Del
    char *chave;
    int ocorr;
} THash2 [Size];

typedef struct pQueue {
    int valores [Max];
    int tamanho;
} PriorityQueue;

typedef struct nodo {
    char *chave;
    int ocorr;
    struct nodo *prox;
} Nodo, *THash [Size];

typedef struct bucket2 {
    int probC;
    int status; // Free | Used | Del
    char *chave;
    int ocorr;
} THash3 [Size];


// aux

void swap(int heap[], int pos1, int pos2) {
    int  aux = heap[pos1];
    heap[pos1] = heap[pos2];
    heap[pos2] = aux;
}

// Parte 1

// Ex 2

void bubbleUp (int i, int h[]) {
    int pai = (i-1)/2;
    while (i > 0 && h[i] < h[pai]) {
        swap(h, i, pai);
        i = pai;
        pai = (i-1)/2;
    }
}

// Ex 3

void bubbleDown (int i, int h[], int N) {
    int menor;
    while(2*i+1 < N) {
        menor = 2*i+1;
        if (2*i+2 < N && h[2*i+2] < h[menor]) {
            menor = 2*i+2;
        }
        if (h[menor] < h[i]) {
            swap(h, menor, i);
        }
        else {
            break;
        }
        i = menor;
    }
}

// Ex 4

void empty (PriorityQueue *q) {
    q->tamanho = 0;
}

int isEmpty (PriorityQueue *q) {
    return q->tamanho == 0;
}

int add1(int x, PriorityQueue *q) {
    int r = -1;
    if (q->tamanho != Max) {
        r = 0;
        q->tamanho++;
        q->valores[q->tamanho - 1] = x;
        bubbleUp(q->tamanho - 1, q->valores);
    }
    return r;
}

int remove1 (PriorityQueue *q, int *rem) {
    int r = -1;
    if (!isEmpty(q)) {
        r = 0;
        q->tamanho--;
        *rem = q->valores[0];
        swap(q->valores, 0, q->tamanho);
        bubbleDown(0, q->valores, q->tamanho);
    }
    return r;
}

// Ex 5

// top-down

void heapify (int v[], int N) {
    for (int i = 1; i < N; i++) {
        bubbleUp(i, v);
    }
}

// bottom-up

void heapify2 (int v[], int N) {
    for (int i = N/2 - 1; i >= 0; i--) {
        bubbleDown(i, v, N);
    }
}

// Ex 6

void ordenaHeap (int h[], int N) {
    for (int i = N-1; i > 0; i--) {
        swap(h, 0, i);
        bubbleDown(0, h, i-1);
    }
}

// Ex 7

PriorityQueue* k_maiores(int k, int N) {
    PriorityQueue *q = calloc(sizeof(PriorityQueue), 1);
    q->tamanho = k;
    for (int i = 0; i < N; i++) {
        int number;
        printf("Escreva o numero e digite enter ( 1 por vez ): ");
        scanf("%d", &number);
        if (number > q->valores[0]) {
            q->valores[0] = number;
            bubbleDown(0, q->valores, q->tamanho);
        }
    }
    return q;
}

// Parte 2.1

unsigned hash(char *str){
    unsigned hash = 5381;
    int c;
    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    return hash;
}

// Ex 1

void initEmpty (THash t) {
    for (int i = 0; i < Size; i++) {
        t[i] = calloc(sizeof(Nodo), 1);
    }
}

// Ex 2

void add2 (char *s, THash t) {
    unsigned pos = hash(s) % Size;
    Nodo *aux = t[pos], *last = aux;
    if (aux->chave != NULL) {
        for (; aux != NULL && strcmp(aux->chave, s) != 0; aux = aux->prox) {
            last = aux;
        }
        if (aux == NULL) {
            Nodo *new_node = calloc(sizeof(THash), 1);
            new_node->chave = strdup(s);
            new_node->prox = NULL;
            new_node->ocorr = 1;
            last->prox = new_node;
        }
        else {
            aux->ocorr++;
        }
    }
    else {
        t[pos]->chave = strdup(s);
        t[pos]->ocorr = 1;
        t[pos]->prox = NULL;
    }
}

// Ex 3

int lookup (char *s, THash t) {
    int r = 0;
    Nodo *aux;
    unsigned pos = hash(s) % Size;
    for (aux = t[pos]; aux != NULL && strcmp(s, aux->chave) != 0; aux = aux->prox);
    if (aux != NULL) r = aux->ocorr;
    return r;
}

// Ex 4

int remove2 (char *s, THash t) {
    int r = 0;
    unsigned pos = hash(s) % Size;
    Nodo *aux, *last;
    aux = t[pos];
    last = aux;
    for (aux = t[pos]; aux != NULL && strcmp(s, aux->chave) != 0; aux = aux->prox) {
        last = aux;
    }
    if (aux != NULL) {
        r = 1;
        aux->ocorr--;
        if (aux->ocorr == 0) {
            if (aux->prox == NULL) {
                last->prox = NULL;
                free(aux->chave);
                free(aux);
            }
            else if (aux == last) {
                last = t[pos];
                t[pos] = t[pos]->prox;
                free(last->chave);
                free(last);
            }
            else {
                last->prox = aux->prox;
                free(aux->chave);
                free(aux);
            }
        }
    }
    return r;
}

// Parte 2.2

// Ex 1

int where (char *s, THash2 t) {
    unsigned hash_n = hash(s), i, r = 0, del = 0;
    for (i = 0; t[(hash_n + i) % Size].status != Free && strcmp(s, t[(hash_n + i) % Size].chave) != 0 && i < Size; i++) {
        if (t[(hash_n + i) % Size].status == Del && !del) {
            del++;
            r = (hash_n + i) % Size;
        }
    }
    if (i == Size && del)  {
        r = -1;
    }
    else if (strcmp(s, t[(hash_n + i) % Size].chave) == 0 && t[(hash_n + i) % Size].status == Used){
        r = (hash_n + i) % Size;
    }
    return r;
}

// Ex 2

// a)

void initEmpty2(THash2 t) {
    for (int i = 0; i < Size; i++) {
        t[i].status = Free;
        t[i].ocorr = 0;
    }
}

// b)

void add3 (char *s, THash2 t) {
    int pos = where(s, t);
    if (t[pos].status == Used) {
        t[pos].ocorr++;
    }

    else {
        t[pos].ocorr = 1;
        t[pos].chave = strdup(s);
        t[pos].status = Used;
    }
}

// c)

int lookup2 (char *s, THash2 t) {
    int pos = where(s, t), r = 0;
    if (t[pos].status == Used) {
        r = t[pos].ocorr;
    }
    return r;
}

// d)

int remove3 (char *s, THash2 t) {
    int pos = where(s, t), r;
    if (t[pos].status == Used) {
        r = 1;
        if (t[pos].ocorr == 1) {
            t[pos].status = Del;
            free(t[pos].chave);
        }
        t[pos].ocorr--;
    }
    else {
        r = 0;
    }
    return r;
}

// Ex 3

int garb_collection (THash2 t) {
    int r = 0;
    for(int i = 0; i < Size; i++) {
        if (t[i].status == Del) {
            t[i].status = Free;
            r++;
        }
        else if (t[i].status == Used){
            int pos = where(t[i].chave, t);
            struct bucket *aux = &t[i];
            t[i] = t[pos];
            t[pos] = *aux;
        }
    }
    return r;
}

// Ex 4

int where2(char *s, THash3 t) {
    unsigned hash_n = hash(s), i, r = 0, del = 0;
    for (i = 0; t[(hash_n + i) % Size].status != Free && strcmp(s, t[(hash_n + i) % Size].chave) != 0 && i < Size; i++) {
        if (t[(hash_n + i) % Size].status == Used) t[(hash_n + i) % Size].probC++; // colisões aqui
        if (t[(hash_n + i) % Size].status == Del && !del) {
            del++;
            r = (hash_n + i) % Size;
        }
    }
    if (i == Size && del)  {
        r = -1;
    }
    else if (!del || strcmp(s, t[(hash_n + i) % Size].chave) == 0){
        r = (hash_n + i) % Size;
    }
    return r;
}

void add4 (char *s, THash3 t) {
    int pos = where2(s, t);
    if (t[pos].status == Used) {
        t[pos].ocorr++;
    }

    else {
        t[pos].ocorr = 1;
        t[pos].chave = strdup(s);
        t[pos].status = Used;
    }
}

int main () {
    return 0;
}
