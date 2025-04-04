#ifndef SYSCALLS_H
#define SYSCALLS_H


// You could also include the syscall prototypes for documentation,
// though they're not strictly necessary
int _write(int file, char *ptr, int len);
int _read(int file, char *ptr, int len);

#endif /* SYSCALLS_H */