#ifndef __CGI_H_INCLUDED
#define __CGI_H_INCLUDED

#include "util.h"

char **parse_query_string(char *input);
void free_query_string(char **query_string);

#endif