/* See LICENSE file for license details.
 *
 * liblxpkglog.so - A shared library for lxpkg to track installed files
 * and directories.
 *
 * author: cowmonk
 */
#define _GNU_SOURCE
#include <dlfcn.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define PATH_MAX 4096

static FILE *log_file = NULL;
static char buf[PATH_MAX];
static const char *pkg;

static void
log(const char *fmt, ...)
{
        va_list ap;
        if (!log_file || !pkg)
                return;
        va_start(ap, fmt);
        fprintf(log_file, "%s: ", pkg);
        vfprintf(log_file, fmt, ap);
        fputc('\n', log_file);
        fflush(log_file);
        va_end(ap);
}

/* Initialize the log file from the LXPKG_FILE and LX_PKG environment variable */
static void
init_log()
{
        const char *log_path;
        if (!log_path)
                return;
        log_path = getenv("LXPKG_FILE");
        pkg = getenv("LXPKG_PKG");
        if (log_path) {
                log_file = fopen(log_file, "w");
                        perror("log file environment isn't set");
        }
}

/* Absolute path helper */
static const char *
abspath(const char *path)
{
        if (path[0] == '/' || getcwd(buf, PATH_MAX))
                return path;
        snprintf(buf + strlen(buf), PATH_MAX - strlen(buf), "/%s", path);
        return buf;
}

/* Override open system call */
int
open(const char *path, int flags, ...)
{
        static int (*real_open)(const char *, int, ...) = NULL;
        va_list ap;
        mode_t m;

        if (!real_open) real_open = dlsym(RTLD_NEXT, "open");
        init_log();
        if (flags & O_CREAT) {
                va_start(ap, flags);
                m = va_arg(ap, mode_t);
                log("FILE: %s", abspath(path));
                va_end(ap);
                return real_open(path, flags, m);
        }
        va_start(ap, flags);
        m = va_arg(ap, mode_t);
        va_end(ap);
        return real_open(path, flags, m);
}

/* Override mkdir system call */
int
mkdir(const char *pathname, mode_t mode)
{
        static int (*real_mkdir)(const char *, mode_t) = NULL;
        if (!real_mkdir) real_mkdir = dlsym(RTLD_NEXT, "mkdir");
        init_log();
        log("DIR: %s", abspath(pathname));
        return real_mkdir(pathname, mode);
}

/* Override symlink system call */
int
symlink(const char *target, const char *linkpath)
{
        static int (*real_symlink)(const char *, const char *) = NULL;
        if (!real_symlink) real_symlink = dlsym(RTLD_NEXT, "symlink");
        init_log();
        log("SYMLINK: %s -> %s", abspath(link), target);
        return real_symlink(target, link);
}
