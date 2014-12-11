#include "viewhandler.h"
typedef int ptrdiff_t;
char *replace_str(const char *str, const char *old, const char *new)
{
	char *ret, *r;
	const char *p, *q;
	size_t oldlen = strlen(old);
	size_t count, retlen, newlen = strlen(new);

	if (oldlen != newlen) {
		for (count = 0, p = str; (q = strstr(p, old)) != NULL; p = q + oldlen)
			count++;
		/* this is undefined if p - str > PTRDIFF_MAX */
		retlen = p - str + strlen(p) + count * (newlen - oldlen);
	} else
		retlen = strlen(str);

	if ((ret = malloc(retlen + 1)) == NULL)
		return NULL;

	for (r = ret, p = str; (q = strstr(p, old)) != NULL; p = q + oldlen) {
		/* this is undefined if q - p > PTRDIFF_MAX */
		ptrdiff_t l = q - p;
		memcpy(r, p, l);
		r += l;
		memcpy(r, new, newlen);
		r += newlen;
	}
	strcpy(r, p);

	return ret;
}

void handle_view(char *view_path) {
	FILE *fp;
	int i;
	int state = V_STATE_NORMAL;
	int file_size;
	char *file_contents;
	char *code_buf;
	int code_len_cur = 0;
	int code_len_max;
	char *block_buf;
	int block_len_cur = 0;
	int block_len_max;
	bool actually_did_something = false;
	const char *ender = "\");\n";
	const char *beginner = "\nprintf(\"";
	bool did_something_in_tag = false;

	fp = fopen(view_path, "r");
	fseek(fp, 0, SEEK_END);
	file_size = ftell(fp);
	rewind(fp);

	file_contents = (char*)calloc(file_size + 1, 1);
	fread(file_contents, 1, file_size, fp);
	code_buf = (char*)calloc(file_size + 1, 1);
	code_len_max = file_size;
	block_buf = (char*)calloc(file_size + 1, 1);
	block_len_max = file_size;
	//strcpy(block_buf, "printf(\"");
	code_len_cur = strlen(code_buf);
	i = 0;
	while (i < file_size) {
		char c = *(file_contents + i);
		char next = 0;
		if (i+1 < file_size) {
			next = *(file_contents + i+1);
		}

		if (c == '<' && next == '%' && state != V_STATE_INTAG)  { // Opening a tag
			state = V_STATE_INTAG;
			i += 2;
			
			if (actually_did_something) {
				block_len_cur += strlen(ender);
				check_var(&block_buf, block_len_cur, &block_len_max);
				strcat(block_buf, ender);
				code_len_cur += strlen(block_buf) + strlen(beginner);
				check_var(&code_buf, code_len_cur, &code_len_max);
				strcat(code_buf, beginner);
				strcat(code_buf, block_buf);
				block_len_cur = 0;
				block_len_max = file_size;
				block_buf = (char*)calloc(file_size + 1, 1);
			}
			actually_did_something = false;
			
			
			
			continue;
		} else if (c == '%' && next == '>' && state == V_STATE_INTAG) { // We be closing a tag
			if (did_something_in_tag && 0) {
				code_len_cur += strlen(beginner);
				check_var(&code_buf, code_len_cur, &code_len_max);
				strcat(code_buf, beginner);
			}
			did_something_in_tag = false;

			state = V_STATE_NORMAL;
			i += 2;
			continue;
		}

		if (state == V_STATE_INTAG) {
			check_var(&code_buf, code_len_cur, &code_len_max);
			*(code_buf + code_len_cur) = c;
			code_len_cur++;
			did_something_in_tag = true;
		} else if (state == V_STATE_NORMAL) {
			check_var(&block_buf, block_len_cur, &block_len_max);
			if (c == '"') {
				*(block_buf + block_len_cur) = '\\';
				block_len_cur++;
				*(block_buf + block_len_cur) = c;
				block_len_cur++;
			} else if (c == '\n') {
				*(block_buf + block_len_cur) = '"';
				block_len_cur++;
				*(block_buf + block_len_cur) = '\n';
				block_len_cur++;
				*(block_buf + block_len_cur) = '"';
				block_len_cur++;
			} else {
				*(block_buf + block_len_cur) = c;
				block_len_cur++;
			}
			actually_did_something = true;
		}

		i++;
	}
	//if (did_something_in_tag) {
		//code_len_cur += strlen(ender);
		//check_var(&code_buf, code_len_cur, &code_len_max);
		//strcat(code_buf, ender);
	//}
	//printf("Dongers raisedd.\n");
	//printf("%s", code_buf);
	FILE *tmp = fopen("./view-tmp.c", "w");
	//printf("%p\n", tmp);
	char *new = replace_str(code_buf, "printf(\"\");", "");
	fputs(new, tmp);
	free(code_buf);
	free(new);
	//fflush(tmp);
	fclose(tmp);
	//system("sed -Ei 's/printf\\(""\\);//g' view-tmp.c");
	//pclose(sed);
	FILE *p = popen("gcc -o view-tmp view-tmp.c", "r");
	pclose(p);
	FILE *p2 = popen("./view-tmp", "r");
	char c;
	do {
		c = fgetc(p2);
		if (c != EOF) {
			putchar(c);
		}
	} while (c != EOF);
	unlink("view-tmp.c");
	unlink("view-tmp");	
}