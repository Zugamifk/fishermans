typedef struct
t_event_bus
{
	event **subscriptions;
} event_bus;

event_bus *
bus_init()
{
	event_bus *eb = malloc(sizeof(event_bus));
	eb->subscriptions = calloc(EVENT_LIST_MAX, sizeof(event *) );
	return eb;
}

event_id
bus_newevent(event_bus *eb)
{
	event_id id = 0;
	for (; id<EVENT_LIST_MAX; id++ ){
		if (eb->subscriptions[id] == NULL) break;
	}
	if (id == EVENT_LIST_MAX) return EVENT_BUS_FULL;
	eb->subscriptions[id] = calloc(EVENT_SUBSCRIPTION_MAX, sizeof(event));
	return id;
}

void
bus_removeevent(event_bus *eb, event_id id)
{
	event *e = eb->subscriptions[id];
	if (e == NULL) return;
	free(e);
	e = NULL;
}

bus_error
bus_triggerevent(event_bus *eb, event_id id, event_descriptor des)
{
	event *e = eb->subscriptions[id];
	if (e == NULL) return EVENT_ID_INVALID;
	for (	event_subscription sub = 0;
			sub < EVENT_SUBSCRIPTION_MAX;
			sub++
		)
	{
		if (e[sub] == NULL) continue;
		e[sub](des);
	}	
	return EVENT_TASK_SUCCESS;
}

event_subscription
bus_subscribe(event_bus *eb, event_id id, event e)
{
	event_subscription sub = 0;
	if (eb->subscriptions[id] == NULL) return EVENT_ID_INVALID;
	for (; sub<EVENT_SUBSCRIPTION_MAX; sub++ ){
		if (eb->subscriptions[id][sub] == NULL) break;
	} 
	if (sub==EVENT_SUBSCRIPTION_MAX) return EVENT_SUBSCRIPTION_FULL;
	eb->subscriptions[id][sub] = e;
	return sub;
}

bus_error
bus_unsubscribe(event_bus *eb, event_id id, event_subscription sub)
{
	if (eb->subscriptions[id] == NULL) return EVENT_ID_INVALID;
	eb->subscriptions[id][sub] = NULL;
	return EVENT_TASK_SUCCESS;
}