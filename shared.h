#ifndef ONPCALC_H
#define ONPCALC_H

#define LINE_SIZE 16384

// przelicza jedna operacje z buf (buf - NULL TERMINATED STRING)
void evalone(char *buf);

// sprawdza czy wyrazenie jest obliczone (buf - NULL TERMINATED STRING)
int is_final(char *buf);

/*
 * Czyta z stdin string zakonczony \n badz \0
 * Do bufora trafia tenze string z koncowym nullem
 */
void readline(char * buf);


#endif
