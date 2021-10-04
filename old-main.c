#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>


pthread_mutex_t *forks;
typedef struct ph_s{
	int n_philo; //number_of_philosophers
	int t_die; //time_to_die
	int t_eat; //time_to_eat 
	int t_sleep;//time_to_sleep 
	struct timeval base;
	int n_meals;//[number_of_times_each_philosopher_must_eat]
}ph_t;

struct timeval after, before;
typedef struct philo_s
{
	int dead;//    Dead or not
	struct timeval after;
	struct timeval before;
	int n_meals;// [number_of_times_eated
}philo_t;

ph_t g_ph;
philo_t *g_philo;

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

double time_diff(struct timeval x)
{
	double x_ms , y_ms , diff;
	struct timeval y;

	time_diff(g_ph.base,y);
	x_ms = (double)x.tv_sec*1000 + (double)x.tv_usec/1000;
	y_ms = (double)y.tv_sec*1000 + (double)y.tv_usec/1000;
	
	diff = (double)y_ms - (double)x_ms;
	
	return diff;
}
void printer(char *s)
{
	int t;

	t = (int)time_diff(g_ph.base);
	if (s[0] == 'f')
		printf("%d ms - Philosopher [%d] has taken a fork\n", index,t);
	else if (s[0] == 'e')
		printf("%d ms - Philosopher [%d] is eating\n", index,t);
	else if (s[0] == 's')
		printf("%d ms - Philosopher [%d] is sleeping\n", index,t);
	else if (s[0] == 't')
		printf("%d ms - Philosopher [%d] is thinking\n", index,t);
	else if (s[0] == 'd')
		printf("%d ms - Philosopher [%d] died\n", index,t);
}

void* routine(void *arg)
{
	int index;
	int next;

	g_philo[index].before = g_ph.base;
	g_philo[index].dead = 0;
	index = *(int*)arg;
	next = index + 1;
	if (index == (g_ph.n_philo - 1))
		next = 0;
	while (g_philo[index].n_meals < g_ph.n_meals || (int)time_diff(g_philo[index].before) < g_ph.t_die)
	{
		pthread_mutex_lock(&forks[index]);
		printer("fork");
		pthread_mutex_lock(&forks[next]);
		printer("fork");
		gettimeofday(&g_philo[index].before,NULL);
		printer("eat");
		usleep(g_ph.t_eat * 1000);
		pthread_mutex_unlock(&forks[index]);
		pthread_mutex_unlock(&forks[next]);
		g_philo[index].n_meals;
		printer("sleep");
		usleep(g_ph.t_sleep * 1000);
		printer("think");
	}
	if ((int)time_diff(g_philo[index].before) >= g_ph.t_die)
	{
		printer("dead");
		g_philo[index].dead == 1;
	}	
	return (NULL);
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
void mutex_constractor(pthread_mutex_t *mutex)
{
	int i;

	i = 0;
	while (i < g_ph.n_philo)
	{
		pthread_mutex_init(&forks[i], NULL);
		i++;
	}
}
int initializer(char **inputs)
{
	g_ph.n_philo = ft_atoi(inputs[1]);
	g_ph.t_die = ft_atoi(inputs[2]);
	g_ph.t_eat = ft_atoi(inputs[3]);
	g_ph.t_sleep = ft_atoi(inputs[4]);
	g_ph.n_meals = ft_atoi(inputs[5]);
	if ((g_ph.n_philo <= 0 && g_ph.n_philo > 200) || g_ph.t_die < 60 || g_ph.t_eat < 60
	 || g_ph.t_sleep < 60 || g_ph.n_meals <= 0)
		return 1;
	/*if (g_ph.n_philo > 1)
		forks = g_ph.n_philo;*/
	//g_ph.forks = malloc(sizeof(char) * forks*2);
	forks = malloc(sizeof(pthread_mutex_t) * g_ph.n_philo);
	g_philo = malloc(sizeof(philo_t)*g_ph.n_philo);
	mutex_constractor(forks);
	return 0;
}
int breaker(void)
{
	int k;
	int i;
	int m;

	k = 1;
	while (k)
	{
		i  = 0;
		m = 0;
		while (i < g_ph.n_philo)
		{
			if (g_philo[i].dead == 1)
			{
				break;
				k = 0;
			}	
			i++;
		}
	}
}
// check all meals before death or reverse these process
void ft_thread(ph_t ph)
{
	int i;
	int j;
	pthread_t *th;

	i = 0;
	j = 0;
	th = (pthread_t *)malloc(sizeof(pthread_t)*g_ph.n_philo);
	
	while(i < g_ph.n_philo)
	{
		
		if (pthread_create(&th[i], NULL, &routine, &i) != 0){
			perror("Failed to create thread");
		}
		i++;
	}
	
	while(j < g_ph.n_philo)
	{
		if (pthread_join(th[j], NULL) != 0) {
            perror("Failed to join thread");
        }
		j++;
	}
	
}

int main(int argc, char **argv)
{
	int checker;
	int *meals;
	int i;

	if (argc == 6 && !inputs_checker(argv) && !initializer(argv))
	{
		printf("Perfect args !\n");
		printf("%d = number_of_philosophers\n%d = time_to_die\n%d = time_to_eat\n%d = time_to_sleep\n%d = number_of_times_each_philosopher_must_eat\n\n\n\n",
		g_ph.n_philo, g_ph.t_die,g_ph.t_eat,g_ph.t_sleep,g_ph.n_meals);
		ft_thread(g_ph);
		if(breaker())
			return (0);
	}
	else
		printf("You fucked xD\n");

	/*pthread_create(&philo1, NULL, &philo_1, NULL);
	pthread_join(philo1,NULL);*/

	return (0);
}

/*
* protect inputs from negative values -> make values checker [done!]
* philos sleep -> use sleep
* philos think -> use sleep
* lock mutex
* search forks
* use forks
* philos eats
* drop forks
* unlock mutex
* print actions
* calc time between now and last eat
* eat when u need to eat
* philos dies after no eating
* after philo after getting satisfied
* when the philo die stop all
* when they all satisfied eating stop all
*gcc -g -pthread old-main.c && ./a.out 5 1000 60 60 5
#include <sys/time.h>

struct timeval stop, before;
gettimeofday(&start, NULL);
//do stuff
gettimeofday(&stop, NULL);
printf("took %lu us\n", (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec);


philo < 201
times > 60 ms

*/