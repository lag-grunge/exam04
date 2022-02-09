#include "microshell.h"

int change_dir(t_cmd *cmd, char *argv[])
{
	if (cmd->start == cmd->end || 
		cmd->end > cmd->start + 1 || \
		!strncmp(argv[cmd->end + 1], "-", 2))
	{
		write_err("error: cd: bad arguments\n");
		return (1);
	}
	else if (chdir(argv[cmd->end + 1]) == -1)
	{
		write_err2("error: cd: cannot change directory to ", argv[cmd->end + 1]);
		return (1);
	}
	return (0);
}
