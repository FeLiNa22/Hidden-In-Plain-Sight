#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include "../inc/encode.h"
#include "../inc/decode.h"

#define DEFAULT_ARGC 3

typedef int proc_wrapper(char *BMP_PATH, int extra_argc, char *extra_args[]);

struct cmd_process {
    char *name;
    int argc;
    proc_wrapper *proc;
};

/* Process Wrappers */
int encode_wrapper(char *BMP_PATH, int extra_argc, char *extra_args[]) {

    char *INP_PATH = extra_args[0];

    // Initializes default PASSWD to be NULL;
    char *PASSWD = "\0";

    // Initializes default OUT_PATH to be the filename of the image with the prefix "encoded_";
    char DEFAULT_NAME[500] = "encoded_";
    strcat(DEFAULT_NAME, basename(BMP_PATH));
    char *OUT_PATH = DEFAULT_NAME;

    // PARSE FLAGS
    if (extra_argc > 1) {

        for (int i = 0; i < extra_argc; ++i) {

            if (strcmp(extra_args[i], "-p") == 0) {

                PASSWD = extra_args[i + 1];

            } else if (strcmp(extra_args[i], "-o") == 0) {

                OUT_PATH = extra_args[i + 1];

            }

        }

    }

    return encode_BMP(BMP_PATH, INP_PATH, OUT_PATH, PASSWD);
}

int decode_wrapper(char *BMP_PATH, int extra_argc, char *extra_args[]) {

    // Initializes default PASSWD to be NULL;
    char *PASSWD = "\0";


    // PARSE FLAGS
    if (extra_argc > 0) {

        for (int i = 0; i < extra_argc; ++i) {

            if (strcmp(extra_args[i], "-p") == 0) {

                PASSWD = extra_args[i + 1];

            }

        }

    }

    return decode_BMP(BMP_PATH, PASSWD);
}

/* List of processes */

static struct cmd_process cmd_processes[] = {
        {"encode", 1, encode_wrapper},
        {"decode", 0, decode_wrapper}
};

static int no_of_cmds = sizeof(cmd_processes) / sizeof(cmd_processes[0]);

/* MAIN (cmdline parser)*/
int main(int argc, char **argv) {

    if (argc < DEFAULT_ARGC) {
        printf("[!] too few arguments provided \n");
        return EXIT_FAILURE;
    }

    char *proc_name = argv[1];
    char *BMP_PATH = argv[2];

    struct cmd_process *cmd_proc = NULL;

    // find which cmd proc_name has been chosen
    for (int i = 0; i < no_of_cmds; ++i) {
        if (strcmp(proc_name, cmd_processes[i].name) == 0) {
            cmd_proc = &cmd_processes[i];
        }
    }

    // if the cmd proc_name is invalid then output error message
    if (cmd_proc == NULL) {
        printf("[!] invalid process requested: %s is not defined\n", proc_name);
        return EXIT_FAILURE;
    }

    // if too little arguments were given for the process
    if (argc < cmd_proc->argc + DEFAULT_ARGC) {
        printf("[!] insufficient command line arguments. Process %s, takes %i arguments\n", cmd_proc->name,
               cmd_proc->argc);
        return EXIT_FAILURE;
    }

    // all checks passed so execute the process
    return cmd_proc->proc(BMP_PATH, argc - DEFAULT_ARGC, &argv[DEFAULT_ARGC]);

}
