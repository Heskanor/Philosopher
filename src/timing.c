/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timing.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashite <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/08 15:06:25 by ashite            #+#    #+#             */
/*   Updated: 2021/10/08 15:06:30 by ashite           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

unsigned int	time_diff(struct timeval x)
{
	unsigned int	x_ms;
	unsigned int	y_ms;
	struct timeval	y;

	gettimeofday(&y, NULL);
	x_ms = (int)x.tv_sec * 1000 + (int)x.tv_usec / 1000;
	y_ms = (int)y.tv_sec * 1000 + (int)y.tv_usec / 1000;
	return (y_ms - x_ms);
}

unsigned int	time_now(void)
{
	struct timeval	y;
	unsigned int	y_ms;

	gettimeofday(&y, NULL);
	y_ms = (int)y.tv_sec * 1000 + (int)y.tv_usec / 1000;
	return (y_ms);
}

void	mysleep(unsigned int t)
{
	struct timeval	y;
	unsigned int	y_ms;

	gettimeofday(&y, NULL);
	y_ms = (int)y.tv_sec * 1000 + (int)y.tv_usec / 1000;
	usleep(t * 1000 * 0.85);
	while (time_now() < y_ms + t)
		continue ;
}

void	freeta(void)
{
	free(g_ph.forks);
	free(g_ph.ph_meals);
	free(g_ph.before);
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
	printer("die", i, 0);
	return (i);
}
