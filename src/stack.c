#define STACK_MAXCAP (1<<8)
#define STACK_DATA void
typedef struct
_S_stack
{
	STACK_DATA** data;
	STACK_DATA** sp;
	int capacity;
} stack;

stack*
stack_init
(
	int max
)
{
	stack* res = malloc(sizeof(stack));
	
	if (max == 0) max = STACK_MAXCAP;
	res->data = malloc(sizeof(STACK_DATA*)*max);
	res->sp = res->data;
	res->capacity = max;
	
	return res;
}

void
stack_free
(
	stack* s
)
{
	free(s->data);
	free(s);
}

void
stack_push
(
	stack*	s
,	STACK_DATA*	data
)
{
	if (s->sp - s->data < s->capacity) {
		*(s->sp) = data;
		s->sp = s->sp + 1;
	}
}

STACK_DATA*
stack_pop
(
	stack*	s
)
{
	if (s->sp == s->data) {
		return NULL;
	} else {
		s->sp = s->sp - 1;
		return *(s->sp);
	}
}

STACK_DATA*
stack_peek
(
	stack*	s
)
{
	if (s->sp == s->data) {
		return NULL;
	} else {
		return *(s->sp-1);
	}
}

bool
stack_isempty
(
	stack* s
)
{
	return s->sp == s->data;
}

bool
stack_isfull
(
	stack* s
)
{
	return s->sp - s->data >= s->capacity;
}