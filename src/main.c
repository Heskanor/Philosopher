/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashite <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/05 21:14:53 by ashite            #+#    #+#             */
/*   Updated: 2021/10/08 19:08:12 by ashite           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void	printer(char *str, int index, unsigned int sleeper)
{
	unsigned int	t;

	pthread_mutex_lock(&g_ph.print_mutex);
	if (str[3] == 'e' || str[0] == 'd')
		pthread_mutex_lock(&g_ph.eat_mutex[index]);
	t = time_diff(g_ph.base);
	printf("%u ms Philosopher[%d] %s\n", t, index + 1, str);
	if (str[0] != 'd')
		pthread_mutex_unlock(&g_ph.print_mutex);
	if (sleeper > 0)
		mysleep(sleeper);
	if (str[3] == 'e' || str[0] == 'd')
		pthread_mutex_unlock(&g_ph.eat_mutex[index]);
}

void	*routine(void *arg)
{
	int	index;
	int	next;

	index = *(int *)arg;
	next = index + 1;
	if (index == (g_ph.n_philo - 1))
		next = 0;
	g_ph.ph_meals[index] = 0;
	while (g_ph.hunger < g_ph.n_philo)
	{
		pthread_mutex_lock(&g_ph.forks[index]);
		printer("has taken a fork", index, 0);
		pthread_mutex_lock(&g_ph.forks[next]);
		printer("has taken a fork", index, 0);
		gettimeofday(&g_ph.before[index], NULL);
		printer("is eating", index, g_ph.t_eat);
		pthread_mutex_unlock(&g_ph.forks[index]);
		pthread_mutex_unlock(&g_ph.forks[next]);
		g_ph.ph_meals[index]++;
		printer("is sleeping", index, g_ph.t_sleep);
		printer("is thinking", index, 0);
	}
	return (NULL);
}

int	breaker(pthread_t *th)
{
	int	i;

	while (1)
	{
		g_ph.hunger = 0;
		i = 0;
		while (i < g_ph.n_philo)
		{
			if (g_ph.t_die <= time_diff(g_ph.before[i]))
				return (death_checker(th, i));
			else if (g_ph.ph_meals[i] >= g_ph.n_meals && g_ph.n_meals != -1)
				g_ph.hunger++;
			if (g_ph.hunger == g_ph.n_philo)
				return (-1);
			i++;
		}
		usleep(500);
	}
	return (1);
}

int	ft_thread(void)
{
	int			i;
	pthread_t	*th;
	int			e;

	i = 0;
	th = (pthread_t *)malloc(sizeof(pthread_t) * (g_ph.n_philo));
	gettimeofday(&g_ph.base, NULL);
	while (i < g_ph.n_philo)
	{
		g_ph.before[i].tv_sec = g_ph.base.tv_sec;
		g_ph.before[i].tv_usec = g_ph.base.tv_usec;
		if (pthread_create(&th[i], NULL, &routine, &i) != 0)
			printf("Failed to create thread");
		usleep(100);
		i++;
	}
	i = 0;
	e = breaker(th);
	while (i < g_ph.n_philo)
	{
		pthread_join(th[i], NULL);
		i++;
	}
	free(th);
	return (e);
}

int	main(int argc, char **argv)
{
	if (((argc == 6 || argc == 5) && !inputs_checker(argv, 0)
			&& !initializer(argv, argc)) && ft_thread() == -1)
		freeta();
	return (0);
}
