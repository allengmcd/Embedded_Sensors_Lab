#ifndef SYSCALLS_H
#define SYSCALLS_H

#include "syscalls.h"
#include <errno.h>
#include <sys/unistd.h> // STDOUT_FILENO, STDERR_FILENO
#include <sys/types.h>
#include <sys/stat.h>

int _write(int file, char *ptr, int len);
int _read(int file, char *ptr, int len);
off_t _lseek(int file, off_t offset, int whence);
int _kill(pid_t pid, int sig);
int _isatty(int file);
pid_t _getpid(void);
int _close(int file);
int _fstat(int file, struct stat *st);

#endif