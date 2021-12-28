/* Converts a RAW icon image file (48x32) into a XPM char array (for inclusion in
   a source file) 

   (c)2005 Damian Parrino

*/

#include <stdio.h>

void convert(char *ifn, char *ofn, char *prefix) {
	FILE *i, *o;
	char x;
	int j;

	i = fopen(ifn, "rb");
	o = fopen(ofn, "w");
	if (!i || !o) {
		printf("error: can't open input or output file\n");
		return;
	}
	fprintf(o, "/* XPM */\n");
	fprintf(o, "static char *%s_xpm =\n", prefix);
	j=0;
	while((x=getc(i)) != EOF) {
		if (j == 0) putc('"', o);
		if (x == 0) {
			putc('.', o);
		} else {
			putc('+', o);
		}
		j++;
		if (j == 48) {
			fprintf(o, "\"\n");
			j=0;
		}
	}
	fprintf(o, ";\n");
	fclose(i);
	fclose(o);
}

int main(int argc, char **argv) {
	char *prefix;

	argc--;
	if (argc != 2 && argc != 3) {
		printf("usage: bin2c <input> <output> [prefix]\n");
		return 0;
	}
	prefix = (argc == 3) ? argv[3] : "vmu_icon";
	convert(argv[1], argv[2], prefix);
	return 0;
}
