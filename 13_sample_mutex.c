#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

void	do_one_thing(int *);
void	do_another_thing(int *);
void	do_wrap_up(int, int);

int	g_r1, g_r2, g_r3;
pthread_mutex_t	g_r3_mutex = PTHREAD_MUTEX_INITIALIZER;

int	main(int argc, char *argv[])
{
	pthread_t	thread1, thread2;

	if (argc > 1)
		g_r3 = atoi(argv[1]);
	if (pthread_create(&thread2, NULL, (void *)do_another_thing, (void* )&g_r2) != 0)
	{
		perror("pthread_create");
		exit(1);
	}
	if (pthread_create(&thread1, NULL, (void *)do_one_thing, (void* )&g_r1) != 0)
	{
		perror("pthread_create");
		exit(1);
	}
	if (pthread_join(thread1, NULL) != 0)
	{
		perror("pthread_join");
		exit(1);
	}
	if (pthread_join(thread2, NULL) != 0)
	{
		perror("pthread_join");
		exit(1);
	}
	do_wrap_up(g_r1, g_r2);
	return (0);
}

void	do_one_thing(int *pnum_times)
{
	int	i, j, x;

	pthread_mutex_lock(&g_r3_mutex);
	if (g_r3 > 0)
	{
		x = g_r3;
		g_r3--;
	}
	else
		x = 1;
	printf("one_x:%d\n", x);
	pthread_mutex_unlock(&g_r3_mutex);
	i = 0;
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

	pthread_mutex_lock(&g_r3_mutex);
	if (g_r3 > 0)
	{
		x = g_r3;
		g_r3--;
	}
	else
		x = 1;
	pthread_mutex_unlock(&g_r3_mutex);
	printf("ano_x:%d\n", x);
	i = 0;
	while (i < 4)
	{
		printf("doing another thing\n");
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
