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
        int i, cmd_start;

        for (i = 1; i < argc; i++) {
                if (!strcmp(argv[i], "-n") || !strcmp(argv[i], "--pkgname")) {
                        if (++i >= argc)
                                usage();
                        pkg = argv[i];
                } else if (!strcmp(argv[i], "-l") || !strcmp(argv[i], "--log")) {
                        if (++i >= argc)
                                usage();
                        log = argv[i];
                } else {
                        break;
                }
        }
        if (i >= argc || !pkg || !log)
                usage();

	cmd_start = i;

	/* Set environment */
	setenv("LXLOG_PKG", pkg, 1);
	setenv("LXLOG_FILE", log, 1);
	setenv("LD_PRELOAD", "/usr/lib/liblxlog.so", 1);

	/* Execute the command directly with argv */
	execvp(argv[cmd_start], &argv[cmd_start]);
	perror("execvp");
	return 1;
}
