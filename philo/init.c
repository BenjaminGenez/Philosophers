#include "philo.h"

static int	is_valid_number(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	if (ft_atoi(str) <= 0)
		return (0);
	return (1);
}

static int	validate_args(int argc, char **argv)
{
	int	i;

	if (argc != 5 && argc != 6)
	{
		printf("Error: Wrong number of arguments\n");
		printf("Usage: ./philo number_of_philosophers time_to_die ");
		printf("time_to_eat time_to_sleep ");
		printf("[number_of_times_each_philosopher_must_eat]\n");
		return (0);
	}
	i = 1;
	while (i < argc)
	{
		if (!is_valid_number(argv[i]))
			return (0);
		i++;
	}
	return (1);
}

static int	init_mutexes(t_data *data)
{
	int	i;

	data->forks = malloc(sizeof(pthread_mutex_t) * data->num_philos);
	if (!data->forks)
		return (0);
	i = 0;
	while (i < data->num_philos)
	{
		if (pthread_mutex_init(&data->forks[i], NULL) != 0)
			return (0);
		i++;
	}
	if (pthread_mutex_init(&data->write_lock, NULL) != 0)
		return (0);
	if (pthread_mutex_init(&data->death_lock, NULL) != 0)
		return (0);
	if (pthread_mutex_init(&data->meal_lock, NULL) != 0)
		return (0);
	return (1);
}

int	init_data(t_data *data, int argc, char **argv)
{
	if (!validate_args(argc, argv))
		return (0);
	memset(data, 0, sizeof(t_data));
	data->num_philos = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	data->must_eat_count = -1;
	if (argc == 6)
		data->must_eat_count = ft_atoi(argv[5]);
	data->someone_died = 0;
	data->all_ate_enough = 0;
	data->start_time = get_time();
	if (!init_mutexes(data))
		return (0);
	return (1);
}

int	init_philos(t_data *data)
{
	int	i;

	data->philos = malloc(sizeof(t_philo) * data->num_philos);
	if (!data->philos)
		return (0);
	i = 0;
	while (i < data->num_philos)
	{
		data->philos[i].id = i + 1;
		data->philos[i].meals_eaten = 0;
		data->philos[i].last_meal_time = data->start_time;
		data->philos[i].left_fork = &data->forks[i];
		data->philos[i].right_fork = &data->forks[(i + 1) % data->num_philos];
		data->philos[i].data = data;
		i++;
	}
	return (1);
}
