#include "gwan.h"    // G-WAN exported functions

#pragma link "mcrypt"
#include <mcrypt.h>

#include <openssl/bio.h>
#include <openssl/evp.h>

#include <math.h>

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <time.h>

#pragma link "curl"
#include <curl/curl.h>

#include <ctype.h>

#include <openssl/sha.h>

#include "open_ssl_AES.c"
// ----------------------------------------------------------------------------

char* concatChar(char* toThis, char* addThis) {
	char* destination = (char*)malloc( strlen( addThis ) + strlen( toThis ) + 1 );
	strcpy( destination, toThis );
	strcat( destination, addThis );
	return destination;
}

char *license = 0;
size_t writer(char *data, size_t size, size_t nmemb, xbuf_t *xbuf) {
	if(!xbuf)
		return 0;
	int len = size * nmemb;
	license = data;
	return len;
}

CURLcode http_get(char *url, xbuf_t *xbuf, char* params) {

	CURL *curl = curl_easy_init();

	if ( !curl ) {
		return 0;
	}

	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
	curl_easy_setopt(curl, CURLOPT_HTTP_TRANSFER_DECODING, 0);
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
	curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 50L);
	curl_easy_setopt(curl, CURLOPT_POST, 1L);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, params);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &xbuf);

	CURLcode result = curl_easy_perform(curl);
	curl_easy_cleanup(curl);
	return result;
}

