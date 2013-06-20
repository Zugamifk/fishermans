typedef int tt_B;
typedef tt_B (*tt_F)(tt_B);
typedef tt_F (*tt_L)(tt_B);
typedef tt_B (*tt_E)(tt_F);
typedef tt_F (*tt_C)(tt_F);

const tt_B X = 0;

tt_B S( tt_B x ) { return x + 1; }

tt_F I( tt_X b) {

void
tt_test()
{
	tt_B x = S(S(S(S(X))));
	printf("%d\n", x);
}