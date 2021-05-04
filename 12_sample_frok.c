#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void	do_one_thing(int *);
void	do_another_thing(int *);
void	do_wrap_up(int, int);

int	g_shared_mem_id;
int	*g_shared_mem_ptr;
int	*g_r1p;
int	*g_r2p;

int	main(void)
{
	pid_t	child1_pid, child2_pid;

	g_shared_mem_id = shmget(IPC_PRIVATE, 2 * sizeof(int), 0660);
	if (g_shared_mem_id == -1)
		perror("shmget"), exit(1);
	g_shared_mem_ptr = (int *)shmat(g_shared_mem_id, NULL, 0);
	if (g_shared_mem_ptr == (void *)-1)
		perror("shmat failed"), exit(1);
	g_r1p = g_shared_mem_ptr;
	g_r2p = g_shared_mem_ptr + 1;
	(*g_r1p) = (*g_r2p) = 0;
	if ((child1_pid = fork()) == 0)
	{
		do_one_thing(g_r1p);
		exit(0);
	}
	else if (child1_pid == -1)
	{
		perror("frok");
		exit(1);
	}
	if ((child2_pid = fork()) == 0)
	{
		do_another_thing(g_r2p);
		exit(0);
	}
	else if (child2_pid == -1)
	{
		perror("frok");
		exit(1);
	}
	if ((waitpid(child1_pid, NULL, 0)) == -1)
	{
		perror("waitpid");
		exit(1);
	}
	if ((waitpid(child2_pid, NULL, 0)) == -1)
	{
		perror("waitpid");
		exit(1);
	}
	do_wrap_up(*g_r1p, *g_r2p);
	return(0);
}

void	do_one_thing(int *pnum_times)
{
	int	i, j, x;

	i = x = 0;
	while (i < 4)
	{
		printf("doing one thing\n");
		j = 0;
		while (j < 10000)
		{
			x += i;
			j++;
		}
		(*pnum_times)++;
		i++;
	}
}

void	do_another_thing(int *pnum_times)
{
	int	i, j, x;

	i = x = 0;
	while (i < 4)
	{
		printf("doing onother thing\n");
		j = 0;
		while (j < 10000)
		{
			x += i;
			j++;
		}
		(*pnum_times)++;
		i++;
	}
}

void	do_wrap_up(int one_times, int another_times)
{
	int	total;

	total = one_times + another_times;
	printf("All done, one thing %d, another %d for a total %d\n",
			one_times, another_times, total);
}