int main(int argc, char *argv[]) {

	u32   method   = (u32)  get_env(argv, REQUEST_METHOD);

	// all the params values
	char* parameters = 0;
	char* tmp = 0;

	char *assetid = 0;
	get_arg("assetid=", &assetid, argc, argv);
	parameters = concatChar("assetid=", assetid);

	char *clientid = 0;
	get_arg("clientid=", &clientid, argc, argv);
	tmp = concatChar("&clientid=", clientid);
	parameters = concatChar(parameters, tmp);

	char *divInfo = 0;
	get_arg("divInfo=", &divInfo, argc, argv);

	tmp = concatChar("&divInfo=", divInfo);
	parameters = concatChar(parameters, tmp);

	char *keyid = 0;
	get_arg("keyid=", &keyid, argc, argv);
	tmp = concatChar("&keyid=", keyid);
	parameters = concatChar(parameters, tmp);

	char *md = 0;
	get_arg("md=", &md, argc, argv);
	tmp = concatChar("&md=", md);
	parameters = concatChar(parameters, tmp);

	char *mk = 0;
	get_arg("mk=", &mk, argc, argv);
	tmp = concatChar("&mk=", mk);
	parameters = concatChar(parameters, tmp);

	char *playbackmode = 0;
	get_arg("playbackmode=", &playbackmode, argc, argv);
	tmp = concatChar("&playbackmode=", playbackmode);
	parameters = concatChar(parameters, tmp);

	char *sessionid = 0;
	get_arg("sessionid=", &sessionid, argc, argv);
	tmp = concatChar("&sessionid=", sessionid);
	parameters = concatChar(parameters, tmp);

	char *userdata = 0;
	get_arg("userdata=", &userdata, argc, argv);
	tmp = concatChar("&userdata=", userdata);
	parameters = concatChar(parameters, tmp);

	char *ver = 0;
	get_arg("ver=", &ver, argc, argv);
	tmp = concatChar("&ver=", ver);
	parameters = concatChar(parameters, tmp);

	char *version = 0;
	get_arg("version=", &version, argc, argv);
	tmp = concatChar("&version=", version);
	parameters = concatChar(parameters, tmp);

	char *wvcss = 0;
	get_arg("wvcss=", &wvcss, argc, argv);
	if ( wvcss != NULL ) {
		tmp = concatChar("&wvcss=", wvcss);
		parameters = concatChar(parameters, tmp);
	}

	char *token = 0;
	get_arg("token=", &token, argc, argv);
	if ( token != NULL ) {
		tmp = concatChar("&token=", token);
		parameters = concatChar(parameters, tmp);
	}

	char *messageid = 0;
	get_arg("messageid=", &messageid, argc, argv);
	if ( messageid != NULL ) {
		tmp = concatChar("&messageid=", messageid);
		parameters = concatChar(parameters, tmp);
	}

	char *extra = 0;
	get_arg("extra=", &extra, argc, argv);
	if ( extra != NULL ) {
		tmp = concatChar("&extra=", extra);
		parameters = concatChar(parameters, tmp);
	}

	char *time_license = 0;
	get_arg("time=", &time_license, argc, argv);
	if ( time_license != NULL ) {
		tmp = concatChar("&time=", time_license);
		parameters = concatChar(parameters, tmp);
	}

	int try = 10;
	int count = 0;

	time_t actual_time;
	char *c_time_string[11];
	unsigned char *toSign;
	unsigned char *hash;
	char* encrypted_text;
	xbuf_t xbuf;

	int iv[] = {45, 23, 122, 103, 150, 13, 169, 80, 48, 215, 50, 102, 134, 209, 212, 202};// 2d177a67960da95030d7326686d1d4ca
	int key[] = {187, 138, 45, 2, 165, 165, 81, 34, 213, 218, 86, 129, 160, 76, 165, 205, // bb8a2d02a5a55122d5da5681a04ca5cd2d300ec33eb72437681f9d00ba4b9386
					45, 48, 14, 195, 62, 183, 36, 55, 104, 31, 157, 0, 186, 75, 147, 134};

	int iv_len = sizeof(iv)/sizeof(int);
	int key_len = sizeof(key)/sizeof(int);
	unsigned char iv_binary[iv_len];
	unsigned char key_binary[key_len];

	for (int i = 0; i < iv_len; i++) {
		iv_binary[i] = (unsigned char)(iv[i]);
	}
	for (int i = 0; i < key_len; i++) {
		key_binary[i] = (unsigned char)(key[i]);
	}

	// Sign process
	EVP_CIPHER_CTX en, de;
	aes_initialize_own(&en, &de, iv_binary, key_binary);

	// To try call "int try" times to obtain the license
	while ( count < try ) {

		// Actual time
		actual_time = time(NULL);

		// Actual time to string
		sprintf(c_time_string, "%u", ((unsigned)actual_time));

		char *assetid_tmp;
		char *client_tmp;
		char *mk_tmp;
		char *md_tmp;

		get_arg("assetid=", &assetid_tmp, argc, argv);
		get_arg("clientid=", &client_tmp, argc, argv);
		toSign = concatChar(assetid_tmp, client_tmp);
		get_arg("mk=", &mk_tmp, argc, argv);
		toSign = concatChar(toSign, mk_tmp);
		get_arg("md=", &md_tmp, argc, argv);
		toSign = concatChar(toSign, md_tmp);
		toSign = concatChar(toSign, c_time_string);
		printf("To Sign: %s\n", toSign);

		// Hashing
		hash = malloc(SHA_DIGEST_LENGTH);
		SHA1(toSign, strlen(toSign), hash);

		int len = (strlen(hash));
		encrypted_text = aes_encrypt(&en, hash, &len);

		// Calculate the sign size
		unsigned int input_size = sizeof(encrypted_text);
		unsigned int adjustment = ( (input_size % 3) ? (3 - (input_size % 3)) : 0);
		unsigned int code_padded_size = ( (input_size + adjustment) / 3) * 4;
		unsigned int newline_size = ((code_padded_size) / 72) * 2;
		unsigned int total_size = code_padded_size + newline_size;

		// To base 64
		char sign[total_size];
		s_snprintf(sign, 255, "%B", encrypted_text);
		printf("Base64 Sig: %s\n", sign);
		printf("Base64 len: %d size:\n", strlen(sign), sizeof(sign));

		char *duration_tmp;
		char *policy_tmp;
		char *usageurl_tmp;
		char *sign_tmp;
		char *time_tmp;
		char *internal_parameters;

		duration_tmp = "&setduration=3600";
		policy_tmp = "&setpolicy=mobile";
		internal_parameters = concatChar(duration_tmp, policy_tmp);

		char *usageurl = "http://development-lgtv.wuaki.tv/license_notification";
		// To base 64
		// TODO: Make here the same sign size calculation
		char usage_base64_url[72]; // aHR0cDovL2RldmVsb3BtZW50LWxndHYud3Vha2kudHYvbGljZW5zZV9ub3RpZmljYXRpb24=
		s_snprintf(usage_base64_url, 255, "%B", usageurl);

		usageurl_tmp = concatChar("&usageurl=", usage_base64_url);
		internal_parameters = concatChar(internal_parameters, usageurl_tmp);

		sign_tmp = concatChar("&sign=", sign);
		internal_parameters = concatChar(internal_parameters, sign_tmp);
		time_tmp = concatChar("&ptime=", c_time_string);
		internal_parameters = concatChar(internal_parameters, time_tmp);

		char *url = "https://fcpstage.shibboleth.tv/widevine/cypherpc/cgi-bin/GetEMMs.cgi";
		xbuf_t *reply = get_reply(argv);
		xbuf_init(&xbuf);

		char *concat_params = concatChar(parameters, internal_parameters);

		printf("Params: %s\n", concat_params);
		printf("Calling... %d %s\n", count, url);

		if (http_get(url, &xbuf, concat_params) != CURLE_OK ) {
			xbuf_cat(reply, "Server or resource not available");
			printf("ERROR: %s\n", xbuf);
		} else {
			// Empiric
			if ( license != NULL && strlen(license) < 20 ) {
				printf("INVALID License: %s\n", license);
			} else {
				printf("License: %s\n", license);
				break;
			}
		}

		count++;
	}

	xbuf_ncat(get_reply(argv), license, strlen(license));

	/*if ( assetid != NULL ) free(assetid);
	if ( clientid != NULL ) free(clientid);
	if ( divInfo != NULL ) free(divInfo);
	if ( keyid != NULL ) free(keyid);
	if ( md != NULL ) free(md);
	if ( mk != NULL ) free(mk);
	if ( playbackmode != NULL ) free(playbackmode);
	if ( sessionid != NULL ) free(sessionid);
	if ( userdata != NULL ) free(userdata);
	if ( ver != NULL ) free(ver);
	if ( version != NULL ) free(version);
	if ( token != NULL ) free(token);
	if ( wvcss != NULL ) free(wvcss);
	if ( messageid != NULL ) free(messageid);
	if ( extra != NULL ) free(extra);
	if ( time_license != NULL ) free(time_license);
	if ( license != NULL ) free(license);
	if ( actual_time != NULL ) free(actual_time);
	*/

	if ( encrypted_text != NULL ) free(encrypted_text);
	if ( hash != NULL ) free(hash);
	if ( toSign != NULL ) free(toSign);
	if ( c_time_string != NULL ) free(c_time_string);
	if ( parameters != NULL ) free(parameters);
	if ( tmp != NULL ) free(tmp);
	if ( &xbuf != NULL ) xbuf_free(&xbuf);

	return 200; // return an HTTP code (200:'OK')
}


