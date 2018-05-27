/*
 * 
 * 
 * Author: dmliscinsky
 */



#include <stdlib.h>



/**
 * A context for performing inter-process communication (IPC) with a child process.
 * 
 * 
 */
typedef struct ipc_ctx {
	pid_t pid;// PID of the child

	int pipe_fd_in;// For stdin to the child process
	int pipe_fd_out;// For stdout from the child process
	int pipe_fd_err;// For stderr from the child process

//	TYPE read_timeout;// The amount of time to block while reading before timing out				//TODO
	unsigned int write_retry_count;

	int expect_prompt;// Expect a prompt output after the submitted command completes (1 = true, 0 = false)			//TODO ????? make a var here????
	int prompt_fd;// The FD to expect the prompt on. (i.e. 1 for stdout, 2 for stderr)
	char *prompt;																									//TODO ????? make a var here????

	//TODO what if there is more than 1 possible prompt ?????????????
	
} ipc_ctx_t;



#define DEFAULT_WRITE_RETRY_COUNT 20 // The nubmer of times to retry the write when write() returns 0



/**
 * Forks and starts a child process using execve(). Sets up all the file descriptors necessary to perform IPC
 * with the child process's stdin, stdout, and stderr. 
 * 
 * @param argv	The first value in argv determines the program to run. argv is passed directly to execve().
 * @param envp	envp is passed directly to execve(). 
 * 
 * @return Upon successful completion, a valid pointer to a dynamically allocated ipc_ctx struct shall be returned; 
 * otherwise, NULL shall be returned and errno set to indicate the error.
 */
ipc_ctx_t * ipc_fork(char * const *argv, char * const *envp);

/**
 * Writes a string of bytes to the child process.
 * 
 * @return 0 if the entire buffer was successfully written. Otherwise, returns the 
 * number of bytes at the end of the buffer that failed to be written.
 */
size_t ipc_write(ipc_ctx_t *ctx, const void *buf, size_t len);

/**
 * Not implemented.
 */
ssize_t ipc_read(ipc_ctx_t *ctx, void **buf, size_t len);



/**
 * Not implemented.
 */
int ipc_execute_command(ipc_ctx_t *ctx, const char *str);



/**
 * Not implemented.
 */
int wait_for_initial_prompt(ipc_ctx_t *ctx);

/**
 * Performs waitpid(pid, exit_code, 0).
 *
 * Stores the exit status of the process in exit_code if the process
 * terminated normally, otherwise exit_code retains the status value
 * set by waitpid().
 */
void waitpid_wexitstatus(pid_t pid, int *exit_code);











