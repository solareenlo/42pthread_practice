#include <pthread.h>
#include <stdio.h>

void	do_one_thing(int *);
void	do_another_thing(int *);
void	do_wrap_up(int, int);

int	g_r1, g_r2;

int	main(void)
{
	pthread_t	thread1, thread2;

	pthread_create(&thread1, NULL, (void *)do_one_thing, (void *)&g_r1);
	pthread_create(&thread2, NULL, (void *)do_another_thing, (void *)&g_r2);
	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);
	do_wrap_up(g_r1, g_r2);
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
