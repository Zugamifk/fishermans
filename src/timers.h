clock_t timer[1<<4];
unsigned int clockcursor = 0;
char lasttime[1<<8];

void timer_fmt() {
	double ss = (double)(clock()/CLOCKS_PER_SEC);
	int mm = (int)ss/60;
	int hh = mm/60;
	int dd = hh/24;
	hh -= dd*24;
	mm -= hh*60;
	ss -= (double)(mm*60);
	sprintf(lasttime, "%d:%d:%d:%2.3f", dd, hh, mm, ss);
}

void tick() {
	timer[clockcursor]=clock();
	timer_fmt();
	printf("TICK(%d) : %s\n", clockcursor, lasttime);
	clockcursor++;
}

void tock() {
	clockcursor--;
	timer_fmt();
	printf("TOCK(%d) : %s : %.3fms\n", clockcursor, lasttime, (float)(clock()-timer[clockcursor])/CLOCKS_PER_SEC);
}