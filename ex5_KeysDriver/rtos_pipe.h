#ifndef RTOS_PIPE_H_INCLUDED
#define RTOS_PIPE_H_INCLUDED


// RTOS pipe stuff
struct rtos_pipe{
  unsigned int begin;
  unsigned int end;
  unsigned int size;
  char *data;
};

extern unsigned int rtos_pipe_read(struct rtos_pipe *pipe, char *data, unsigned int num_bytes); 
extern unsigned int rtos_pipe_write(struct rtos_pipe *pipe, char *data, unsigned int num_bytes);



#endif
