#ifndef __UTIL_H_INCLUDED
#define __UTIL_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef char bool;
#define true 1
#define false 0

char *os_system(char *cmd);
char *scan_val(char *kv);
void check_var(void **ptr, int cur_size, int *max);

#endif