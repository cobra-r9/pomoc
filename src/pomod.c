// file : pomod.c 
// data : 27-06-2026
// std  : c23

// include the systemwide headers. 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/select.h>
#include <sys/time.h>


// include the program specific headers. 
#include "protocol.h"
#include "state.h"
#include "config.h"
#include "timer.h"

// initialise the PomData struct. 
// name it the data. 
static PomData data;

// define the helper functions.

// define the is_running function. 
// this function will return 0 if something is running. ie the countdown is running. 
// either break or focus is running. 
// it would return 1 if data's state is either idle or paused. (that is remaining two states of the PomState)
static int is_running(void) {
    return data.state == STATE_FOCUS_RUNNING ||
           data.state == STATE_BREAK_RUNNING;
}

// a hepler function to format the time to 2 integer places, whatsoever, prefixing zero. 
static void format_time(char *buf, int hr, int min, int sec) {
    // if the hour exists (it is not 0), we format the hour too. 
    if (hr > 0)
        sprintf(buf, "%02d:%02d:%02d", hr, min, sec);
    // if the hour is invalid or 0, we do not format the hour, instead only min and sec. 
    else
        sprintf(buf, "%02d:%02d", min, sec);
}

// handle the commands from the client. 
// we shall design this as handle_command function, which modifies the reply as per the command in each case. 
static void handle_command(const char *cmd, char *reply) {
    
    // STATUS commands. 
    // remember the protocol.c? we have defined what the CMD_STATUS looks like. we compare if the cmd is the same as provided.
    // if so, we craft the reply as such. 
    if (strcmp(cmd, CMD_STATUS) == 0) {
        char timebuf[32];
        format_time(timebuf, data.remaining_hr, data.remaining_min, data.remaining_sec);
        sprintf(reply, "%s %s %s", state_str(&data), state_phase_str(&data), timebuf);
        return;
    }

    // similarly, reply what is active : see the state.h and state.c 
    if (strcmp(cmd, CMD_STATUS_ACT) == 0) {
        sprintf(reply, "%s", state_phase_str(&data));
        return;
    }

    // similarly for the remaining time. parsed as string. 
    if (strcmp(cmd, CMD_STATUS_TIME) == 0) {
        char timebuf[32];
        format_time(timebuf, data.remaining_hr, data.remaining_min, data.remaining_sec);
        sprintf(reply, "%s", timebuf);
        return;
    }

    // we are goning to do this formating via strcmp and sprintf sequently for all the commands in protocl.h and craft a reply. 
    if (strcmp(cmd, CMD_STATUS_HR) == 0) {
        sprintf(reply, "%02d", data.remaining_hr);
        return;
    }

    if (strcmp(cmd, CMD_STATUS_MIN) == 0) {
        sprintf(reply, "%02d", data.remaining_min);
        return;
    }

    if (strcmp(cmd, CMD_STATUS_SEC) == 0) {
        sprintf(reply, "%02d", data.remaining_sec);
        return;
    }

    // for command START 
    if (strcmp(cmd, CMD_START) == 0) {
        // if the data state is not idel, or it is not paused, it means that the pomo is either in break or focus 
        // that is : the pomo is already running. 
        // so we graft a reply, put response to err. 
        if (data.state != STATE_IDLE && data.state != STATE_FOCUS_PAUSED) {
            sprintf(reply, "%s already running", RES_ERR);
            return;
        }

        // in case the data state is idle mode, 
        // we need to load the focus, update the PomData struct. 
        if (data.state == STATE_IDLE) timer_load_focus(&data);

        // then we flip the state from whatever it is previous to state focus running. 
        data.state = STATE_FOCUS_RUNNING;

        // craft the OK reply. 
        sprintf(reply, "%s", RES_OK);
        return;
    }

    // for command PAUSE 
    if (strcmp(cmd, CMD_PAUSE) == 0) {
        // similarly if the state is not focus running, say it is not in focus. 
        if (data.state != STATE_FOCUS_RUNNING) {
            sprintf(reply, "%s not in focus", RES_ERR);
            return;
        }

        // if we are gonna hit the pause command twice. 
        // @ first pause - the running timer's state is paused. 
        // so, its state is not in STATE_FOCUS_RUNNING.
        // @ second pause - the above if block executes. leading to ERR. 

        // flip to state focus paused. 
        data.state = STATE_FOCUS_PAUSED;

        // craft the reply and format. 
        sprintf(reply, "%s", RES_OK);
        return;
    }

    // for command END 
    if (strcmp(cmd, CMD_END) == 0) {
        // if you try to end the timer when the break is running...
        // then , nope, I am not going to allow 5 hours of programming. You definitely need to touch some grass. 
        if (data.state == STATE_BREAK_RUNNING) {
            sprintf(reply, "%s cannot end break. touch some grass.", RES_ERR);
            return;
        }

        // case the data state is in IDLE  mode, then it means that we have not yet started the timer. 
        if (data.state == STATE_IDLE) {
            sprintf(reply, "%s not running", RES_ERR);
            return;
        }

        // if neither of the above if blocks executes, we need to load the break. 

        // load the break timer data into PomData struct. 
        timer_load_break(&data);

        // flip the state to BREAK_RUNNING after the focus ends. 
        data.state = STATE_BREAK_RUNNING;
        sprintf(reply, "%s", RES_OK);
        return;
    }

    // command for setting custom timer.
    // SET focus hr:min:sec 
    if (strncmp(cmd, CMD_SET_FOCUS, strlen(CMD_SET_FOCUS)) == 0) {
        if (is_running()) { sprintf(reply, "%s running", RES_ERR); return; }
        int a, b, c;
        if (sscanf(cmd + strlen(CMD_SET_FOCUS) + 1, "%d:%d:%d", &a, &b, &c) == 3) {
            data.focus_hr = a; data.focus_min = b; data.focus_sec = c;
            sprintf(reply, "%s", RES_OK);
        } else {
            sprintf(reply, "%s bad format", RES_ERR);
        }
        return;
    }

    // SET break min:sec 
    if (strncmp(cmd, CMD_SET_BREAK, strlen(CMD_SET_BREAK)) == 0) {
        if (is_running()) { sprintf(reply, "%s running", RES_ERR); return; }
        int a, b;
        if (sscanf(cmd + strlen(CMD_SET_BREAK) + 1, "%d:%d", &a, &b) == 2) {
            data.break_min = a; data.break_sec = b;
            sprintf(reply, "%s", RES_OK);
        } else {
            sprintf(reply, "%s bad format", RES_ERR);
        }
        return;
    }

    // command for incrementing the focus while not running : INCR means not just increment,
    // we can + as well as - individual time fields. 
    // INCR focus|break hr|min|sec +N|-N 
    if (strncmp(cmd, CMD_INCR, strlen(CMD_INCR)) == 0) {
        if (is_running()) { sprintf(reply, "%s running", RES_ERR); return; }

        char phase[16], field[8];
        int delta;
        if (sscanf(cmd + strlen(CMD_INCR) + 1, "%15s %7s %d", phase, field, &delta) != 3) {
            sprintf(reply, "%s bad format", RES_ERR);
            return;
        }

        int *target = NULL;
        if (strcmp(phase, "focus") == 0) {
            if      (strcmp(field, "hr")  == 0) target = &data.focus_hr;
            else if (strcmp(field, "min") == 0) target = &data.focus_min;
            else if (strcmp(field, "sec") == 0) target = &data.focus_sec;
        } else if (strcmp(phase, "break") == 0) {
            if      (strcmp(field, "min") == 0) target = &data.break_min;
            else if (strcmp(field, "sec") == 0) target = &data.break_sec;
        }

        if (!target) { sprintf(reply, "%s bad field", RES_ERR); return; }

        *target += delta;
        if (*target < 0) *target = 0;
        sprintf(reply, "%s", RES_OK);
        return;
    }

    sprintf(reply, "%s unknown command", RES_ERR);
}

