#include "err.h"
#include "onpcalc.h"
#include <string.h>
#include <unistd.h>
#include <stdio.h>

const int LINE_SIZE = 1024;
/*
 * Czyta z stdin string zakonczony \n badz \0
 * Do bufora trafia tenze string z koncowym nullem
 */
void readline(char * buf){
	char b;
	int i = 0;
	int r;
	while ((r = read(0, &b, 1)) == 1){
		if (b=='\n')
			b = '\0';
		buf[i] = b;
		i++;
		if (b=='\0')
			return;
	}
	if (r == 0)
		syserr("CLOSED PIPE\n");
	if (r == -1)
		syserr("Error in read\n");
}

int main(){
	char buf[LINE_SIZE];
	while(strcmp(buf, "TERMINATE\0") != 0){
		readline(buf);
		if (is_final(buf) == 0){
			evalone(buf);
		}
		write(1, buf, strlen(buf)+1);
	}

	return 0;
}
