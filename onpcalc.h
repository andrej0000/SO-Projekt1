#ifndef ONPCALC_H
#define ONPCALC_H

/*checks if expression betweend str[s] and str[e] is + - * or / */
int is_op(int s, int e, char * str);

/* returns next whitespace index in null-terminated string str */
int ws(int s, char *str, int l);

/* returns value of number betweend s and e in string str */
int get_val(int s, char * str);

/* returns result of operation arg1 op arg2 */
int op_calc(char op, int arg1, int arg2);

/* calculates one operation and puts changed expression in buf 
	buf should be null-terminated
*/
void evalone(char *buf);

/* checks if expression in buf is fully calculated */
int is_final(char *buf);



#endif
