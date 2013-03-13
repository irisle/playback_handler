#include "gwan.h" // G-WAN exported functions

int main(int argc, char *argv[]) {

	printf("%s \n", "******************** LICENSE NOTIFICATION ********************");
	static char msg[] = "0";
    xbuf_ncat(get_reply(argv), msg, sizeof(msg) - 1);
	return 200; // return an HTTP code (200:'OK')

}
