int
load(const char *filename, char **data, int *len)
{
	// open file
	FILE *fp = fopen(filename, "r");
	if (!fp) {
		printf("\'%s\' couldn't be opened!\n", filename);
		return 0;
	}
	fseek(fp, 0, SEEK_END);
	*len = ftell(fp);
	rewind(fp);
	*data = calloc(*len, sizeof(char)); 
	
	int succ = fread(*data, *len, 1, fp);
	
	(*data)[*len-1]='\0';

	fclose(fp);

	return succ;

}

void
save(const char *filename, char *data, int len)
{
	// open file
	FILE *fp = fopen(filename, "w");
	if (!fp) {
		printf("\'%s\' couldn't be opened!!\n", filename);
		return;
	}

	fwrite(data, 1, len, fp);

	fclose(fp);
}