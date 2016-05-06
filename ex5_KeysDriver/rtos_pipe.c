#include "rtos_pipe.h"


// RTOS pipes

unsigned int rtos_pipe_write(struct rtos_pipe *pipe, char *data, unsigned int num_bytes){
  int full = 0, idx;
  unsigned int i;
  
  for(i=0; i<num_bytes; i++){  
    idx = (pipe->end + 1)%pipe->size;
    if(idx == pipe->begin){
      full = 1;
      break;
    }  
    pipe->data[pipe->end] = data[i];
    pipe->end = idx;
  }
  return i;
}

unsigned int rtos_pipe_read(struct rtos_pipe *pipe, char *data, unsigned int num_bytes){
  unsigned int i;
  for(i=0; i<num_bytes; i++){
    if(pipe->begin == pipe->end){
      break;
    }
    data[i] = pipe->data[pipe->begin];
    pipe->begin = (pipe->begin+1)%pipe->size;
  }
  return i;
}


