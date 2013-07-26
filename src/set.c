
#define SETMAXSIZE 1<<8
typedef void SETDATA;
typedef void (*set_printcb)(SETDATA*);

typedef struct
_S_set
{
	SETDATA** items;
	int size;
} set;

set*
set_init()
{
	set *nset = malloc(sizeof(set));
	nset->items = malloc(sizeof(SETDATA*)*SETMAXSIZE);
	nset->size = 0;
	return nset;
}

void
set_free
(
	set* s
)
{
	free(s->items);
	free(s);
}

bool
set_has
(
	set* s,
	SETDATA* d
)
{
	for(int i = 0; i < s->size; i++) {
		if (d == s->items[i]) {
			return true;
		}
	}
	return false;
}

void
set_add
(
	set* s,
	SETDATA* d
)
{
	if (set_has(s, d)) return;
	s->items[s->size] = d;
	s->size = s->size + 1;
}

void
set_remove
(
	set* s,
	SETDATA* d
)
{
	bool shifting = false;
	for(int i = 0; i < s->size; i++) {
		if (d == s->items[i]) {
			shifting = true;
		}
		if(shifting) {
			s->items[i] = s->items[i+1];
		}
	}
	s->size = s->size - 1;
}

void
set_union
(
	set* result,
	set* a,
	set* b
)
{
	for (int i = 0; i < a->size; i++) {
		set_add(result, a->items[i]);
	}
	
	for (int i = 0; i < b->size; i++) {
		set_add(result, b->items[i]);
	}
}

void
set_intersection
(
	set* result,
	set* a,
	set* b
)
{
	for (int i = 0; i < a->size; i++) {
		if (set_has(b, a->items[i])) 
			set_add(result, a->items[i]);
	}
}

void
set_difference
(
	set* result,
	set* a,
	set* b
)
{
	for (int i = 0; i < a->size; i++) {
		if (!set_has(b, a->items[i]))
			set_add(result, a->items[i]);
	}
}

void
set_print
(
	set* s,
	set_printcb cb
)
{
	printf("( ");
	for(int i = 0; i < s->size; i++) {
		cb(s->items[i]);
		printf(", ");
	}
	printf(" )\n");
}

void
set_testcb(SETDATA* d)
{
	printf("%d", d);
}

void
set_test()
{
	printf("SET S: ");
	set* s = set_init();
	set_add(s, (SETDATA*)1);
	set_add(s, (SETDATA*)2);
	set_add(s, (SETDATA*)3);
	set_add(s, (SETDATA*)4);
	set_add(s, (SETDATA*)5);
	set_add(s, (SETDATA*)6);
	set_print(s, set_testcb);

	printf("S \\ 2, 6: ");
	set_remove(s, (SETDATA*)6);
	set_remove(s, (SETDATA*)2);
	set_print(s, set_testcb);
	
	printf("SET T: ");
	set* t = set_init();
	set_add(t, (SETDATA*)1);
	set_add(t, (SETDATA*)2);
	set_add(t, (SETDATA*)9);
	set_add(t, (SETDATA*)4);
	set_print(t, set_testcb);
	
	printf("UNION OF S, T: ");
	set* u = set_init();
	set_union(u, s, t);
	set_print(u, set_testcb);
	
	printf("INTERSECTION OF S, T: ");
	set* i = set_init();
	set_intersection(i, s, t);
	set_print(i, set_testcb);
	
	printf("DIFFERENCE OF S, T: ");
	set* d = set_init();
	set_difference(d, s, t);
	set_print(d, set_testcb);
	
	set_free(s);
	set_free(t);
	set_free(u);
	set_free(i);
	set_free(d);
}