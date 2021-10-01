#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

pthread_mutex_t g_mutex;
typedef struct ph_s{
	int n_philo; //number_of_philosophers
	int t_die; //time_to_die
	int t_eat; //time_to_eat 
	int t_sleep;//time_to_sleep 
	int n_meals;//[number_of_times_each_philosopher_must_eat]
	char *forks;
}ph_t;

typedef struct filo_s{
	int dead; //Dead or not
	int damaged; // low HP
	int n_meals;// [number_of_times_eated
}filo_t;

ph_t g_ph;
filo_t g_filo;

int		ft_atoi(const char *str)
{
	long long	nbr;
	int			i;
	int			s;

	i = 0;
	nbr = 0;
	s = 1;
	while (str[i] == ' ' || str[i] == '\n' || str[i] == '\t'
			|| str[i] == '\v' || str[i] == '\f' || str[i] == '\r')
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
			return ((s < 0) ? (0) : (-1));
		}
	}
	return (nbr * s);
}

int is_degit(char*s)
{
	int i;
	i = 0;
	if(s == NULL)
		return 2; 
	while (s[i] != '\0')
	{
		if (s[i] < '0' || s[i] >'9')
			return (0);
		i++;
	}
	return (1);
}

void* routine(void *meals) {
    int meal;
	meal = *(int*)meals;
    //if meal <
	meal++;
    printf("Local sum: %d\n", meal);
    *(int*)meals = meal;
    return meals;

}

int inputs_checker(char **inputs)
{
	int i;

	i = 1;
	while (i < 6 && is_degit(inputs[i]))
		i++;
	if (i != 6)
		return (1);
	return (0);
}

int initializer(char **inputs)
{
	int forks;
	forks = 2;
	g_ph.n_philo = ft_atoi(inputs[1]);
	g_ph.t_die = ft_atoi(inputs[2]);
	g_ph.t_eat = ft_atoi(inputs[3]);
	g_ph.t_sleep = ft_atoi(inputs[4]);
	g_ph.n_meals = ft_atoi(inputs[5]);
	if (g_ph.n_philo <= 0 || g_ph.t_die < 60 || g_ph.t_eat < 60
	 || g_ph.t_sleep < 60 || g_ph.n_meals <= 0)
		return 1;
	if (g_ph.n_philo > 1)
		forks = g_ph.n_philo;
	g_ph.forks = malloc(sizeof(char) * forks*2);
	//g_ph.n_meals = malloc(sizeof(int) * g_ph.n_philo);
	memset( g_ph.forks, 1, forks*2 );
	return 0;
}

/*void ft_thread(ph_t ph)
{
	int i;
	pthread_t *philo;
	i = 0;
	philo = (pthread_t *)malloc(sizeof(pthread_t)*g_ph.n_philo);
	while(i < g_ph.n_philo)
	{
		if (pthread_create(&philo[i], NULL, &routine, NULL) != 0) {
			perror("Failed to create thread");
		}
	}
}*/

int main(int argc, char **argv)
{
	int checker;
	int *meals;
	int i;

	if (argc == 6 && !inputs_checker(argv) && !initializer(argv))
	{
		printf("Perfect args !\n");
		printf("%d = number_of_philosophers\n%d = time_to_die\n%d = time_to_eat\n%d = time_to_sleep\n%d = number_of_times_each_philosopher_must_eat\n",
		g_ph.n_philo, g_ph.t_die,g_ph.t_eat,g_ph.t_sleep,g_ph.n_meals);
		//ft_thread(g_ph);
	}
	else
		printf("You fucked xD\n");

	/*pthread_create(&philo1, NULL, &philo_1, NULL);
	pthread_join(philo1,NULL);*/

	return (0);
}

/*
protect inputs from negative values -> make values checker [done!]
philos sleep -> use sleep
philos think -> use sleep
lock mutex
search forks
use forks
philos eats
drop forks
unlock mutex
print actions
calc time between now and last eat
eat when u need to eat
philos dies after no eating
stop philo after getting satisfied
when the philo die stop all
when they all satisfied eating stop all
*/