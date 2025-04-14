#include "syscalls.h"


#ifndef S_IFCHR
#define S_IFCHR 0020000
#endif

int _write(int file, char *ptr, int len) {
    (void)ptr;
    (void)len;

    // Redirect stdout and stderr to UART
    if (file == STDOUT_FILENO || file == STDERR_FILENO) {
        // Your platform-specific code to send data to UART
        // For TM4C1294, you might use something like:
        for (int i = 0; i < len; i++) {
            // UARTCharPut(UART0_BASE, ptr[i]);  // Example function, use your actual UART function
        }
        return len;
    }
    
    errno = EBADF;
    return -1;
}


int _read(int file, char *ptr, int len) {
    (void)file;
    (void)ptr;
    (void)len;
    return 0; // No input
}

off_t _lseek(int file, off_t offset, int whence) {
    (void)file;
    (void)offset;
    (void)whence;
    errno = ESPIPE;
    return -1;
}

int _kill(pid_t pid, int sig) {
    (void)pid;
    (void)sig;
    errno = EINVAL;
    return -1;
}

int _isatty(int file) {
    (void)file;
    return 1; // Pretend it's a terminal
}

pid_t _getpid(void) {
    return 1; // Dummy PID
}

int _close(int file) {
    (void)file;
    return -1; // Close not supported
}

int _fstat(int file, struct stat *st) {
    (void)file;
    if (st) {
        st->st_mode = S_IFCHR; // Character device
    }
    return 0;
}