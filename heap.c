#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "dat.h"

// 静态函数只在定义文件中可见
static void  
set(Heap *h, int k, void *x) // 设置第k个元素为x
{
    h->data[k] = x;
    h->rec(x, k); // ?
}


static void
swap(Heap *h, int a, int b) // 交互第a个元素和第b个元素
{
    void *tmp;

    tmp = h->data[a];
    set(h, a, h->data[b]);
    set(h, b, tmp);
}


static int
less(Heap *h, int a, int b) // 返回第a个元素和第b个元素值小的 less 
{
    return h->less(h->data[a], h->data[b]);
}


static void
siftdown(Heap *h, int k) // 下沉
{
    for (;;) {
        int p = (k-1) / 2; /* parent */

        if (k == 0 || less(h, p, k)) {
            return;
        }

        swap(h, k, p);
        k = p;
    }
}


static void
siftup(Heap *h, int k) // 上升
{
    for (;;) {
        int l, r, s;

        l = k*2 + 1; /* left child */
        r = k*2 + 2; /* right child */

        /* find the smallest of the three */
        s = k;
        if (l < h->len && less(h, l, s)) s = l;
        if (r < h->len && less(h, r, s)) s = r;

        if (s == k) {
            return; /* satisfies the heap property */
        }

        swap(h, k, s);
        k = s;
    }
}


// Heapinsert inserts x into heap h according to h->less.
// It returns 1 on success, otherwise 0.
int
heapinsert(Heap *h, void *x)
{
    int k;

    if (h->len == h->cap) { // 长度等于容量，则扩容
        void **ndata;
        int ncap = (h->len+1) * 2; /* allocate twice what we need */

        ndata = malloc(sizeof(void*) * ncap);
        if (!ndata) {
            return 0;
        }

        memcpy(ndata, h->data, sizeof(void*)*h->len);
        free(h->data);
        h->data = ndata;
        h->cap = ncap;
    }

    k = h->len;
    h->len++;
    set(h, k, x);
    siftdown(h, k);
    return 1;
}


void *
heapremove(Heap *h, int k)
{
    void *x;

    if (k >= h->len) {
        return 0;
    }

    x = h->data[k];
    h->len--;
    set(h, k, h->data[h->len]);
    siftdown(h, k);
    siftup(h, k);
    h->rec(x, -1);
    return x;
}
