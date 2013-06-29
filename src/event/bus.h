#define EVENT_BUS

#define	EVENT_LIST_MAX	1<<0xf
#define EVENT_SUBSCRIPTION_MAX	1<<0xf

typedef void *event_descriptor;
typedef void (*event)(event_descriptor);
typedef int event_id;
#define EVENT_ID_NULL -1
typedef int	event_subscription;

typedef int bus_error;

#define EVENT_BUS_FULL -1
#define EVENT_SUBSCRIPTION_FULL -2
#define EVENT_ID_INVALID -3
#define EVENT_TASK_SUCCESS 0

#ifdef HASHTABLE
#define EVENT_BUS_HASDICT
typedef struct
t_event_bus
{
	event **subscriptions;
	hashtable* eventdictionary;
} event_bus;
#else
typedef struct
t_event_bus
{
	event **subscriptions;
} event_bus;
#endif //HASHTABLE


#include "bus.c"