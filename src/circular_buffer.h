#ifndef CIRCULAR_BUFFER_H_   /* Include guard for header*/
#define CIRCULAR_BUFFER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <arpa/inet.h>


#ifndef CIRCULAR_BUFFER_SIZE   /* Include guard for buffer size*/
#define CIRCULAR_BUFFER_SIZE 1024
#endif

//typedef int bool;
#define true 1
#define false 0

typedef struct{
	unsigned int rule_id;
    char buffer[10000];
    char write_valid;  
    int packet_data;  
    int lcore_id;
    int stream_id; 
    char timeStamp[100];
    char src_ip[INET6_ADDRSTRLEN+1];
    char dst_ip[INET6_ADDRSTRLEN+1];
    int src_port;
    int dst_port;  
    int direction; 
    int stream_size; 
    int l7_error;
    int l7_prot;
    int pkt_content_size;
    int http_content_size;
    int http_header_size;
    int match_possition;
    int door_rule_id;
    //char rule; // know by the management layer
    char saved_result[100];
    //stream_id;
    //time_stamp;
    char pkt_content[9000];
}cb_buffer_struct;

typedef char buftype;

// circular buffer structure
typedef struct
{	
	cb_buffer_struct buffer[CIRCULAR_BUFFER_SIZE];             /* circular buffer data structure to store data */
     //buftype buffer[CIRCULAR_BUFFER_SIZE];				/* initalize circular buffer */
     signed long long read_offset;				/* read offset of buffer */
     signed long long write_offset;				/* write offset of buffer */
     signed long long  count;						/* amount of data in buffer */
     signed long long  size;						/* buffer size */

 } circular_buffer;

void CB_init(circular_buffer* cb);  		 	/* initalize circular buffer */
void CB_free(circular_buffer* cb);				/* free circular buffer */
int CB_push(circular_buffer* cb, cb_buffer_struct data);	/* push data into circular buffer */
cb_buffer_struct CB_pop(circular_buffer* cb, int *buf_empty);				/* pop data from circular buffer */
int CB_full(circular_buffer* cb);				/* check for buffer full condition */
int CB_empty(circular_buffer* cb);				/* check for buffer empty condition */

#endif // CIRCULAR_BUFFER_H_