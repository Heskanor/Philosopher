/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashite <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/08 15:06:38 by ashite            #+#    #+#             */
/*   Updated: 2021/10/08 19:07:58 by ashite           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

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

	i = 0;
	pthread_mutex_init(&g_ph.print_mutex, NULL);
	pthread_mutex_init(&g_ph.meal_mutex, NULL);
	while (i < g_ph.n_philo)
	{
		pthread_mutex_init(&mutex[i], NULL);
		i++;
	}
	i = 0;
	while (i < g_ph.n_philo)
	{
		pthread_mutex_init(&g_ph.eat_mutex[i], NULL);
		i++;
	}
}

int	initializer(char **inputs, int m)
{
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
	g_ph.forks = malloc(sizeof(pthread_mutex_t) * g_ph.n_philo);
	g_ph.eat_mutex = malloc(sizeof(pthread_mutex_t) * g_ph.n_philo);
	g_ph.ph_meals = malloc(sizeof(int) * g_ph.n_philo);
	g_ph.philos = malloc(sizeof(int) * g_ph.n_philo);
	g_ph.before = malloc(sizeof(struct timeval) * g_ph.n_philo);
	mutex_constractor(g_ph.forks);
	return (0);
}
