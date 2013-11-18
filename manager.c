#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include "err.h"
#include "onpcalc.h"
#include <string.h>
#include <stdlib.h>


const int LINE_SIZE = 1024;

/*
 * Czyta z stdin string zakonczony \n badz \0
 * Do bufora trafia tenze string z koncowym nullem
 */
void readline(char * buf){
	char b;
	int i = 0;
	while (read(0, &b, 1) == 1){
		if (b=='\n')
			b = '\0';
		buf[i] = b;
		i++;
		if (b=='\0')
			return;
	}

}


int main(int argc, char ** argv){
	if (argc != 4){
		printf("Wrong arguments\n");
		return 1;
	}
	int N = atoi(argv[1]);
	char *datafile = argv[2];
	char *outputfile = argv[3];
	//int a;
	//for (a = 0; a < argc; a++)
	//	printf("%s\n", argv[a]);
	int tmp_pipe[2];
	if (pipe(tmp_pipe) == -1)
		syserr("Error in pipe");
	if (close(1) == -1)
		syserr("Error in close");
	if (dup2(tmp_pipe[1], 1) == -1)
		syserr("Error in dup2");
	int i = 0;
	int infd = tmp_pipe[0];
	for (i = 0; i < N; i++){
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
			//fprintf(stderr, "%i %i infd %i\n", tmp_pipe[0], tmp_pipe[1], infd);
			if (close(infd) == -1)
				syserr("Error in close infd");
			infd = tmp_pipe[0];
			break;
		}
	}

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
	int inside = 0;
	int final = 0;
	int read = 0;
	while (final < lines){
		if (inside < N && read < lines){
			sprintf(fbuf, "%i: ", read);
			if (fgets(fbuf + strlen(fbuf), LINE_SIZE - strlen(fbuf), source) != NULL){
				//fprintf(stderr, "%s", fbuf);
			}
			if (fbuf[strlen(fbuf)-1] == '\n')
				fbuf[strlen(fbuf)-1] = '\0';
			//fprintf(stderr, "MANAGER WYSYLAM %i str %s\n", strlen(fbuf), fbuf);
			write(1, fbuf, strlen(fbuf)+1);
			inside++;
			read++;
		}
		else {
			readline(buf);
			if (is_final(buf) == 1){
				inside--;
				final++;
				fprintf(dest, "%s\n", buf);
			}
			else {
				write(1, buf, strlen(buf)+1);
			}
		}
	}





	write(1, "TERMINATE\0", 10);

	//Koniec
	for (i = 0; i < N; i++)
		if (wait(0) == -1)
			syserr("Error in wait");
	return 0;
}
