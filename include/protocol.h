// file : protocol.h 

#ifndef PROTOCOL_H
#define PROTOCOL_H

// socket path
extern const char *SOCKET_PATH;

// commands to start pause and end the pomodoro. 
extern const char *CMD_START;
extern const char *CMD_PAUSE;
extern const char *CMD_TOGGLE;
extern const char *CMD_END;

// client commands to get various status of the pomod daemon. 
extern const char *CMD_STATUS;
extern const char *CMD_STATUS_ACT;
extern const char *CMD_STATUS_TIME;
extern const char *CMD_STATUS_HR;
extern const char *CMD_STATUS_MIN;
extern const char *CMD_STATUS_SEC;

// client commands to set the time for focus and break, override the configfile. 
extern const char *CMD_SET_FOCUS;
extern const char *CMD_SET_BREAK;

// client commands to increment the time for focus or break.
extern const char *CMD_INCR;

// the response strings for OK and ERR. 
extern const char *RES_OK;
extern const char *RES_ERR;

// standard buffer size. 
extern const int BUF_SIZE;

#endif

// tip : alternatively, we can also use #pragma once. (modern. )

