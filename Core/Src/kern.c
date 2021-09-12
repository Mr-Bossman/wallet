#include <_ansi.h>
#include <_syslist.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/times.h>
#include <limits.h>
#include <signal.h>
#include "kern.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unwind.h>

#define STDIN_FILENO  0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2

extern QSPI_HandleTypeDef hqspi;


_Unwind_Reason_Code unwind_backtrace_callback(struct _Unwind_Context* context, void* arg) {
    uintptr_t pc = _Unwind_GetIP(context);
    printf("0x%x\n", pc);
    return _URC_NO_REASON;
}

void print_trace (void)
{
  printf("code %x\n", HAL_QSPI_GetState(&hqspi));
  printf("code %lx\n", HAL_QSPI_GetError(&hqspi));
  _Unwind_Reason_Code rc = _Unwind_Backtrace(unwind_backtrace_callback, 0);
  printf("code %d\n", rc);
}

void sig_func (int sig)
{
    printf("exit %d\n", sig);
}

UART_HandleTypeDef *gHuart;

void RetargetInit(UART_HandleTypeDef *huart) {
  gHuart = huart;
  for(uint8_t i = 0;i <= 32;i++)
    signal(i,sig_func);
  setvbuf(stdout, NULL, _IONBF, 0);
}

int _isatty(int fd) {
  if (fd >= STDIN_FILENO && fd <= STDERR_FILENO)
    return 1;

  errno = EBADF;
  return 0;
}

int _write(int fd, char* ptr, int len) {
  HAL_StatusTypeDef hstatus = HAL_OK;

  if (fd == STDOUT_FILENO || fd == STDERR_FILENO) {
    for(int i = 0; i < len;i++){
      if(*(ptr+i) == '\n'){
        hstatus |= HAL_UART_Transmit(gHuart, (uint8_t *) ptr, i, HAL_MAX_DELAY);
        uint8_t n = '\r';
        hstatus |= HAL_UART_Transmit(gHuart, &n, 1, HAL_MAX_DELAY);
        len -=i;
        i=0;
        ptr += i;
      }
    }
    hstatus |= HAL_UART_Transmit(gHuart, (uint8_t *) ptr, len, HAL_MAX_DELAY);
    if (hstatus == HAL_OK)
      return len;
    else
      return EIO;
  }
  errno = EBADF;
  return -1;
}

int _close(int fd) {
  if (fd >= STDIN_FILENO && fd <= STDERR_FILENO)
    return 0;

  errno = EBADF;
  return -1;
}

int _lseek(int fd, int ptr, int dir) {
  (void) fd;
  (void) ptr;
  (void) dir;

  errno = EBADF;
  return -1;
}

int _read(int fd, char* ptr, int len) {
  HAL_StatusTypeDef hstatus;

  if (fd == STDIN_FILENO) {
    hstatus = HAL_UART_Receive(gHuart, (uint8_t *) ptr, 1, HAL_MAX_DELAY);
    if (hstatus == HAL_OK)
      return 1;
    else
      return EIO;
  }
  errno = EBADF;
  return -1;
}

int _fstat(int fd, struct stat* st) {
  if (fd >= STDIN_FILENO && fd <= STDERR_FILENO) {
    st->st_mode = S_IFCHR;
    return 0;
  }

  errno = EBADF;
  return 0;
}

#include <stdio.h>
#include <string.h>
#include <malloc.h>

/* gcc -Wl,--wrap=free -Wl,--wrap=realloc */

void* __real_realloc(void *, size_t);

//zero memory even on error...
void* __wrap_realloc(void *ptr, size_t sz) {
    memset(ptr,0,malloc_usable_size(ptr));
    void *ret = __real_realloc(ptr,sz);
    if(!ret)free(ptr);
    return ret;
}

void __real_free(void *);

void __wrap_free(void *ptr) {
    memset(ptr,0,malloc_usable_size(ptr));
    //__real_free(ptr);
    ptr = NULL;
}