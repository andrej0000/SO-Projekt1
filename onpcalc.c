#include "onpcalc.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*checks if expression betweend str[s] and str[e] is + - * or / */
int is_op(int s, int e, char * str)
{
	if (e-s != 2)
		return 0;
	char op = str[s+1];
	if (((op == '+') || (op == '-')) || ((op == '*') || (op == '/')))
		return 1;
	return 0;
}

/* returns next whitespace index in null-terminated string str */
int ws(int s, char *str, int l){
	s++;
	while (s<l && str[s] != ' '){
		s++;
	}
	return s;
}

/* returns value of number betweend s and e in string str */
int get_val(int s, char * str)
{
	return atoi(str+s);
}

/* returns result of operation arg1 op arg2 */
int op_calc(char op, int arg1, int arg2)
{
	switch(op){
	case '+':
		return arg1 + arg2;
	case '-':
		return arg1 - arg2;
	case '*':
		return arg1 * arg2;
	case '/':
		return arg1 / arg2;
	default:
		return 0;
	}
	return 0;
	
}

/* calculates one operation and puts changed expression in buf 
	buf should be null-terminated
*/
void evalone(char *buf)
{
	int len = strlen(buf);
	printf("len: %i", len);
	int f = -1;
	int s = -1;
	int o = -1;
	int e = -1;
	while (e < len && (is_op(o, e, buf) == 0)){
		f = s;
		s = o;
		o = e;
		e = ws(e, buf, len);
	}

	int arg1 = get_val(f+1, buf);
	int arg2 = get_val(s+1, buf);
	char op = buf[o+1];
	int res = op_calc(op, arg1, arg2);

	char *first_part= malloc(sizeof(char)*(s+1));
	char *second_part = malloc(sizeof(char)*(len-e+1));
	memcpy(first_part, buf, f+1);
	memcpy(second_part, buf+e, len-e);
	second_part[len-e] = '\0';
	sprintf(buf, "%s%i%s", first_part, res, second_part);
}

int is_final(char *buf)
{
	int s = -1;
	int e = -1;
	while (e != strlen(buf)){
		s = e;
		e = ws(s, buf, strlen(buf));
		if (is_op(s, e, buf) == 1)
			return 0;
	}
	return 1;
}

