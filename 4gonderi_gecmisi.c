#include "1musteri.c"

#define Max 25

int b = 0;
int count = 0;

typedef struct stack {
  int top;
  struct Kargo items[Max];
} st;

void createEmptyStack(st *s) { s->top = -1; }

void belirleme(struct Kargo *a) {
  b++;
  a->kargo_id = b;
}

// Check if the stack is full
int isfull(st *s) { return s->top == Max - 1; }

int isempty(st *s) { return s->top == -1; }

// Add elements into stack
void push(st *s, struct Kargo isim) {
  if (isfull(s)) {
    printf("Daha fazla kargo gönderemezsiniz.\n");
  } else {
    s->top++;
    s->items[s->top] = isim;
    belirleme(&s->items[s->top]);
  }
  count++;
}

// Remove element from stack
void pop(st *s) {
  if (isempty(s)) {
    printf("\nSTACK EMPTY\n");
  } else {
    printf("Item popped= %d\n", s->items[s->top].kargo_id);
    s->top--;
  }
  count--;
}

// Print elements of stack
void printStack(st *s) {
  printf("Stack: \n");
  printf("KARGO NUMARASI  TESLIM TARIHI  DURUMU  SURESI  ROTASI\n");
  printf("----------------------------------------------------\n");
  for (int i = 0; i <= s->top; i++) {
    printf("%d  %s  %s  %d  %s\n", s->items[i].kargo_id,
           s->items[i].kargo_teslim_tarihi, s->items[i].kargo_durum,
           s->items[i].kargo_suresi, s->items[i].rota.sehir_adi);
  }
  printf("\n");
}
