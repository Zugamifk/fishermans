typedef void (*list_deletedatacb)(void *);
typedef struct
list 
{
	struct list *next;
	void *data;
} list;

list *
list_new() {
	list *l = malloc(sizeof(list));
	l->next = NULL;
	l->data = NULL;
	return l;
}

void
list_delete(list *l) {
	if (l->next) {
		list_delete(l->next);
	}
	free(l);
}

void
list_deepdelete(list *l, void (*freecb)(void *)) {
	if (l->next) {
		list_delete(l->next);
	}
	freecb(l->data);
	free(l);
}

list *
list_end(list *l) 
{
	while(l->next) {
		l = l->next;
	}
	return l;
}

int
list_add(list *l, void *d) 
{
	int ind = 0;
	while(l->next) {
		l = l->next;
		ind++;
	}
	l->next = list_new();
	l->data = d;
	return ind;
}

list *
list_get(list *l, int i)
{
	while(i>0 && l->next != NULL) {
		l = l->next;
		i--;
	}
	return l;
}

int
list_size(list *l)
{
	int ct = 0;
	while(l->data!=NULL) {
		l=l->next;
		ct++;
	}
	return ct;
}

void
list_deleteend(list *l) 
{
	list *tl;
	while(l->data!=NULL) {
		tl = l;
		l = l->next;
	}
	free(l);
	free(tl->data);
	tl->data = NULL;
	tl->next = NULL;
}
