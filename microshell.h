#include <stdlib.h> //malloc, free
#include <unistd.h> //dup, dup2, close, write, exit, chdir, execve, pipe
#include <sys/wait.h> // wait, waitpid
#include <signal.h> //kill, signal
#include <string.h> //strncmp, strcmp
#define FATAL_ERR 2

enum e_oper {
	o_pipe,
	o_scln

}; 

typedef struct	s_cmd	{
	int		start;
	int		end;
	struct s_cmd	*next;
} t_cmd;

typedef struct	s_chain {
	t_cmd 		*start_cmd;
	struct s_chain	*next;
} t_chain;


void write_err(const char *s);
void write_err2(const char *s1, const char *s2);
void	fatal(void);
void	redir(int *fd, int type);
void save_restore(void);
int	ft_strlen(char *s);
t_chain	*new_chain(void);
t_cmd	*new_cmd(void);
void	clean_chain(t_chain *chain);

int change_dir(t_cmd *cmd, char *argv[]);

t_chain *parser(char **beg, char **end);
