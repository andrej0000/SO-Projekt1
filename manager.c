/*
 *	Andrzej Sulecki
 *	as320426
 *	Systemy operacyjne
 *	Zadanie zaliczeniowe 1
 */

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "err.h"
#include "shared.h"


const char * PATH = "DATA/";

int main(int argc, char ** argv){
	if (argc != 4){
		printf("Wrong arguments\n");
		return 1;
	}
	int N = atoi(argv[1]);
	char datafile[80];
	strcpy(datafile, PATH);
	strcpy(datafile+strlen(PATH), argv[2]);

	char outputfile[80];
	strcpy(outputfile, PATH);
	strcpy(outputfile+strlen(PATH), argv[3]);

	int tmp_pipe[2];
	if (pipe(tmp_pipe) == -1)
		syserr("Error in pipe");

	//Podmiana stdout w manadzerze
	if (close(1) == -1)
		syserr("Error in close");
	if (dup2(tmp_pipe[1], 1) == -1)
		syserr("Error in dup2");
	int i = 0;
	int infd = tmp_pipe[0];
	for (i = 0; i < N; i++){
		//Zamykanie nie potrzebnego pipe'a
		if (close(tmp_pipe[1]) == -1)
			syserr("Error in close");
		if (pipe(tmp_pipe) == -1)
			syserr("Error in pipe");
		switch (fork()){
		case -1:
			syserr("Error in fork");
			break;
		case 0: // jestesmy w dziecku
			//podmianka stdin
			if (close(0) == -1)
				syserr("Error in close child stdin");
			if (dup2(infd, 0) == -1)
				syserr("Error in dup2 child stdin");
			if (close(infd) == -1)
				syserr("Error in close child infd");

			//podmianka stdout
			if (close(1) == -1)
				syserr("Error in close child stdout");
			if (dup2(tmp_pipe[1], 1) == -1)
				syserr("Error in dup2 child stdout");
			if (close(tmp_pipe[1]))
				syserr("Error in close child");
			//zamykanie nie potrzebnych lacz
			if (close(tmp_pipe[0]) == -1)
				syserr("Error in close redundant pipe");
			//exec
			execlp("./executor", "executor", (char *) NULL);
			syserr("Error in exec");
			break;
		default: // jestesmy managerem
			if (close(infd) == -1)
				syserr("Error in close infd");
			//zapisujemy potrzebny jeszcze koniec pipe'a
			infd = tmp_pipe[0];
			break;
		}
	}

	//Podmiana stdin w managerze
	if (close(0) == -1)
		syserr("Error in close manager stdin");
	if (dup2(infd, 0) == -1)
		syserr("Error in dup2");


	//Zakonczona incjalizacja procesow

	FILE* source;
	FILE* dest;
	source = fopen(datafile, "r");
	if (source == NULL)
		syserr("Error opening data file");
	dest = fopen(outputfile, "w");
	if (dest == NULL)
		syserr("Error opening data file");

	char buf[LINE_SIZE];
	char fbuf[LINE_SIZE];
	if (fgets(fbuf, LINE_SIZE, source) == NULL)
		syserr("Wrong data file format");
	int lines = atoi(fbuf);
	int inside = 0; //ilosc wyrazen 'wpuszczonych' do pierscienia
	int final = 0; //ilosc obliczonych wyrazen
	int read = 0; //ilosc przeczytanych z pliku wyrazen

	while (final < lines){ //jesli nie obliczylismy wszystkich
		 //wolne miejsce w pierscieniu i zostalo cos do przeczytania
		if (inside < N && read < lines){
			sprintf(fbuf, "%i: ", read + 1);
			if (fgets(fbuf + strlen(fbuf), LINE_SIZE - strlen(fbuf), source) != NULL){
				//zapewniamy konczenie sie na NULL a nie \n
				if (fbuf[strlen(fbuf)-1] == '\n')
					fbuf[strlen(fbuf)-1] = '\0';
				write(1, fbuf, strlen(fbuf)+1);
				inside++;
				read++;
			}
		}
		else {
			readline(buf);
			if (is_final(buf) == 1){
				inside--;
				final++;
				fprintf(dest, "%s\n", buf);
			}
			else {
				if(write(1, buf, strlen(buf)+1) == -1)
					syserr("Error in write");
			}
		}
	}

	//Wiadomosc konca
	if(write(1, "TERMINATE\0", 10) == -1)
		syserr("Error in write");

	//Koniec
	for (i = 0; i < N; i++)
		if (wait(0) == -1)
			syserr("Error in wait");
	return 0;
}
