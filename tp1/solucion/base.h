
#ifndef _BASE_H_
#define _BASE_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned int uint;

typedef enum boolean
{
	False = 0,
	True = 1
} boolean;

typedef enum tipo_elementos {
	Integer = 0,
	Double  = 1,
	String  = 2
} tipo_elementos;

typedef union valor_elemento {
		int i;
		double d;
		char *s;
} __attribute__((__packed__)) valor_elemento;

valor_elemento ival(int i);
valor_elemento dval(double d);
valor_elemento sval(char *s);

#ifdef __cplusplus
}
#endif

#endif /// _BASE_H_



