/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ashite <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/08 15:06:12 by ashite            #+#    #+#             */
/*   Updated: 2021/10/08 15:06:16 by ashite           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <pthread.h>
# include <sys/time.h>

typedef struct ph_s{
	int					n_philo;
	unsigned int		t_die;
	unsigned int		t_eat;
	unsigned int		t_sleep;
	struct timeval		base;
	int					n_meals;
	struct timeval		*before;
	int					hunger;
	int					*ph_meals;
	int					*philos;
	pthread_mutex_t		*forks;
	pthread_mutex_t		meal_mutex;
	pthread_mutex_t		print_mutex;
	pthread_mutex_t		*eat_mutex;
}						t_ph;

t_ph			g_ph;

void			printer(char *str, int index, unsigned int sleeper);
void			mutex_constractor(pthread_mutex_t *mutex);
int				inputs_checker(char **inputs, int c);
int				death_checker(pthread_t *th, int i);
int				initializer(char **inputs, int m);
unsigned int	time_diff(struct timeval x);
int				ft_atoi(const char *str);
void			mysleep(unsigned int t);
int				breaker(pthread_t *th);
void			*routine(void *arg);
int				is_degit(char *s);
int				ft_thread(void);
unsigned int	time_now(void);
void			freeta(void);

#endif
