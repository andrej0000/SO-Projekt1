/*
 *	Andrzej Sulecki
 *	as320426
 *	Systemy operacyjne
 *	Zadanie zaliczeniowe 1
 */

#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include "err.h"
#include "shared.h"

char buf[LINE_SIZE];

int main(){
	while(strcmp(buf, "TERMINATE\0") != 0){
		readline(buf);
		if (is_final(buf) == 0){
			evalone(buf);
		}
		if(write(1, buf, strlen(buf)+1) == -1)
			syserr("Error in write");
	}

	return 0;
}
