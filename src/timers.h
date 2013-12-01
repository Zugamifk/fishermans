clock_t timer[1<<4];
unsigned int clockcursor = 0;

void tick() {
	timer[clockcursor]=clock();
	printf("TICK(%d) : %d\n", clockcursor, timer[clockcursor]);
	clockcursor++;
}

void tock() {
	clockcursor--;
	printf("TOCK(%d) : %d : %fms\n", clockcursor, clock(), (float)(clock()-timer[clockcursor])/CLOCKS_PER_SEC);
}