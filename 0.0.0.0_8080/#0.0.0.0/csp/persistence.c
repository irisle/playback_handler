// ============================================================================
// C servlet sample for the G-WAN Web Application Server (http://trustleap.ch/)
// ----------------------------------------------------------------------------
// persistence.c: using G-WAN's persistence pointers to attach data structures
//                that can be shared between G-WAN scripts, handlers, etc.
//
//            http://gcc.gnu.org/onlinedocs/gcc-4.1.1/gcc/Atomic-Builtins.html
// ============================================================================
#include "gwan.h" // G-WAN exported functions

#include <malloc.h>
#include <stdio.h>
// ----------------------------------------------------------------------------
// Server, Handler and VirtualHost persistence pointers (see gwan.h):
// US_SERVER_DATA,      // G-WAN-wide global pointer (for maintenance script)
// US_HANDLER_DATA=200, // Listener-wide pointer 
// US_VHOST_DATA,       // Virtual-Host-wide pointer
// ----------------------------------------------------------------------------
typedef struct 
{ 
   kv_t *kv;    // a Key-Value store
   char *blah; // a string
   u64   val;   // a counter
} data_t;
// ----------------------------------------------------------------------------
int main(int argc, char *argv[])
{
   // get the Handler persistent pointer to attach anything you need
 //data_t **data = (data_t**)get_env(argv, US_HANDLER_DATA);
 //data_t **data = (data_t**)get_env(argv, US_VHOST_DATA);
   data_t **data = (data_t**)get_env(argv, US_SERVER_DATA);

   xbuf_t *reply = get_reply(argv);
   xbuf_cat(reply, "<h2>Using G-WAN's persistence Pointers</h2>");

   if(!data[0]) // first time: persistent pointer is uninitialized
   {
      data[0] = (data_t*)calloc(1, sizeof(data_t));
      if(!data[0])
         return 500; // out of memory

      xbuf_cat(reply, "initialized data<br>");
   }
   
   // not thread-safe: you should either use an atomic operation
   // or 
   // a counter per G-WAN worker thread; use gettid() to find who's who)
   // data[0]->val++;
   
   // here is the atomic increment operation
   __sync_fetch_and_add(&data[0]->val, 1);
   
   xbuf_xcat(reply, "Value: %d", data[0]->val);
   return 200; // return an HTTP code (200:'OK')
}
// ============================================================================
// End of Source Code
// ============================================================================
