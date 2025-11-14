#include "philo.h"

static int	create_philosophers(t_data *data)
{
	int	i;

	data->start_time = get_time();
	i = 0;
	while (i < data->num_philos)
	{
		data->philos[i].last_meal_time = data->start_time;
		/* [1] Create one thread per philosopher. Each thread runs
		   `philosopher_routine` and is stored in the philosopher
		   structure's `thread` field. */
		if (pthread_create(&data->philos[i].thread, NULL,
				philosopher_routine, &data->philos[i]) != 0)
			return (0);
		i++;
	}
	return (1);
}

static void	join_philosophers(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		pthread_join(data->philos[i].thread, NULL);
		i++;
	}
}

static int	handle_single_philosopher(t_data *data)
{
	if (data->num_philos == 1)
	{
		printf("0 1 has taken a fork\n");
		ft_usleep(data->time_to_die);
		printf("%d 1 died\n", data->time_to_die);
		return (1);
	}
	return (0);
}

static int	start_simulation(t_data *data)
{
	if (!init_philos(data))
	{
		cleanup(data);
		return (0);
	}
	if (!create_philosophers(data))
	{
		cleanup(data);
		return (0);
	}
	monitor_philos(data);
	join_philosophers(data);
	cleanup(data);
	return (1);
}

int	main(int argc, char **argv)
{
	t_data	data;

	if (!init_data(&data, argc, argv))
	{
		printf("Error: Invalid arguments\n");
		return (1);
	}
	if (handle_single_philosopher(&data))
	{
		cleanup(&data);
		return (0);
	}
	if (!start_simulation(&data))
		return (1);
	return (0);
}
