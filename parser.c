#include "microshell.h"

char	**find_oper(char **beg, char **end, int type)
{
	char **cur;
	char *ops[3] = {"|", ";", NULL};

	cur = beg;
	while (cur <= end)
	{
		if (!strncmp(*cur, ops[type], 2))
			return (cur);
		cur++;
	}
	return (end + 1);
}

t_cmd	*get_pipe(char **beg, char **end, char **orig)
{
	t_cmd *start;
	t_cmd *cmd;
	char **del;
	char **cur;

	cmd = new_cmd();
	start = cmd;
	cur = beg;
	while (1)
	{
		del = find_oper(cur, end, o_pipe);
		cmd->start =(int)(cur - orig);
		cmd->end = (int)(del - 1 - orig);
		cur = del + 1;
		if (cur > end)
			break ; 
		cmd->next = new_cmd();
		cmd = cmd->next;
	}
	return start;
}


t_chain *parser(char **beg, char **end)
{
	t_chain *start;
	t_chain *chain;
	char **del;
	char **cur;

	chain = new_chain();
	start = chain;
	cur = beg;
	while (1)
	{
		del = find_oper(cur, end, o_scln);
		chain->start_cmd = get_pipe(cur, del - 1, beg);
		cur = del + 1;
		if (cur > end)
			break ; 
		chain->next = new_chain();
		chain = chain->next;
	}
	return start;
}
