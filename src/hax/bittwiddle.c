void
bittwiddle_test()
{
	int a, b, c;
	a = 10;
	b = -10;
	c = 256;
	
	printf("VALS:\ta = %d,\tb = %d,\tc = %d\n", a, b, c);
	printf("SIGN:\ta = %d,\tb = %d\n", BT_SIGN(a), BT_SIGN(b));
	printf("OPPSIGN:\ta, b : %d,\ta, a : %d\n", BT_OPPSIGN(a, b),BT_OPPSIGN(a, a));
	printf("ABSMASK:\t a : %d\n", _BT_ABSMASK(a));
	printf("ABS:\t a : %d,\tb : %d\n", BT_ABS(a), BT_ABS(b));
	printf("MIN: a, b : %d\n", BT_MIN(a, b));
	printf("MAX: a, b : %d\n", BT_MAX(a, b));
	printf("POWER OF 2?: a : %d, c : %d\n", BT_POW2(a), BT_POW2(c));
}