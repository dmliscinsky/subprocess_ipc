/*
 * 
 * 
 * Author: dmliscinsky
 */



#include <unistd.h>
#include <sysexits.h>
#include <sys/wait.h>

#include "ipc.h"
#include "debug.h"



ipc_ctx_t * ipc_fork(char * const *argv, char * const *envp) {
	
	ipc_ctx_t *ctx = NULL;
	int pipe_fd_in[2];// For stdin to the child process
	int pipe_fd_out[2];// For stdout from the child process
	int pipe_fd_err[2];// For stderr from the child process


	// Allocate memory for the IPC struct
	if ( !(ctx = (ipc_ctx_t *)malloc(sizeof(ipc_ctx_t))) ) {
		DEBUG_perror("malloc");
		return NULL;
	}

	// Create the pipes and fork to share it
	if (pipe(pipe_fd_in) < 0 || pipe(pipe_fd_out) < 0 || pipe(pipe_fd_err) < 0) {
		DEBUG_perror("pipe");	
		free(ctx);
		return NULL;
	}

	if ((ctx->pid = fork()) < 0) {
		DEBUG_perror("fork");
		free(ctx);
		return NULL;
	}
	// If child process
	else if (ctx->pid == 0) {
		
		// Set up pipe for child's STDIN
		close(pipe_fd_in[1]);

		if (dup2(pipe_fd_in[0], STDIN_FILENO) < 0) {
			DEBUG_perror("dup2");
		}

		close(pipe_fd_in[0]);

		// Set up pipe for child's STDOUT
		close(pipe_fd_out[0]);
		close(pipe_fd_err[0]);

		if (dup2(pipe_fd_out[1], STDOUT_FILENO) < 0) {
			DEBUG_perror("dup2");
		}

		if (dup2(pipe_fd_err[1], STDERR_FILENO) < 0) {
			DEBUG_perror("dup2");
		}

		close(pipe_fd_out[1]);
		close(pipe_fd_err[1]);

		execve(argv[0], argv, envp);
		DEBUG_perror("exec");
		exit(EX_OSERR);//TODO not necessarily an error... (-x perms)
	}
	
	// Close unneeded pipes to child
	close(pipe_fd_in[0]);
	close(pipe_fd_out[1]);
	close(pipe_fd_err[1]);
	



	//TODO check if child proc creation failed or had an error before exec




	// Store FDs for talking with child
	ctx->pipe_fd_in  = pipe_fd_in[1];
	ctx->pipe_fd_out = pipe_fd_out[0];
	ctx->pipe_fd_err = pipe_fd_err[0];

	// Initialize remaining struct fields to defualt values
//	ctx->read_timeout = ???????????;
	ctx->write_retry_count = DEFAULT_WRITE_RETRY_COUNT;
	ctx->expect_prompt = 0;
	ctx->prompt_fd = -1;
	ctx->prompt = NULL;

	return ctx;
}
/*
???? func(???????) {


	close(pipe_fd_in[1]);
	close(pipe_fd_out[0]);
	close(pipe_fd_err[0]);


	int exit_code;
	waitpid_wexitstatus(pid, &exit_code);
	while (wait(NULL) > 0); // Reap all child processes

}
*/
int wait_for_initial_prompt(ipc_ctx_t *ctx) {



}

/**
 * Writes a string of bytes to the child process.
 * 
 * @return 0 if the entire buffer was successfully written. Otherwise, returns the 
 * number of bytes at the end of the buffer that failed to be written.
 */
size_t ipc_write(ipc_ctx_t *ctx, const void *buf, size_t len) {

	ssize_t offset = 0;
	ssize_t written;
	unsigned int wr_retry_count = ctx->write_retry_count;

	do {
		written = write(ctx->pipe_fd_in, (char *)buf + offset, len);

		// If write failed
		if (written < 0) {
			wr_retry_count--;
		}
		else {
			offset += written;
			len -= written;

			// Reset retry count
			wr_retry_count = ctx->write_retry_count;
		}

		// Continue writing until the entire buffer is written or too many failed writes occur
	} while (len > 0 && wr_retry_count > 0);
	
	return len;
}

ssize_t ipc_read(ipc_ctx_t *ctx, void **buf, size_t len) {

}

int ipc_execute_command(ipc_ctx_t *ctx, const char *str) {
	/*
	size_t len = strlen(str);
	
	// Do nothing if string is empty
	if (len == 0) {
		return ? ;
	}

	// Check if string already has a terminating '\n'
	if () {
		// Replace trailing '\0' with '\n'
		
	}



	//NOTE str needs to have terminating '\n' ('\0' does not matter....)

	*/
}








/**
 * Performs waitpid(pid, exit_code, 0).
 *
 * Stores the exit status of the process in exit_code if the process
 * terminated normally, otherwise exit_code retains the status value
 * set by waitpid().
 */
void waitpid_wexitstatus(pid_t pid, int *exit_code) {

	waitpid(pid, exit_code, 0);

	/* If process terminated normally, get exit code. */
	if (WIFEXITED(*exit_code)) {
		*exit_code = WEXITSTATUS(*exit_code);
	}
}