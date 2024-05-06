#define __NL__

#define INT set /A
#define SET(a) = a

				
#define EQAUL(c,a,b) if NOT %a% == %b% goto c\
				 SETLOCAL

#define ELSE(c) goto c ## 2\
			:c

#define ENDELSE(a) ENDLOCAL\
			:a ## 2

#define ENDIF(a) ENDLOCAL\
			:a

@echo off