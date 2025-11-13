#include "philo.h"

int	is_simulation_over(t_data *data)
{
	int	result;

	pthread_mutex_lock(&data->death_lock);
	result = data->someone_died || data->all_ate_enough;
	pthread_mutex_unlock(&data->death_lock);
	return (result);
}

static int	check_philosopher_death(t_data *data, int i)
{
	long long	current_time;
	long long	time_since_meal;

	pthread_mutex_lock(&data->meal_lock);
	current_time = get_time();
	time_since_meal = current_time - data->philos[i].last_meal_time;
	pthread_mutex_unlock(&data->meal_lock);
	if (time_since_meal >= data->time_to_die)
	{
		pthread_mutex_lock(&data->death_lock);
		data->someone_died = 1;
		pthread_mutex_unlock(&data->death_lock);
		pthread_mutex_lock(&data->write_lock);
		printf("%lld %d died\n", current_time - data->start_time,
			data->philos[i].id);
		pthread_mutex_unlock(&data->write_lock);
		return (1);
	}
	return (0);
}

static int	check_all_ate_enough(t_data *data)
{
	int	i;
	int	all_satisfied;

	if (data->must_eat_count == -1)
		return (0);
	all_satisfied = 1;
	i = 0;
	while (i < data->num_philos)
	{
		pthread_mutex_lock(&data->meal_lock);
		if (data->philos[i].meals_eaten < data->must_eat_count)
			all_satisfied = 0;
		pthread_mutex_unlock(&data->meal_lock);
		if (!all_satisfied)
			break ;
		i++;
	}
	if (all_satisfied)
	{
		pthread_mutex_lock(&data->death_lock);
		data->all_ate_enough = 1;
		pthread_mutex_unlock(&data->death_lock);
		return (1);
	}
	return (0);
}

void	monitor_philos(t_data *data)
{
	int	i;

	while (1)
	{
		i = 0;
		while (i < data->num_philos)
		{
			if (check_philosopher_death(data, i))
				return ;
			i++;
		}
		if (check_all_ate_enough(data))
			return ;
		usleep(1000);
	}
}
