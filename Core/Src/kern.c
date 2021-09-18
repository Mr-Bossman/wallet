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
#include "app_fatfs.h"
#include "Cstring_func.h"
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "indexed_array.h"

#define STDIN_FILENO 0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2

static struct alloc filenums;

_Unwind_Reason_Code unwind_backtrace_callback(struct _Unwind_Context *context, void *arg)
{
  uintptr_t pc = _Unwind_GetIP(context);
  printf("0x%x\n", pc);
  return _URC_NO_REASON;
}

void print_trace(void)
{
  _Unwind_Reason_Code rc = _Unwind_Backtrace(unwind_backtrace_callback, 0);
  printf("code %d\n", rc);
}

void sig_func(int sig)
{
  printf("exit %d\n", sig);
}

FATFS FatFs;

UART_HandleTypeDef *gHuart;
uint8_t stdinbuf[400] = {0};

void RetargetInit(UART_HandleTypeDef *huart)
{
  gHuart = huart;
  for (uint8_t i = 0; i <= 32; i++)
    signal(i, sig_func);
  setvbuf(stdout, NULL, _IONBF, 0);
  filenums = NewAlloc();
  HAL_UART_Receive_DMA(gHuart, stdinbuf, sizeof(stdinbuf));
}
int mount(){
  FRESULT fres;
  fres = f_mount(&FatFs, "", 1);
  if (fres != FR_OK)
    printf("f_mount error (%i)\r\n", fres);
  return fres;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  stdinbuf[0] = stdinbuf[sizeof(stdinbuf) - 1];
  memset(stdinbuf + 1, 0, sizeof(stdinbuf) - 1);
  HAL_UART_Receive_DMA(gHuart, stdinbuf + 1, sizeof(stdinbuf) - 1);
}

FATFS *getFat()
{
  return &FatFs;
}

int _isatty(int fd)
{
  if (fd >= STDIN_FILENO && fd <= STDERR_FILENO)
    return 1;

  errno = EBADF;
  return 0;
}

int _write(int fd, char *ptr, int len)
{
  HAL_StatusTypeDef hstatus = HAL_OK;

  if (fd == STDOUT_FILENO || fd == STDERR_FILENO)
  {
    for (int i = 0; i < len; i++)
    {
      if (*(ptr + i) == '\n')
      {
        hstatus |= HAL_UART_Transmit(gHuart, (uint8_t *)ptr, i, HAL_MAX_DELAY);
        uint8_t n = '\r';
        hstatus |= HAL_UART_Transmit(gHuart, &n, 1, HAL_MAX_DELAY);
        len -= i;
        i = 0;
        ptr += i;
      }
    }
    hstatus |= HAL_UART_Transmit(gHuart, (uint8_t *)ptr, len, HAL_MAX_DELAY);
    if (hstatus == HAL_OK)
      return len;
    else
      return EIO;
  }
  else if (fd - 3 < filenums.size)
  {
    FRESULT fr;
    unsigned int l;
    fr = f_write(filenums.get(&filenums, fd - 3), ptr, len, &l);
  }
  errno = EBADF;
  return -1;
}

int _close(int fd)
{
  if (fd >= STDIN_FILENO && fd <= STDERR_FILENO)
    return 0;
  if (fd - 3 < filenums.size)
  {
    FRESULT fr;
    fr = f_close(filenums.get(&filenums, fd - 3));
    filenums.del(&filenums, fd - 3);
  }

  errno = EBADF;
  return -1;
}

int _lseek(int fd, int ptr, int dir)
{
  if (fd - 3 < filenums.size)
  {
    FRESULT fr;
    unsigned int l;
    // need to do stuff with wence https://man7.org/linux/man-pages/man2/lseek.2.html
    //fr = f_lseek(filenums.get(&filenums,fd-3)),ptr);
  }

  errno = EBADF;
  return -1;
}

static size_t read_index = 0;
int _read(int fd, char *ptr, int len)
{
  if (fd == STDIN_FILENO)
  {
    size_t cpylen = 0;
    while (!cpylen || cpylen == read_index)
      cpylen = strnlen(stdinbuf, (len < sizeof(stdinbuf)) ? len : sizeof(stdinbuf));
    if (cpylen < read_index)
      read_index = 0;
    memcpy(ptr, stdinbuf + read_index, cpylen - read_index);
    size_t tmp = cpylen - read_index;
    read_index = cpylen;
    return tmp;
  }
  else if (fd - 3 < filenums.size)
  {
    FRESULT fr;
    unsigned int l;
    fr = f_read(filenums.get(&filenums, fd - 3), ptr, len, &l);
  }
  errno = EBADF;
  return -1;
}

int _fstat(int fd, struct stat *st)
{
  if (fd >= STDIN_FILENO && fd <= STDERR_FILENO)
  {
    st->st_mode = S_IFCHR;
    return 0;
  }
  //http://elm-chan.org/fsw/ff/doc/stat.html https://linux.die.net/man/2/fstat
  errno = EBADF;
  return 0;
}

int _open(const char *name, int flags, int mode)
{
  FRESULT fr;
  size_t ind = filenums.add(&filenums);
  fr = f_open(filenums.get(&filenums, ind), name, FA_WRITE | FA_CREATE_ALWAYS);

  if (fr)
    return -1;
  return ind + 3;
}

/* gcc -Wl,--wrap=free -Wl,--wrap=realloc */

void *__real_realloc(void *, size_t);

//zero memory even on error...
void *__wrap_realloc(void *ptr, size_t sz)
{
  memset(ptr, 0, malloc_usable_size(ptr));
  void *ret = __real_realloc(ptr, sz);
  if (!ret)
    free(ptr);
  return ret;
}

void __real_free(void *);

void __wrap_free(void *ptr)
{
  memset(ptr, 0, malloc_usable_size(ptr));
  __real_free(ptr);
  ptr = NULL;
}