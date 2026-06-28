// name : protocol.c 
// date : 2026-06-27 
// std  : C 23 standard 

#include "protocol.h"

// socket path
const char *SOCKET_PATH = "/tmp/pomo.socket";

// commands to start pause and end the pomodoro. 
const char *CMD_START = "START";
const char *CMD_PAUSE = "PAUSE";
const char *CMD_TOGGLE = "TOGGLE";
const char *CMD_END = "END";

// client commands to get various status of the pomod daemon. 
const char *CMD_STATUS = "STATUS";
const char *CMD_STATUS_ACT = "STATUS active";
const char *CMD_STATUS_TIME = "STATUS time";
const char *CMD_STATUS_HR = "STATUS hr";
const char *CMD_STATUS_MIN = "STATUS min";
const char *CMD_STATUS_SEC = "STATUS sec";

// client commands to set the time for focus and break, override the configfile. 
const char *CMD_SET_FOCUS = "SET focus";
const char *CMD_SET_BREAK = "SET break";

// client commands to increment the time for focus or break.
const char *CMD_INCR = "INCR";

// the response strings for OK and ERR. 
const char *RES_OK = "OK";
const char *RES_ERR = "ERR";

// standard buffer size. 
const int BUF_SIZE = 256;


