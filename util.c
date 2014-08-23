#include "util.h"

char *os_system(char *cmd) {
	char *out = (char*)calloc(513, 1);
	int max = 512;
	FILE *fp = popen(cmd, "r");
	char c;
	int counter = 0;
	
	do {
		c = fgetc(fp);
		
		if (counter > max) {
			max *= 2;
			out = (char*)realloc(out, max);
		}
		*(out + counter) = c;
		counter++;
	} while (c != EOF);
	*(out + counter-1) = 0;
	return out;
}

/* I am dumb and I accidentally wrote this but
   I'm sure it will be useful sometime.     */
char *scan_val(char *kv) {
	bool in_value = false;
	char *value = (char*)calloc(1, strlen(kv)+1);
	int counter = 0;

	while (*kv) {
		if (in_value) {
			*(value + counter) = *kv;
			counter++;
		} else if (*kv == '=') {
			in_value = true;
		}
		kv++;
	}
	*(value + counter) = 0;
	return value;
}

/* Please don't complain about my lack of NULL retval checking. */
void check_var(void **ptr, int cur_size, int *max) {
	if (cur_size > (*max)) {
		(*max) *= 2;
		*ptr = realloc(*ptr, *max);
	}
}