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

pthread_mutex_t *forks;
typedef struct ph_s{
	int n_philo;
	int t_die;
	int t_eat;
	int t_sleep;
	struct timeval base;
	int n_meals;
	struct timeval now;
	struct timeval eating;
	int is_dead;
	int* dead
	int ph_meals;
	pthread_mutex_t death;
}ph_t;

ph_t g_ph;

int main(int argc, char **argv)
{
	int checker;
	int *meals;
	int i;

	if (argc == 6 && !inputs_checker(argv) && !initializer(argv))
	{
		printf("Perfect args !\n");
		/*printf("%d = number_of_philosophers\n%d = time_to_die\n%d = time_to_eat\n%d = time_to_sleep\n%d = number_of_times_each_philosopher_must_eat\n\n\n\n",
		g_ph.n_philo, g_ph.t_die,g_ph.t_eat,g_ph.t_sleep,g_ph.n_meals);*/
		if(ft_thread(g_ph))
			return (0);
	}
	else
		printf("You fucked xD\n");
	return (0);
}
