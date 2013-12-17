fontcharset *
fontcharset_init()
{
	fontcharset *fcs = malloc(sizeof(fontcharset));
	fcs->set = malloc(sizeof(fontchar*)*128);
	for(int i =0;i<128;i++) {
		fcs->set[i] = NULL;
	}
	fcs->complete = 0;
	return fcs;
}

fontcharset *
fontcharset_loadfont(const char *filename)
{

	// open file
	FILE *fp = fopen(filename, "r");
	if (!fp) {
		printf("\'%s\' doesn't exist!\n", filename);
		return NULL;
	}

	fontcharset *fcs = fontcharset_init();

	double spacing;
	fscanf(fp, "%lf\n", &spacing);
	fcs->spacing = spacing;

	char index;
	double width;
	double x, y;
	int p1, p2;
	fscanf(fp, "%c %lf\n", &index, &width);

	int addedchars = 0;
	for(int i=0;i<128; i++) {

		if (i==index)
		{
			fcs->set[i] = fontchar_init();
			fcs->set[i]->width = width;

			vec2 **pts = malloc(sizeof(vec2*)*CHARSET_MAXPTS);
			int ptct = 0;
			vec2 **vi = pts;
			fscanf(fp, "----\n");
			while(fscanf(fp, "%lf\t%lf\n", &x, &y)==2) {
				*vi = vec2_new(x, y);
				vi++;
				ptct++;
			}
			fcs->set[i]->pts = pts;
			fcs->set[i]->ptct = ptct;

			fscanf(fp, "----\n");
			vec2 ***edges = malloc(sizeof(vec2**)*CHARSET_MAXEDGES);
			int edgect = 0;
			vec2 ***ei = edges;
			while(fscanf(fp, "%d %d\n", &p1, &p2)==2) {
				*ei = malloc(sizeof(vec2*)*2);
				(*ei)[0] = fcs->set[i]->pts[p1];
				(*ei)[1] = fcs->set[i]->pts[p2];
				ei++;
				edgect++;
			}
			fcs->set[i]->edges = edges;
			fcs->set[i]->edgect = edgect;

		//	fontchar_tostring(fcs->set[i]);

			fscanf(fp, "----\n");
			fscanf(fp, " %c %lf\n", &index, &width);

			addedchars++;
		}
	}

	fclose(fp);

	if (addedchars == 128) {
		fcs->complete = 1;
	}
	return fcs;
}

fontchar *
fontcharset_getchar(fontcharset *fcs, char *c)
{
		return fcs->set[(int)(*c)];
}
