#include "gwan.h" // G-WAN exported functions
#include <stdlib.h>
#include "string_util.c"

int main(int argc, char *argv[]) {

	printf("%s \n", "******************** GOT PLAYBACK? ********************");

	char *user = 0;
	get_arg("user=", &user, argc, argv);
	u32 *pHTTP_status;
	pHTTP_status = get_env(argv, HTTP_CODE);
	if ( user != NULL ) {
		printf("User %s \n", user);
		kv_t **vhost_ptr = (kv_t**)get_env(argv, US_VHOST_DATA), //persistent ptr
		       *heartbeat = 0; //convenience pointer (var->m instead of (*var)->m)
		if ( vhost_ptr && *vhost_ptr ) {
			heartbeat = *vhost_ptr;
			char *milliseconds = kv_get(heartbeat, user, sizeof(user) - 1);
			printf("Milliseconds C: %s\n", milliseconds);
		}
	}

	return 204; // return an HTTP code (204:'OK - empty body')

}
