#include "cgi.h"
#include "util.h"

typedef void action_t(char**);

typedef struct {
	char *name;
	action_t *action;
} action_s;

void action_index(char **get_params) {
	extern char **environ;
	char **env;
	char *uname, *id, *date;

	uname = os_system("uname -a");
	id = os_system("id");
	date = os_system("date");

	printf("<h1>Hello, Equestria!</h1>\n");
	printf("<h2>This is a CGI script written in C.</h2>\n");
	printf("<p>Your IP address is %s</p>\n", getenv("REMOTE_ADDR"));
	printf("<p>Links: <a href='test.c?do=index'>Index</a> | <a href='test.c?do=test'>Test</a></p>\n");
	printf("<hr>\n");
	printf("<h3>System Info</h3>");
	printf("PID of this program: %d<br>", getpid());
	printf("`date`: %s<br>", date);
	printf("`uname -a`: %s<br>", uname);
	printf("`id`: %s<br>", id);

	printf("<h3>Environment Variables</h3>\n");
	printf("<pre>\n");
	for (env = environ; *env != NULL; env++) {
		printf("%s\n", *env);
	}
	printf("\n</pre>\n");

	free(uname);
	free(id);
	free(date);
}

void action_test(char **get_params) {
	//printf("<h1>Test</h1>");
	handle_view("views/time.html.ec");
}

const action_s valid_actions[] = {
	{ "index", action_index },
	{ "test", action_test },
	NULL
};

const int num_actions = 2;

int main(int argc, char *argv[]) {
	char *query_string;
	char **get_params;
	extern char **environ;
	char **env;
	bool action_found;

	/* Headings */
	printf("<!DOCTYPE html>\n");
	printf("<html>\n");
	printf("<head>\n");
	printf("<title>Welcome to a thing.</title>\n");
	printf("<meta charset=\"utf-8\">\n");
	printf("</head>\n");
	printf("<body>\n");
	/* End headings */

	/* Body */
	get_params = parse_query_string();

	action_found = false;
	while (*get_params) {
		char *k = *get_params;
		get_params++;
		char *v = *get_params;
		get_params++;

		if (strcmp(k, "do") == 0) { // ?do=someAction
			int i = 0;
			for (i = 0; i < num_actions; i++) { 
				action_s act = valid_actions[i];
				if (strcmp(act.name, v) == 0) {
					act.action(get_params);
					action_found = true;
				}
			}
		}
	}
	//free_query_string(get_params); // TODO: Fix this

	if (!action_found) {
		printf("<h1>Error: Invalid or empty action given.</h1>\n");
	}
	/* End body */

	/* Closing stuff */
	printf("</body>\n");
	printf("</html>\n");
	/* End closing stuff */

	return 0;
}
