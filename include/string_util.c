#include "gwan.h" // G-WAN exported functions

char* concatChar(char* toThis, char* addThis) {
	char* destination = (char*)malloc( strlen( addThis ) + strlen( toThis ) + 1 );
	strcpy( destination, toThis );
	strcat( destination, addThis );
	return destination;
}

static inline void own_u32toa(char *p, u32 v) {
   do *p-- = '0' + (v % 10), v /= 10; while(v);
}
