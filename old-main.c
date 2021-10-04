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
	int n_meals;//[number_of_times_each_philosopher_must_eat]
}ph_t;

/*typedef struct    timeval_s{
  time_t        tv_sec ;   //used for seconds
  suseconds_t       tv_usec ;   //used for microseconds
}timeval_t;*/
struct timeval after, before;
typedef struct philo_s
{
	int dead;//    Dead or not
	int damaged;// HP level
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

double time_diff(struct timeval x , struct timeval y)
{
	double x_ms , y_ms , diff;
	
	x_ms = (double)x.tv_sec*1000000 + (double)x.tv_usec;
	y_ms = (double)y.tv_sec*1000000 + (double)y.tv_usec;
	
	diff = (double)y_ms - (double)x_ms;
	
	return diff;
}
//test push
void* routine(void *arg)
{
	int index = *(int*)arg;
	
	int time;
	time = gettimeofday(&before,NULL);
	g_philo[index].n_meals = 0;
	while (g_philo[index].n_meals < g_ph.n_meals && after.tv_sec * 1000 < g_ph.t_die)
	{
		time = gettimeofday(&before,NULL);
		pthread_mutex_lock(&forks[index]);
		printf("10 ms - Philosopher [%d] has taken a fork\n", index);
		if (index == (g_ph.n_philo - 1))
			pthread_mutex_lock(&forks[0]);
		else
			pthread_mutex_lock(&forks[index + 1]);
		printf("10 ms - Philosopher [%d] has taken a fork\n", index);
		printf("10 ms - Philosopher [%d] is eating\n", index);
		usleep(10);
		pthread_mutex_unlock(&forks[index]);
		time = gettimeofday(&after,NULL);
		g_philo[index].n_meals++;
	}
	printf("\nchbe3 : philo%d\n",index);
	if (index == (g_ph.n_philo - 1))
		pthread_mutex_unlock(&forks[0]);
	else
		pthread_mutex_unlock(&forks[index + 1]);
	printf("10 ms - Philosopher [%d] is sleeping\n", index);
	usleep(10);
	printf("10 ms - Philosopher [%d] is thinking\n", index);
	usleep(10);
	return (NULL);
	//died
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