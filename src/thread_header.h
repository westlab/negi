
#ifndef THEREAD_HEADER_H_   /* Include guard for header*/
#define THEREAD_HEADER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <arpa/inet.h>
#include <pthread.h> 
#include <time.h>

#include "circular_buffer.h"
#include "shared_memory_support.h"

#define MAX_THREADS 10

#define MEASURE_TIME 0 

extern char *thread_mmap_buf;
extern circular_buffer **cb_threads;

#endif