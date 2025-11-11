#include "philo.h"

long long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	ft_usleep(long long time)
{
	long long	start;

	start = get_time();
	while ((get_time() - start) < time)
		usleep(100);
}

int	ft_atoi(const char *str)
{
	int	result;
	int	sign;

	result = 0;
	sign = 1;
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		result = result * 10 + (*str - '0');
		str++;
	}
	return (result * sign);
}

void	print_status(t_philo *philo, char *status)
{
	long long	timestamp;

	pthread_mutex_lock(&philo->data->write_lock);
	pthread_mutex_lock(&philo->data->death_lock);
	if (!philo->data->someone_died)
	{
		timestamp = get_time() - philo->data->start_time;
		printf("%lld %d %s\n", timestamp, philo->id, status);
	}
	pthread_mutex_unlock(&philo->data->death_lock);
	pthread_mutex_unlock(&philo->data->write_lock);
}

void	cleanup(t_data *data)
{
	int	i;

	if (data->forks)
	{
		i = 0;
		while (i < data->num_philos)
		{
			pthread_mutex_destroy(&data->forks[i]);
			i++;
		}
		free(data->forks);
	}
	pthread_mutex_destroy(&data->write_lock);
	pthread_mutex_destroy(&data->death_lock);
	pthread_mutex_destroy(&data->meal_lock);
	if (data->philos)
		free(data->philos);
}
