#ifndef __dbg_h__
#define __dbg_h__

#include <stdio.h>
#include <errno.h>
#include <string.h>

// RED \033[22;31m
// GREEN \033[22;32m
// BROWN \033[22;33m
// BLUE \033[22;34m
// MAGENTA \033[22;35m
// CYAN \033[22;36m
// GRAY \033[22;37m
// DGRAY \033[01;30m
// LRED \033[01;31m
// LGREEN \033[01;32m
// YELLOW \033[00;33m
// LBLUE \033[01;34m
// LMAGENTA \033[01;35m
// LCYAN \033[01;36m
// WHITE \033[01;37m
// NORM \033[0m

#ifdef VERBOSE_DEBUG

#ifdef DEBUG
#define debug(M, ...) fprintf(stderr, "[\033[22;34mDEBUG\033[0m] (\033[22;32m%s\033[0m :\033[22;32m%3d\033[0m : \033[22;32m%s\033[0m) " M "\n", __FILE__, __LINE__, __PRETTY_FUNCTION__, ##__VA_ARGS__)
#else
#define debug(M, ...) 
#endif

#define clean_errno() (errno == 0 ? "None" : strerror(errno))

#define log_err(M, ...) fprintf(stderr, "[\033[22;31mERROR\x1B[0m] (\033[22;32m%s\033[0m :\033[22;32m%3d\033[0m : \033[22;32m%s\033[0m) [\033[22;34merrno: %s\033[0m] " M "\n", __FILE__, __LINE__, __PRETTY_FUNCTION__, clean_errno(), ##__VA_ARGS__)

#define log_warn(M, ...) fprintf(stderr, "[\033[22;33mWARN\x1B[0m]  (\033[22;32m%s\033[0m :\033[22;32m%3d\033[0m : \033[22;32m%s\033[0m) [\033[22;34merrno: %s\033[0m] " M "\n", __FILE__, __LINE__, __PRETTY_FUNCTION__, clean_errno(), ##__VA_ARGS__)

#define log_info(M, ...) fprintf(stderr, "[\033[22;32mINFO\x1B[0m]  (\033[22;32m%s\033[0m :\033[22;32m%3d\033[0m : \033[22;32m%s\033[0m) " M "\n", __FILE__, __LINE__, __PRETTY_FUNCTION__, ##__VA_ARGS__)

#define check(A, M, ...) if(!(A)) { log_err(M, ##__VA_ARGS__); errno=0; goto error; }

#define sentinel(M, ...) { log_err(M, ##__VA_ARGS__); errno=0; goto error; }

#define check_mem(A) check((A), "Out of memory.");

#define check_debug(A, M, ...) if(!(A)) { debug(M, ##__VA_ARGS__); errno=0; goto error; }

#else

#ifdef DEBUG
#define debug(M, ...) fprintf(stderr, "[\033[22;34mDEBUG\033[0m] " M "\n", ##__VA_ARGS__)
#else
#define debug(M, ...) 
#endif

#define clean_errno() (errno == 0 ? "None" : strerror(errno))

#define log_err(M, ...) fprintf(stderr, "[\033[22;31mERROR\x1B[0m] [\033[22;34merrno: %s\033[0m] " M "\n", clean_errno(), ##__VA_ARGS__)

#define log_warn(M, ...) fprintf(stderr, "[\033[22;33mWARN\x1B[0m] [\033[22;34merrno: %s\033[0m] " M "\n", clean_errno(), ##__VA_ARGS__)

#define log_info(M, ...) fprintf(stderr, "[\033[22;32mINFO\x1B[0m] " M "\n", ##__VA_ARGS__)

#define check(A, M, ...) if(!(A)) { log_err(M, ##__VA_ARGS__); errno=0; goto error; }

#define sentinel(M, ...) { log_err(M, ##__VA_ARGS__); errno=0; goto error; }

#define check_mem(A) check((A), "Out of memory.");

#define check_debug(A, M, ...) if(!(A)) { debug(M, ##__VA_ARGS__); errno=0; goto error; }

#endif

#define print_bits(c) printf("0b"); for(int i = ((sizeof(c)*8)-1); i >= 0; i--) printf("%x", (c>>i)&1); printf("\n");

#endif 
