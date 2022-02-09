#include "microshell.h"

void write_err(const char *s)
{
	write(STDERR_FILENO, s, ft_strlen((char *)s));
}

void write_err2(const char *s1, const char *s2)
{
	write_err(s1);
	write_err(s2);
	write(STDERR_FILENO, "\n", 1);
}

void	fatal(void)
{
	write_err("error: fatal\n");
	exit(FATAL_ERR);
}

void	redir(int *fd, int type)
{
	if (type == STDIN_FILENO || type == STDOUT_FILENO)
	{
		dup2(fd[type], type);
		close(fd[0]);
		close(fd[1]);
	}

}

void save_restore(void)
{
	static int sav[2] = {0, 0};

	if (!sav[0] && !sav[1])
	{
		sav[STDIN_FILENO] = dup(STDIN_FILENO);
		sav[STDOUT_FILENO] = dup(STDOUT_FILENO);
	}
	else
	{
		dup2(sav[STDIN_FILENO], STDIN_FILENO);
		close(sav[STDIN_FILENO]);
		dup2(sav[STDOUT_FILENO], STDOUT_FILENO);
		close(sav[STDOUT_FILENO]);
		sav[STDOUT_FILENO] = 0;
		sav[STDIN_FILENO] = 0;
	}

}

int	ft_strlen(char *s)
{
	int i;

	i = 0;
	while (s[i])
		i++;
	return i;
}

t_chain	*new_chain(void)
{
	t_chain	*res;

	res = malloc(sizeof(t_chain));
	if (!res)
		fatal();
	res->start_cmd = NULL;
	res->next = NULL;
	return (res);
}

t_cmd	*new_cmd(void)
{
	t_cmd *res;

	res = malloc(sizeof(t_cmd));
	if (!res)
		fatal();
	res->start = 0;
	res->end = 0;
	res->next = NULL;
	return res;
}

void	clean_chain(t_chain *chain)
{
	t_cmd *start;
	void *tmp;

	while (chain)
	{
		start = chain->start_cmd;
		while (start)
		{
			tmp = start;			
			start = start->next;
			free(tmp);
		}
		tmp = chain;
		chain = chain->next;
		free(tmp);
	}
}

