#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static void
usage(void)
{
        fprintf(stderr, "usage: lxlog [-n pkgname] [-l logfile] command [args...]\n");
        exit(1);
}

int
main(int argc, char *argv[])
{
        const char *pkg = NULL, *log = NULL;
        char *cmd, *p;
        int i;

        for (i = 1; i < argc; i++) {
                if (!strcmp(argv[i], "-n") || !strcmp(argv[i], "pkgname")) {
                        if (++i >= argc)
                                usage();
                        pkg = argv[i];
                } else if (!strcmp(argv[i], "-l") || !strcmp(argv[i], "log")) {
                        if (++i >= argc)
                                usage();
                        log = argv[i];
                } else {
                        break;
                }
        }
        if (i >= argc || !pkg || !log)
                usage();
        setenv("LXLOG_PKG", pkg, 1);
        setenv("LXLOG_FILE", log, 1);
        setenv("LD_PRELOAD", "/usr/lib/liblxlog.so", 1);

        cmd = malloc(4096); /* just a random amount */
        if (!cmd) {
                perror("malloc");
                return 1;
        }
        cmd[0] = '\0';
        for (; i < argc; i++) {
                strncat(cmd, argv[i], 4096 - strlen(cmd) - 1);
                strncat(cmd, " ", 4096 - strlen(cmd) - 1);
        }

        p = strrchr(cmd, '\0');
        if (p > cmd && p[-1] == ' ')
                p[-1] = '\0';
        execvp(argv[i], &argv[i]);
        perror("execvp");
        free(cmd);
        return 1;
}
