int
load(const char *filename, char **data, int *len)
{
	// open file
	FILE *fp = fopen(filename, "r");
	if (!fp) {
		printf("\'%s\' doesn't exist!\n", filename);
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