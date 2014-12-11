#include "cgi.h"

#define STATE_IN_KEY 0
#define STATE_IN_VALUE 1

/* This function is mainly based on black magic.
   I don't even remember how it works.       */
char **parse_query_string(char *input) {
	char **get_params = NULL;
	int get_params_len = 0;
	char *query_string_raw = input;//getenv("QUERY_STRING");
	int state = STATE_IN_KEY;
	bool have_one_more = false;

	if (query_string_raw == NULL || strlen(query_string_raw) == 0) {
		get_params = (char**)calloc((sizeof(char*)), 1);
		get_params[0] = NULL;
		return get_params;
	}

	char *current_key;
	char *current_value;
	int current_key_counter = 0;
	int current_value_counter = 0;
	int current_key_size = 128;
	int current_value_size = 128;

	current_key = (char *)calloc(current_key_size, 1);
	current_value = (char *)calloc(current_value_size, 1);

	while (*query_string_raw) {
		char c = *query_string_raw;
		if (c == '&') {
			current_key[current_key_counter] = 0;
			current_value[current_value_counter] = 0;
			get_params_len += 2;
			get_params = (char **)realloc(get_params, ((sizeof(char*)) * (get_params_len + 2)));
			int ind_1 = get_params_len - 2;
			int ind_2 = (get_params_len) -1;
			get_params[ind_1] = current_key;
			get_params[ind_2] = current_value;

			state = STATE_IN_KEY;
			current_key_counter = 0;
			current_key_size = 128;
			current_key = (char *)calloc(current_key_size, 1);
			have_one_more = true;
		} else if (c == '=') {
			state = STATE_IN_VALUE;
			current_value_counter = 0;
			current_value_size = 128;
			current_value = (char *)calloc(current_value_size, 1);
		} else if (state == STATE_IN_KEY) { // In key
			check_var((void **)&current_key, current_key_counter, &current_key_size);
			*(current_key + current_key_counter) = c;
			current_key_counter++;
		} else if (state == STATE_IN_VALUE) { // In value
			check_var((void **)&current_value, current_value_counter, &current_value_size);
			*(current_value + current_value_counter) = c;
			current_value_counter++;
		}

		query_string_raw++;
	}
	get_params_len += 2;
	get_params = (char **)realloc(get_params, ((sizeof(char*)) * (get_params_len + 2)));
	int ind_1;
	int ind_2;

	if (have_one_more) {
		ind_1 = get_params_len - 2;
		ind_2 = get_params_len - 1;
	} else {
		ind_1 = 0;
		ind_2 = 1;	
	}

	get_params[ind_1] = current_key;
	get_params[ind_2] = current_value;
	get_params[get_params_len] = NULL;
	get_params[get_params_len + 1] = NULL;

	return get_params;
}

/* This doesn't work. */
void free_query_string(char **query_string) {
	char **ptr = query_string;

	while (*ptr != NULL) {
		free(*ptr);
		ptr++;
	}

	free(query_string);
}