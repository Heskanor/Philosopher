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
	int n_philo;
	int t_die;
	int t_eat;
	int t_sleep;
	struct timeval base;
	int n_meals;
	struct timeval *before;
	int *timer;
	int death;
	int hunger;
	int* philo;
	int* ph_meals;//allocated
	pthread_mutex_t* forks; //allocated
	pthread_mutex_t print_mutex;
}ph_t;

ph_t g_ph;

int is_degit(char*s)
{
	int i;
	i = 0;
	if (s == NULL)
		return 2; 
	while (s[i] != '\0')
	{
		if (s[i] < '0' || s[i] >'9')
			return (0);
		i++;
	}
	return (1);
}

int inputs_checker(char **inputs, int c)
{
	int i;

	i = 1;
	if (c == 0)
	{
		while (i < 6 && is_degit(inputs[i]))
			i++;
		if (i != 6)
			return (1);
		return (0);
	}else if ((g_ph.n_philo <= 0 && g_ph.n_philo > 200) 
	|| g_ph.t_die < 60 || g_ph.t_eat < 60
	 || g_ph.t_sleep < 60 || g_ph.n_meals <= 0)
		return 1;
	return (0);
}

int		ft_atoi(const char *str)
{
	long long	nbr;
	int			i;
	int			s;

	i = 0;
	nbr = 0;
	s = 1;
	while (str[i] == 32 || (str[i] < 14 &&  str[i] > 8))
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

void mutex_constractor(pthread_mutex_t *mutex)
{
	int i;

	i = 0;
	pthread_mutex_init(&g_ph.print_mutex, NULL);
	while (i < g_ph.n_philo)
	{
		pthread_mutex_init(&mutex[i], NULL);
		i++;
	}
}

int initializer(char **inputs)
{
	g_ph.death = 0;
	g_ph.hunger = 0;
	g_ph.n_philo = ft_atoi(inputs[1]);
	g_ph.t_die = ft_atoi(inputs[2]);
	g_ph.t_eat = ft_atoi(inputs[3]);
	g_ph.t_sleep = ft_atoi(inputs[4]);
	g_ph.n_meals = ft_atoi(inputs[5]);
	if(inputs_checker(inputs,1))
		return(1);
	g_ph.forks = malloc(sizeof(pthread_mutex_t) * g_ph.n_philo);//allocated
	g_ph.ph_meals = malloc(sizeof(int) * g_ph.n_philo);//allocated
	g_ph.philo = malloc(sizeof(int) * g_ph.n_philo);//allocated
	g_ph.timer = malloc(sizeof(struct timeval) * g_ph.n_philo);//allocated
	memset(g_ph.ph_meals, 0, g_ph.n_philo);
	memset(g_ph.philo, 0, g_ph.n_philo);
	memset(g_ph.timer, 0, g_ph.n_philo);
	mutex_constractor(g_ph.forks);
	return 0;
}

int time_diff(struct timeval x)
{
	int x_ms;
	int y_ms;
	int diff;
	struct timeval y;

	gettimeofday(&y, NULL);
	x_ms = (int)x.tv_sec * 1000 + (int)x.tv_usec / 1000;
	y_ms = (int)y.tv_sec * 1000 + (int)y.tv_usec / 1000;
	diff = y_ms - x_ms;
	return (diff);
}

void printer(char *s, int index, int sleeper)
{
	int t;

	t = time_diff(g_ph.base);
	//g_ph.philo = s[0];
	pthread_mutex_lock(&g_ph.print_mutex);
	if (s[0] == 'f')
		printf("%d ms - Philosopher [%d] has taken a 2end fork\n", t,index + 1);
	else if (s[0] == 'F')
		printf("%d ms - Philosopher [%d] has taken a 1st fork\n", t,index + 1);
	else if (s[0] == 'e')
		printf("%d ms - Philosopher [%d] is eating\n", t,index + 1);
	else if (s[0] == 's')
		printf("%d ms - Philosopher [%d] is sleeping\n", t,index + 1);
	else if (s[0] == 't')
		printf("%d ms - Philosopher [%d] is thinking\n", t,index + 1);
	pthread_mutex_unlock(&g_ph.print_mutex);
	usleep(sleeper * 1000);
}

void* routine(void *arg)
{
	int index;
	int next;

	index = *(int*)arg;
	next = index + 1;
	if (index == (g_ph.n_philo - 1))
		next = 0;
	while (g_ph.hunger < g_ph.n_meals)
	{
		pthread_mutex_lock(&g_ph.forks[index]);
		printer("Fork",index, 0);
		pthread_mutex_lock(&g_ph.forks[next]);
		printer("fork",index,0);
		printer("eat",index,g_ph.t_eat);
		gettimeofday(&g_ph.before[index],NULL);
		pthread_mutex_unlock(&g_ph.forks[index]);
		pthread_mutex_unlock(&g_ph.forks[next]);
		g_ph.ph_meals[index]++;
		printer("sleep",index,g_ph.t_sleep);
		printer("think",index,0);
	}
	return (NULL);
}

int breaker(pthread_t *th)
{
	int k;
	int i;
	int j;

	k = 1;
	while (g_ph.hunger != g_ph.n_meals)
	{
		g_ph.hunger = 0;
		i = 0;
		while (i < g_ph.n_philo)
		{
			if (g_ph.t_die <= time_diff(g_ph.before[i]))
			{
				j = 0;
				while (j < g_ph.n_philo)
				{
					pthread_detach(th[i]);
					j++;
				}
				printf("%d ms - Philosopher [%d] died\n", time_diff(g_ph.before[i]), i + 1);
				return i;
			}
			else if (g_ph.ph_meals[i] == g_ph.n_meals)
				g_ph.hunger++;
			i++;
		}
	}
	return 1;
}
// check all meals before death or reverse these process
// make the loop

int ft_thread(void)
{
	int i;
	pthread_t *th;
	int e;

	i = 0;
	th = (pthread_t *)malloc(sizeof(pthread_t)*(g_ph.n_philo + 1)); //allocated
	gettimeofday(&g_ph.base,NULL);
	while(i < g_ph.n_philo)
	{
		g_ph.before[i].tv_sec = g_ph.base.tv_sec;
		g_ph.before[i].tv_usec = g_ph.base.tv_usec;
		if (pthread_create(&th[i], NULL, &routine, &i) != 0){
			perror("Failed to create thread");
		}
		i++;
		usleep(100);
	}
	i = 0;
	e = breaker(th);
	while(i < g_ph.n_philo)
	{
		pthread_join(th[i], NULL);
		i++;
	}
	return (e); 
}

int main(int argc, char **argv)
{
	int checker;
	int *meals;
	int i;

	if (argc == 6 && !inputs_checker(argv,0) && !initializer(argv))
	{
		printf("Perfect args !\n");
		if(ft_thread())
			return (0);
	}
	else
		printf("You fucked xD\n");
	return (0);
}




/*printf("%d = number_of_philosophers\n%d = time_to_die\n%d = time_to_eat\n%d = time_to_sleep\n%d = number_of_times_each_philosopher_must_eat\n\n\n\n",
		g_ph.n_philo, g_ph.t_die,g_ph.t_eat,g_ph.t_sleep,g_ph.n_meals);*/
		