
#include "circular_buffer.h"



void CB_init(circular_buffer* cb)
{        
    cb->size = CIRCULAR_BUFFER_SIZE; 
    cb->read_offset = 0;
    cb->write_offset = 0;
    cb->count = 0;  
    int i = 0;
    for  (i=0;i<CIRCULAR_BUFFER_SIZE;i++)
        cb->buffer[i].write_valid = 0x0; 
}

void CB_free(circular_buffer* cb)
{
    free(cb);
}

int CB_push(circular_buffer* cb, cb_buffer_struct data)
{
    if (cb == NULL || cb->buffer == NULL)
        return false;

    if (CB_full(cb)){
        //printf("warning: circular buffer is full. failed to push data to circular buffer. \n");
        return false;
    }
    else{
        //cb->buffer[write_offset] = data;
        //*(cb->buffer + cb->write_offset) = data;
        memcpy((void *)&(cb->buffer[cb->write_offset]), &data, sizeof(cb_buffer_struct));
        cb->buffer[cb->write_offset].write_valid = 0x1;
        cb->write_offset+=1;
        //cb->count+=1;
    }
    
    if ( cb->write_offset == cb->size - 1)
        cb->write_offset = 0;

    return true;
}

cb_buffer_struct CB_pop(circular_buffer* cb, int *buf_empty)
{    
    cb_buffer_struct data;
    if (cb == NULL || cb->buffer == NULL)
        strcpy( data.buffer, "\0" );

    if(CB_empty(cb)){
        *buf_empty = 1;
        //printf("warning: circular buffer is empty. waiting for data\n");
        //printf(".");
    }
    else{
        *buf_empty = 0;
        //data =  cb->buffer[read_offset];
        //data = *(cb->buffer + cb->read_offset);
        memcpy(&data, (const void *)&(cb->buffer[cb->read_offset]), sizeof(cb_buffer_struct));
        cb->buffer[cb->read_offset].write_valid = 0x0;
        cb->read_offset += 1;
        if (cb->read_offset == cb->size - 1)
            cb->read_offset = 0;
        //cb->count -= 1;
    }
    return data;
}

int CB_full(circular_buffer* cb)
{   /*
    if(cb->count == cb->size){
        return 1;
    }
    */
    if(cb->buffer[cb->write_offset].write_valid == 0x1){
        return 1;
    }
    return 0;    
}

int CB_empty(circular_buffer* cb)
{   /*
    if(cb->count == 0){
        return 1;
    }*/
    if(cb->buffer[cb->read_offset].write_valid == 0x0){
        return 1;
    }
    return 0;    
}

