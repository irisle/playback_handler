#include "gwan.h" // G-WAN exported functions
#include "string_util.c"

int main(int argc, char *argv[]) {

	char *assetid = 0;
	get_arg("assetid=", &assetid, argc, argv);
	int iassetid = atoi(assetid);
	printf("Size %d, %d ", sizeof(iassetid), iassetid);

	char *nonce = 0;
	get_arg("nonce=", &nonce, argc, argv);
	int inonce = atoi(nonce);
	printf("Size nonce %d, %d \n", sizeof(inonce), inonce);

	char *hb_status = "0";
	char *response = 0;
	response = concatChar(hb_status, assetid);
	response = concatChar(response, nonce);
	printf("Heartbeat response: %s\n", response);
	printf("Size Heartbeat response %d \n", sizeof(response));
	printf("VALUE Heartbeat response %d \n", atoi(response));
	printf("SIZE VALUE Heartbeat response %d \n", sizeof((long int)atoi(response)));

	// To base 64
	char r[strlen(response) + 1];
	printf("R: %d\n", sizeof(r));
	s_snprintf(r, 255, "%B", response);
	printf("Base64 Heartbeat response: %s\n", r);

	xbuf_t *reply = get_reply(argv);

	// -------------------------------------------------------------------------
	// get the current HTTP date (like "Sun, 06 Nov 1994 08:49:37 GMT")
	// -------------------------------------------------------------------------
	char *p = (char*)get_env(argv, SERVER_DATE);

	// -------------------------------------------------------------------------
	// create your custom response headers and append our HTML
	// -------------------------------------------------------------------------
	int position = 0;
	xbuf_xcat(reply,
			"HTTP/1.1 200 OK\r\n"
			"Date: %s\r\n"
			"Last-Modified: %s\r\n"
			"Encoding: base64\r\n"
			"Content-type: text/plain; charset=ISO-8859-1\r\n"
			"Length: 12\r\n"
			"Content-Length:      %n\r\n" // make room for the HTML length
			"Connection: close\r\n\r\n",
			p, p,
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
	own_u32toa(p, reply->len - headers_size);

	return 200; // return an HTTP code (200:'OK')


}
