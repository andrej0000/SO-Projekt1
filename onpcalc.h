#ifndef ONPCALC_H
#define ONPCALC_H

// przelicza jedna operacje z buf (buf - NULL TERMINATED STRING)
void evalone(char *buf);

// sprawdza czy wyrazenie jest obliczone (buf - NULL TERMINATED STRING)
int is_final(char *buf);

#endif
