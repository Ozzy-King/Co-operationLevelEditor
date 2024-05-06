#include "conv.h"

INT test SET(100)
INT test2 SET(101)
INT test3 SET(101)

EQAUL(1, test, test2)
	echo equal
ELSE(1)
	echo not eqaul
	EQAUL(2, test2, test3)
		echo not eqaul
	ENDIF(2)
ENDELSE(1)