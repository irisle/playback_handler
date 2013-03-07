#include "gwan.h" // G-WAN exported functions

int main(int argc, char *argv[]) {

	char * req = (char*) get_env(argv, QUERY_STRING);
    printf("%s \n", req);
    // To base 64
	char response[1];
	s_snprintf(response, 255, "%B", "0");

	xbuf_ncat(get_reply(argv), response, sizeof(response) - 1);

	return 200; // return an HTTP code (200:'OK')
}
