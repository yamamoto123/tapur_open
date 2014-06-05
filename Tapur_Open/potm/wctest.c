
#include <stdio.h>
#include <unistd.h>	// sleep()
#include <ctype.h>	// toupper()
#include "../include/wave_convert.h"

// wave_convert.c
extern int Adpcm_Wave(char *adpcm_filename, char *wave_filename);
extern int Wave_Adpcm(char *wave_filename, char *adpcm_filename);

char *touppers(char *s)
{
	char *p = s;
	while(*s) {
		*s = toupper(*s);
		s++;
	}
	return p;
}

int main(int argc, char *argv[])
{
	char *s;

	if (argc != 4) {
		printf("USAGE:wctest <WA or AW> <in_file> <out_file>\n");
		exit(1);
	}

	s = touppers(argv[1]);
	if (strcmp(s,"WA") == 0) {
		if(Wave_Adpcm(argv[2],argv[3])) {
			printf("error Wave_Adpcm(%s,%s)\n",argv[2],argv[3]);
		}
	} else if (strcmp(s,"AW") == 0) {
		if(Adpcm_Wave(argv[2],argv[3])) {
			printf("error Adpcm_Wave(%s,%s)\n",argv[2],argv[3]);
		}
	}

	exit(1);
}
