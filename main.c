/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashite <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/05 21:14:53 by ashite            #+#    #+#             */
/*   Updated: 2021/10/05 21:15:01 by ashite           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>

typedef struct ph_s{
	int					n_philo;
	int					t_die;
	int					t_eat;
	int					t_sleep;
	struct timeval		base;
	int					n_meals;
	struct timeval		*before;
	int					death;
	int					hunger;
	int					*ph_meals;
	pthread_mutex_t		*forks;
	pthread_mutex_t		print_mutex;
}						t_ph;

t_ph	g_ph;

int	is_degit(char *s)
{
	int	i;

	i = 0;
	if (s == NULL)
		return (2);
	while (s[i] != '\0')
	{
		if (s[i] < '0' || s[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

int	inputs_checker(char **inputs, int c)
{
	int	i;

	i = 1;
	if (c == 0)
	{
		while (i < 6 && is_degit(inputs[i]))
			i++;
		if (i != 6)
			return (1);
		return (0);
	}
	else if ((g_ph.n_philo <= 0 || g_ph.n_philo > 200)
		|| g_ph.t_die < 60 || g_ph.t_eat < 60
		|| g_ph.t_sleep < 60 || (g_ph.n_meals <= 0 && g_ph.n_meals != -1))
		return (1);
	return (0);
}

int	ft_atoi(const char *str)
{
	long long	nbr;
	int			i;
	int			s;

	i = 0;
	nbr = 0;
	s = 1;
	while (str[i] == 32 || (str[i] < 14 && str[i] > 8))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i++] == '-')
			s = -1;
	}
	while (str[i] != '\0' && str[i] >= '0' && str[i] <= '9')
	{
		nbr = nbr * 10 + (str[i++] - '0');
		if (nbr > 4294967295)
		{
			return (-1);
		}
	}
	return (nbr * s);
}

void	mutex_constractor(pthread_mutex_t *mutex)
{
	int	i;
	int	forks;

	i = 0;
	forks = g_ph.n_philo;
	pthread_mutex_init(&g_ph.print_mutex, NULL);
	while (i < forks)
	{
		pthread_mutex_init(&mutex[i], NULL);
		i++;
	}
}

int	initializer(char **inputs, int m)
{
	g_ph.death = 0;
	g_ph.hunger = 0;
	g_ph.n_philo = ft_atoi(inputs[1]);
	g_ph.t_die = ft_atoi(inputs[2]);
	g_ph.t_eat = ft_atoi(inputs[3]);
	g_ph.t_sleep = ft_atoi(inputs[4]);
	g_ph.n_meals = -1;
	if (m == 6)
		g_ph.n_meals = ft_atoi(inputs[5]);
	if (inputs_checker(inputs, 1))
		return (1);
	if (g_ph.n_philo == 1)
		g_ph.forks = malloc(sizeof(pthread_mutex_t) * g_ph.n_philo + 1);
	else
		g_ph.forks = malloc(sizeof(pthread_mutex_t) * g_ph.n_philo);
	g_ph.ph_meals = malloc(sizeof(int) * g_ph.n_philo);
	g_ph.before = malloc(sizeof(struct timeval) * g_ph.n_philo);
	mutex_constractor(g_ph.forks);
	return (0);
}

int	time_diff(struct timeval x)
{
	int				x_ms;
	int				y_ms;
	int				diff;
	struct timeval	y;

	gettimeofday(&y, NULL);
	x_ms = (int)x.tv_sec * 1000 + (int)x.tv_usec / 1000;
	y_ms = (int)y.tv_sec * 1000 + (int)y.tv_usec / 1000;
	diff = y_ms - x_ms;
	return (diff);
}

void	printer(char *s, int index, int sleeper)
{
	int	t;

	t = time_diff(g_ph.base);
	pthread_mutex_lock(&g_ph.print_mutex);
	if (s[0] == 'd')
		printf("%d ms Philosopher[%d] died\n", t, index + 1);
	else if (s[0] == 'f')
		printf("%d ms Philosopher[%d] has taken a 2end fork\n", t, index + 1);
	else if (s[0] == 'F')
		printf("%d ms Philosopher[%d] has taken a 1st fork\n", t, index + 1);
	else if (s[0] == 'e')
		printf("%d ms Philosopher[%d] is eating\n", t, index + 1);
	else if (s[0] == 's')
		printf("%d ms Philosopher[%d] is sleeping\n", t, index + 1);
	else if (s[0] == 't')
		printf("%d ms Philosopher[%d] is thinking\n", t, index + 1);
	pthread_mutex_unlock(&g_ph.print_mutex);
	if (s[0] == 'e' || s[0] == 's')
		usleep(sleeper * 1000);
}

void	*routine(void *arg)
{
	int	index;
	int	next;

	index = *(int *)arg;
	next = index + 1;
	g_ph.ph_meals[index] = 0;
	if (index == (g_ph.n_philo - 1) && index != 0)
		next = 0;
	while (g_ph.hunger < g_ph.n_philo)
	{
		pthread_mutex_lock(&g_ph.forks[index]);
		printer("Fork", index, 0);
		pthread_mutex_lock(&g_ph.forks[next]);
		printer("fork", index, 0);
		printer("eat", index, g_ph.t_eat);
		gettimeofday(&g_ph.before[index], NULL);
		pthread_mutex_unlock(&g_ph.forks[index]);
		pthread_mutex_unlock(&g_ph.forks[next]);
		g_ph.ph_meals[index]++;
		printer("sleep", index, g_ph.t_sleep);
		printer("think", index, 0);
	}
	return (NULL);
}

int	death_checker(pthread_t *th, int i)
{
	int	j;

	j = 0;
	while (j < g_ph.n_philo)
	{
		pthread_detach(th[j]);
		j++;
	}
	printer("dead", i, 0);
	return (i);
}

int	breaker(pthread_t *th)
{
	int	i;

	while (k)
	{
		g_ph.hunger = 0;
		i = 0;
		while (i < g_ph.n_philo)
		{
			if (g_ph.t_die <= time_diff(g_ph.before[i]))
			{
				return (death_checker(th, i));
			}
			else if (g_ph.ph_meals[i] >= g_ph.n_meals && g_ph.n_meals != -1)
				g_ph.hunger++;
			if (g_ph.hunger == g_ph.n_philo)
				return (-1);
			i++;
		}
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

void	freeta(void)
{
	free(g_ph.forks);
	free(g_ph.ph_meals);
	free(g_ph.before);
}

int	main(int argc, char **argv)
{
	if ((argc == 6 || argc == 5) && !inputs_checker(argv, 0)
		&& !initializer(argv, argc))
	{
		if (ft_thread() == -1)
		{
			printf("\nkolchi chbe3\n");
			freeta();
		}	
		else
			return (0);
	}
	else
		printf("You fucked xD\n");
	return (0);
}
