// ============================================================================
// Handler C script for the G-WAN Web Application Server (http://trustleap.ch/)
// ----------------------------------------------------------------------------
// main.c: bypass HTTP parsing
// ============================================================================
//#pragma debug   // uncomment to get symbols/line numbers in crash reports

#include "gwan.h" // G-WAN exported functions
// ----------------------------------------------------------------------------
// init() will initialize your data structures, load your files, etc.
// ----------------------------------------------------------------------------
// init() should return -1 if failure (to allocate memory for example)
int init(int argc, char *argv[])
{
   // define which handler states we want to be notified in main():
   // enum HANDLER_ACT { 
   //  HDL_INIT = 0, 
   //  HDL_AFTER_ACCEPT, // just after accept (only client IP address setup)
   //  HDL_AFTER_READ,   // each time a read was done until HTTP request OK
   //  HDL_BEFORE_PARSE, // HTTP verb/URI validated but HTTP headers are not 
   //  HDL_AFTER_PARSE,  // HTTP headers validated, ready to build reply
   //  HDL_BEFORE_WRITE, // after a reply was built, but before it is sent
   //  HDL_HTTP_ERRORS,  // when G-WAN is going to reply with an HTTP error
   //  HDL_CLEANUP };
   u32 *states = (u32*)get_env(argv, US_HANDLER_STATES);
   *states = 1 << HDL_AFTER_READ; // we assume "GET /hello" sent in one shot
   return 0;
}
// ----------------------------------------------------------------------------
// clean() will free any allocated memory and possibly log summarized stats
// ----------------------------------------------------------------------------
void clean(int argc, char *argv[]) {}
// ----------------------------------------------------------------------------
// main() does the job for all the connection states below:
// (see 'HTTP_Env' in gwan.h for all the values you can fetch with get_env())
// ----------------------------------------------------------------------------
int main(int argc, char *argv[]) {
   const long state = (long)argv[0];
   if(state == HDL_AFTER_READ) {
      xbuf_t *read_xbuf = (xbuf_t*)get_env(argv, READ_XBUF);
      xbuf_replfrto(read_xbuf, read_xbuf->ptr, read_xbuf->ptr + 16, "/license", "/?license.c");
      xbuf_replfrto(read_xbuf, read_xbuf->ptr, read_xbuf->ptr + 16, "/license_ack", "/?license_ack.c");
      xbuf_replfrto(read_xbuf, read_xbuf->ptr, read_xbuf->ptr + 16, "/heartbeat", "/?heartbeat.c");
      // Note: you may have to look for the ending double-CRLF to check
      //       if there are other pipelined requests to rewrite
   }
   return 255; // execute next connection step
}
// ============================================================================
// End of Source Code
// ============================================================================
