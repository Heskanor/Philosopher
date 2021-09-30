#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int inputs_checker(char **inputs)
{
	int i;
	int input;

	i = 1;
	input = 1;
	while (i < 6)
	{
		input = is_degit(inputs[i]);
		i++;
	}
	return (input);
}

int main(int argc, char **argv)
{
    int n_philo, t_die, t_eat, t_sleep, n_meals, i,j;
    int *meals;
    char *forks;
    if (argc != 6)
        return (1);
    

}