// the main function. the daemon and stuffs. 
int main(void) {
    // initialise the data struct. 
    state_init(&data);
    // load the config into the data struct. 
    config_load(&data);
    // load the focus timer. 
    timer_load_focus(&data);

    // create a UNIX domain socket, type SOCK_STREAM. 
    int server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    // return's negative value on failure. if so, then exit with failure status. 
    if (server_fd < 0) { 
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // if there exists a prevoiusly created socket, then unlink the socket. 
    // SOCKET_PATH is defined in the protocol.h 
    unlink(SOCKET_PATH);

    // create addr struct. 
    // sys/un.h has sockaddr_un 
    struct sockaddr_un addr;

    // initialise the addr field with zero value. 
    memset(&addr, 0, sizeof(addr));

    // set the sun family  to UNIX faimily. 
    addr.sun_family = AF_UNIX;

    // initialuse the addr struct's socket path. 
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);

    // bind to the server_fd; if failed, exit with error status. 
    if (bind(server_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    // listen upto 5 clients. 
    // on failure, similarly exit with error code. 
    if (listen(server_fd, 5) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    // output : print that the daemon is running. 
    printf("pomod running. socket: %s\n", SOCKET_PATH);

    while (1) {
        fd_set rfds;
        // fd_set is a set of file descriptors. We are declaring it fresh every iteration. 
        FD_ZERO(&rfds);
        // we zero out the fd_set we created. Because rfds is modified by select in every iteratoin. 
        FD_SET(server_fd, &rfds);
        // then we add server_fd to the watch set (rfds). We are telling select to watch this fd for incomming connections.

        struct timeval tv = { .tv_sec = 1, .tv_usec = 0 };
        // have we added something like sleep 1? somewhere? Nope. This timeval is a struct that is - time in seconds and microseconds. 
        // Describes the time in seconds and microseconds, in sys/time.h

        int ret = select(server_fd + 1, &rfds, NULL, NULL, &tv);
        // This is the time block. Wakes up when either a client connects, or 1 second passes.

        // only tick if the state of the pomodoro is either running break or running focus. 
        if (data.state == STATE_FOCUS_RUNNING || data.state == STATE_BREAK_RUNNING) {
            // we tick the time accordingly. 
            // modifiy the remaining_hr, min or sec. 
            int done = timer_tick(&data.remaining_hr, &data.remaining_min, &data.remaining_sec);
            // if the timer tick succeeds. then 
            if (done) {
                // we load the break if focus has completed. 
                if (data.state == STATE_FOCUS_RUNNING) {
                    timer_load_break(&data);
                    data.state = STATE_BREAK_RUNNING;
                    printf("Focus done. Break started.\n");
                // else we load the idle state if break has completed. 
                } else {
                    data.state = STATE_IDLE;
                    timer_load_focus(&data);
                    printf("Break done. Back to idle.\n");
                }
            }
        }

        if (ret > 0 && FD_ISSET(server_fd, &rfds)) {
            int client_fd = accept(server_fd, NULL, NULL);
            if (client_fd < 0) continue;

            char buf[BUF_SIZE];
            memset(buf, 0, BUF_SIZE);
            int n = read(client_fd, buf, BUF_SIZE - 1);
            if (n > 0) {
                buf[strcspn(buf, "\n")] = 0;

                char reply[BUF_SIZE];
                memset(reply, 0, BUF_SIZE);
                handle_command(buf, reply);

                write(client_fd, reply, strlen(reply));
            }
            close(client_fd);
        }
    }

    return 0;
}
