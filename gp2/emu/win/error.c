/*
    error.c --
    Error logging 
*/

#include "shared.h"

FILE *error_log;
extern t_Callbacks *m_CallBacks;

struct {
    int enabled;
    int verbose;
    FILE *log;
} t_error;

void error_init(void)
{
#ifdef DEBUG    
    //error_log = fopen("error.log","w");
#endif
}

void error_shutdown(void)
{
#ifdef DEBUG    
    //if(error_log) fclose(error_log);
#endif
}

void error(char *format, ...)
{
    va_list ap;
    char token[0x100];
	char msg[0x100];
    
    strcpy(msg, "\0");
    va_start(ap, format);
    vsprintf(token, format, ap);
    strcat(msg, token);
    va_end(ap);

	m_CallBacks->m_fctLogMessage(0,msg);
}

void level_error(int level,char *format, ...)
{
    va_list ap;
    char token[0x100];
	char msg[0x100];
    
    strcpy(msg, "\0");
    va_start(ap, format);
    vsprintf(token, format, ap);
    strcat(msg, token);
    va_end(ap);

	m_CallBacks->m_fctLogMessage(level,msg);
}

