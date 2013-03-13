#include "gwan.h" // G-WAN exported functions
#include <stdlib.h>
#include "string_util.c"

typedef struct record_s { // this is a user-defined 'record' structure
	char *user;
	char *mililseconds;
	char *when;
} record_t;

int main(int argc, char *argv[]) {

	printf("%s \n", "******************** HEARTBEAT ********************");

	char *seconds = NULL;
	get_arg("seconds=", &seconds, argc, argv);
	if ( seconds != NULL ) {
		printf("Seconds %s \n", seconds);
	}
	char *platform = NULL;
	get_arg("platform=", &platform, argc, argv);
	if ( platform != NULL ) {
		printf("Platform %s \n", platform);
	}
	char *api_version = NULL;
	get_arg("api_version=", &api_version, argc, argv);
	if ( api_version != NULL ) {
		printf("Api Version %s \n", api_version);
	}
	char *user = NULL;
	get_arg("user=", &user, argc, argv);
	if ( user != NULL ) {
		printf("User %s \n", user);
	}

	kv_t **vhost_ptr = (kv_t**)get_env(argv, US_VHOST_DATA); // persistent ptr
	if ( vhost_ptr && !*vhost_ptr ) {
		// initialize Key-Value store
		*vhost_ptr = (kv_t*)malloc(sizeof(kv_t));
		kv_init(*vhost_ptr, "HEARTBEAT", 1000000, 0, 0, 0);
		printf("%s \n", "******************** KV STORE INITIALIZE ********************");
	}

	kv_item item;
	item.key = user;
	item.klen = sizeof(user) - 1;
	item.val = seconds;
	item.flags = 0;
	kv_add(*vhost_ptr, &item);

	if ( seconds != NULL ) free(seconds);
	if ( platform != NULL ) free(platform);
	if ( api_version != NULL ) free(api_version);

	static char msg[] = "0";
	xbuf_ncat(get_reply(argv), msg, sizeof(msg) - 1);

	return 200; // return an HTTP code (200:'OK')

}

void widevine(int argc, char *argv[]) {
	/*char *assetid = 0;
	//get_arg("assetid=", &assetid, argc, argv);
	if ( assetid != NULL ) {
		printf("Assetit %s \n", assetid);
	}
	printf("%s \n", "******************** 0 ********************");
	char *nonce = 0;
	//get_arg("nonce=", &nonce, argc, argv);
	if ( nonce != NULL ) {
		printf("Nonce %s \n", assetid);
	}
	printf("%s \n", "******************** 1 ********************");
	char *seconds = 0;
	//get_arg("seconds=", &seconds, argc, argv);
	if ( seconds != NULL ) {
		printf("Seconds %s \n", seconds);
	}
	printf("%s \n", "******************** 2 ********************");
	char *platform = 0;
	//get_arg("platform=", &platform, argc, argv);
	if ( platform != NULL ) {
		printf("Platform %s \n", platform);
	}
	printf("%s \n", "******************** 3 ********************");
	char *api_version = 0;
	//get_arg("api_version=", &api_version, argc, argv);
	if ( api_version != NULL ) {
		printf("Api Version %s \n", api_version);
	}
	printf("%s \n", "******************** 4 ********************");
	char *hb_status = 0;
    char *response = 0;

	// -------------------------------------------------------------------------
	// get the current HTTP date (like "Sun, 06 Nov 1994 08:49:37 GMT")
	// -------------------------------------------------------------------------
	char *p = (char*)get_env(argv, SERVER_DATE);

	if ( assetid != NULL && nonce != NULL) {
		printf("%s \n", "******************** WIDEVINE HEARTBEAT ********************");
		xbuf_t *reply = get_reply(argv);
	 *hb_status =  "0";
		response = concatChar(hb_status, assetid);
		response = concatChar(response, nonce);
		printf("Heartbeat response: %s\n", response);

		// To base 64
		//char r[strlen(response) + 1];
		char r[strlen(response)];
		s_snprintf(r, 255, "%B", response);
		printf("Base64 Heartbeat response: %s, %d, %d\n", r, sizeof(response), strlen(response));

		// -------------------------------------------------------------------------
		// create your custom response headers and append our HTML
		// -------------------------------------------------------------------------
		int position = 0;
		xbuf_xcat(reply,
				"HTTP/1.1 200 OK\r\n"
				"Date: %s\r\n"
				"Last-Modified: %s\r\n"
				"Encoding: base64\r\n"
				"Content-Type: text/plain; charset=UTF-8\r\n"
				"Length: %d\r\n"
				"Content-Length:      %n\r\n" // make room for the HTML length
				"Connection: keep-alive\r\n\r\n",
				//p, p, sizeof(r),
				p, p, 12,
				&position);  // "Sun, 06 Nov 2009 08:49:37 GMT"
		int headers_size = reply->len;

		// -------------------------------------------------------------------------
		// save the place where to patch the empty 'Content-Length' HTTP Header
		// -------------------------------------------------------------------------
		p = (char*)(reply->ptr + position);

		// -------------------------------------------------------------------------
		// append your HTML page to the reply buffer
		// -------------------------------------------------------------------------
		xbuf_xcat(reply, r);

		// -------------------------------------------------------------------------
		// store the HTML size in the empty space of the 'Content-Length' header
		// -------------------------------------------------------------------------
		//own_u32toa(p, reply->len - headers_size);
		own_u32toa(p, 12);

	} else {

		printf("%s \n", "******************** 5 ********************");
		static char msg[] = "0";
		xbuf_ncat(get_reply(argv), msg, sizeof(msg) -1);

	}

	printf("%s \n", "******************** 6 ********************");
	if ( assetid != NULL ) free(assetid);
	if ( nonce != NULL ) free(nonce);
	if ( seconds != NULL ) free(seconds);
	if ( platform != NULL ) free(platform);
	if ( api_version != NULL ) free(api_version);
	if ( p != NULL ) free(p);
	if ( hb_status != NULL ) free(hb_status);
	if ( response != NULL ) free(response);
	printf("%s \n", "******************** 7 ********************");*/
}
