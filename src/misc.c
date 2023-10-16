#include "misc.h"

char strcmpr(const char* a, const char* b)
{
	int al = strlen(a);
	if(al != strlen(b)){return 0;}
	for(int i = 0; i < al; i++){
		if(a[i] != b[i]){return 0;}
	}
	return 1;
}