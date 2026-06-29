// file : pomoc.c 
// date : 2026-06-27
// std  : c23

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#include "protocol.h"

// this is just the normal arg parsing. 

// function for sending command to the daemon. 
static int send_command(const char *cmd, char *reply, int reply_size) {
    // initialise the socket and crate fd. 
    int fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (fd < 0) { perror("socket"); return -1; }


    // let's now know the structure of the sockaddr_un what is it? 
    // struct sockaddr_run {
    //      sa_family_t sun_family;
    //      char sun_path[108];
    // }
    // therefore, we need to first : zero the entire struct, avoid the garbage - done by memset. 
    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));

    // set the sun family to AF_UNIX. 
    addr.sun_family = AF_UNIX;

    // our socket : SOCKET_PATH is far more less bytes than the maximum size of the sun_path : 108 bytes. 
    // accounting for the null terminator - it should be there always, for it to be a valid C string. 
    // therefore, we can have a string path of length 107 and the last byte goes for the null terminator. 

    // strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);
    // note that we have zeroed out the entire struct field via memset.
    // we copy SOCKET_PATH to sun_path, that is, we copy, sizeof(addr.sun_path) - 1, that is 107 bytes from the SOCKET_PATH to the sun_path, case, the remaining are just nulled '\0', yet the path is valid. 


    // the above strncpy has the following function signature : 
    // strncpy : string do n copy (the expanded form, that is what I call it.)
    // char *strncpy(char dst[restrict dsize], const char *restrict src, size_t dsize)
    // here dsize is the destination size, src is the source string. 
    // literally means - from the source string, copy dsize amount of bytes into the dst buffer (which by the signature's declaration is exactly dsize bytes in size. perfect match.)

    // and this is snprintf. It would definitely add a null terminator, whatsoever via truncation.
    // see the man 3 snprintf if you need to learn more. (i use it btw.)
    snprintf(addr.sun_path, sizeof(addr.sun_path), "%s", SOCKET_PATH);

    if (connect(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        fprintf(stderr, "%s: pomod not running\n", RES_ERR);
        close(fd);
        return -1;
    }

    write(fd, cmd, strlen(cmd));

    int n = read(fd, reply, reply_size - 1);
    if (n > 0) reply[n] = 0;
    else reply[0] = 0;

    close(fd);
    return EXIT_SUCCESS;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "usage: pomoc <command> [args]\n");
        return 1;
    }

    char cmd[256];
    char reply[256];
    memset(cmd,   0, sizeof(cmd));
    memset(reply, 0, sizeof(reply));

    if (strcmp(argv[1], "start") == 0) {
        snprintf(cmd, sizeof(cmd), "%s", CMD_START);

    } else if (strcmp(argv[1], "pause") == 0) {
        snprintf(cmd, sizeof(cmd), "%s", CMD_PAUSE);

    } else if (strcmp(argv[1], "toggle") == 0) {
        snprintf(cmd, sizeof(cmd), "%s", CMD_TOGGLE);

    } else if (strcmp(argv[1], "end") == 0) {
        snprintf(cmd, sizeof(cmd), "%s", CMD_END);

    } else if (strcmp(argv[1], "status") == 0) {
        if (argc == 2)
            snprintf(cmd, sizeof(cmd), "%s", CMD_STATUS);
        else if (strcmp(argv[2], "active") == 0)
            snprintf(cmd, sizeof(cmd), "%s", CMD_STATUS_ACT);
        else if (strcmp(argv[2], "time") == 0)
            snprintf(cmd, sizeof(cmd), "%s", CMD_STATUS_TIME);
        else if (strcmp(argv[2], "hr") == 0)
            snprintf(cmd, sizeof(cmd), "%s", CMD_STATUS_HR);
        else if (strcmp(argv[2], "min") == 0)
            snprintf(cmd, sizeof(cmd), "%s", CMD_STATUS_MIN);
        else if (strcmp(argv[2], "sec") == 0)
            snprintf(cmd, sizeof(cmd), "%s", CMD_STATUS_SEC);
        else {
            fprintf(stderr, "%s: unknown status field '%s'\n", RES_ERR, argv[2]);
            return 1;
        }

    } else if (strcmp(argv[1], "focus") == 0) {
        if (argc < 3) { fprintf(stderr, "%s: missing args\n", RES_ERR); return 1; }
        if (argc == 4)
            snprintf(cmd, sizeof(cmd), "%s focus %s %s", CMD_INCR, argv[2], argv[3]);
        else
            snprintf(cmd, sizeof(cmd), "%s %s", CMD_SET_FOCUS, argv[2]);

    } else if (strcmp(argv[1], "break") == 0) {
        if (argc < 3) { fprintf(stderr, "%s: missing args\n", RES_ERR); return 1; }
        if (argc == 4)
            snprintf(cmd, sizeof(cmd), "%s break %s %s", CMD_INCR, argv[2], argv[3]);
        else
            snprintf(cmd, sizeof(cmd), "%s %s", CMD_SET_BREAK, argv[2]);

    } else {
        fprintf(stderr, "%s: unknown command '%s'\n", RES_ERR, argv[1]);
        return 1;
    }

    if (send_command(cmd, reply, sizeof(reply)) == 0)
        printf("%s\n", reply);

    return EXIT_SUCCESS;
}
