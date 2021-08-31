// All credit to Carmine Noviello for this code
// https://github.com/cnoviello/mastering-stm32/blob/master/nucleo-f030R8/system/src/retarget/retarget.c

#include <_ansi.h>
#include <_syslist.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/times.h>
#include <limits.h>
#include <signal.h>
#include "retarget.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unwind.h>



_Unwind_Reason_Code unwind_backtrace_callback(struct _Unwind_Context* context, void* arg) {
    uintptr_t pc = _Unwind_GetIP(context);
    printf("0x%x\n", pc);
    return _URC_NO_REASON;
}

void print_trace (void)
{
  _Unwind_Reason_Code rc = _Unwind_Backtrace(unwind_backtrace_callback, 0);
  printf("code %d\n", rc);
}

void sig_func (int sig)
{
    printf("exit %d\n", sig);
}
#define STDIN_FILENO  0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2

UART_HandleTypeDef *gHuart;

void RetargetInit(UART_HandleTypeDef *huart) {
  gHuart = huart;
  for(uint8_t i = 0;i <= 32;i++)
    signal(i,sig_func);
  /* Disable I/O buffering for STDOUT stream, so that
   * chars are sent out as soon as they are printed. */
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