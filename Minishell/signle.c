#include "minishell.h"

void	ft_signal_back_slash(int signum)
{
	(void)signum;
	ft_putstr_fd("\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	exit(131);
}

void	ft_signal_handler_herdoc(int signum)
{
	if (signum == SIGINT)
	{
		write(1, "\n", 1);
		exit(130); // Exit if SIGINT is received
	}
}
void	ft_signal_handler_cmd(int signum)
{
	(void)signum;
	ft_putstr_fd("\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	exit(130);
}