//char *url = "https://staging.shibboleth.tv/widevine/cypherpc/cgi-bin/GetEMMs.cgi";

//int keysize = 32; /* 256 bits */
/*int buffer_len = 32;
   encrypted_text = calloc(1, buffer_len);
   memcpy(encrypted_text, hash, buffer_len);
   encrypt(encrypted_text, buffer_len, iv_binary, key_binary, keysize);*/


/*aes_t *ctx;
   int mode = 1;
   int keylen = 32;
   int len = 16;
   char *src;
   char *dst;
   printf("INIT...%d\n", len);
   aes_init(ctx, mode, key_binary, keylen);
   printf("ENCRYPT...\n");
   aes_enc(ctx, mode, len, iv_binary, toSign, dst);
   printf("DEST: %s\n", dst);*/

//int iv[] = {0x2d, 0x17, 0x7a, 0x67, 0x96, 0x0d, 0xa9, 0x50, 0x30, 0xd7, 0x32, 0x66, 0x86, 0xd1, 0xd4, 0xca};
/*int key[] = { 0xbb, 0x8a, 0x2d, 0x02, 0xa5, 0xa5, 0x51, 0x22, 0xd5, 0xda, 0x56, 0x81, 0xa0, 0x4c, 0xa5, 0xcd,
      		   	   	   0x2d, 0x30, 0x0e, 0xc3, 0x3e, 0xb7, 0x24, 0x37, 0x68, 0x1f, 0x9d, 0x00, 0xba, 0x4b, 0x93, 0x86 };*/


//assetid=809029467&clientid=|721299820&divInfo=SVDbOxATT9Z2nOyEyfTRQ7Rh%2BVsuO%2BakhH3zgfTcyCChhXZr7HIETHK1c5ffTf8IcnNPlN9GrNaDiMvhlNlT6A%3D%3D
//&keyid=0&md=CARD-35230:wuaki&mk=Broadcom&playbackmode=st&sessionid=|721299820:UN
//&userdata=ip:,streamid:,deviceid:|721299820,optdata:&ver=6&version=4.5.0.4134
//&token=AAAAAgAAABdyiGwdPy44WEIT49QbYf4NB0t1/Xo9m2EF6ZgbOtY4hOg7vRZeuwawpDvB3stR6kFm9nwAkbr/2HAqNAZLlx0b
//&sign=6IdQ8eRW8ubUtA2n1Pda0onsKW1DMVkd0dtvFkHJFMY=&ptime=1362139777

//0eE+nBA4sKexb1WhZ2HgDbAM6X4Xqg==

//assetid=809029467&clientid=|721299820&divInfo=n3aPz9Nucn2fCdzbPVU0+BUzec5gxdGgKb1Dz92hOX1SAGKvp1svDUnfqlJLcXfsI08PRkmgK3Fi/JiK0w2mSw==
//&keyid=0&md=CARD-35230:wuaki&mk=Broadcom&playbackmode=st&sessionid=|721299820:UN
//&token=AAAAAgAAABdyiGwdPy44WEIT49QbYf4NB0t1/Xo9m2EF6ZgbOtY4hOg7vRZeuwawpDvB3stR6kFm9nwAkbr/2HAqNAZLlx0b
//&userdata=ip:,streamid:,deviceid:|721299820,optdata:&ver=6&version=4.5.0.4134
//&sign=9gyToJvp1C98iu6/NwsUy/ZVmIo/llfQLZNGLK40UCc=&ptime=1362138787
// ============================================================================
// End of Source Code
// ============================================================================
