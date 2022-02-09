#include "microshell.h"

int exec_cmd(t_cmd *cmd, char *argv[], char *env[])
{
	char *path;

	path = argv[cmd->start + 1];
	argv[cmd->end + 2] = NULL;
	execve(path, argv + cmd->start + 1, env);
	write_err2("error: cannot execute ", path);
	exit (EXIT_FAILURE);
}



int exec_pipe(t_cmd *start_cmd, char *argv[], char *env[])
{
	pid_t	pid;
	int	fd[2];
	int	p;
	t_cmd	*cmd;
	int	status;

	p = 1;
	cmd = start_cmd;
	if (cmd->next)
		save_restore();
	while (cmd)
	{
		if (cmd->next && pipe(fd) == -1)
			fatal();
		pid = fork();
		if (pid < 0)
			fatal();
		else if (pid == 0)
		{
			signal(SIGINT, SIG_DFL);
			signal(SIGHUP, SIG_DFL);
			signal(SIGQUIT, SIG_DFL);
			redir(fd, STDOUT_FILENO);
			exec_cmd(cmd, argv, env);
		}
		else
		{
			p++;
			if (cmd->next)
				redir(fd, STDIN_FILENO);
			cmd = cmd->next;
		}
	}
	if (p > 1)
		save_restore();
	while (p)
	{
		waitpid(-1, &status, 0);
		p--;
	}
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (0);
}




int exec_chain(t_chain *chain, char *argv[], char *env[])
{
	int ret;
	t_cmd *cmd;

	ret = 0;
	while (chain)
	{
		cmd = chain->start_cmd;
		if (cmd)
		{
			if (!strncmp(argv[cmd->start + 1], "cd", 3))
				ret = change_dir(cmd, argv);
			else
				ret = exec_pipe(cmd, argv, env);
		}
		chain = chain->next;
	}
	return ret;
}



int main(int argc, char *argv[], char *env[])
{
	t_chain *chain;
	int	ret;

	signal(SIGINT, SIG_IGN);
	signal(SIGHUP, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	if (argv + 1 > argv + argc - 1)
		return (0);
	chain = parser(argv + 1, argv + argc - 1);
	ret = exec_chain(chain, argv, env);
	clean_chain(chain);
	return (ret);
}
