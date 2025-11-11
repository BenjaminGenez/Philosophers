#include "philo.h"

static void	take_forks(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(philo->right_fork);
		print_status(philo, "has taken a fork");
		pthread_mutex_lock(philo->left_fork);
		print_status(philo, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(philo->left_fork);
		print_status(philo, "has taken a fork");
		pthread_mutex_lock(philo->right_fork);
		print_status(philo, "has taken a fork");
	}
	pthread_mutex_lock(&philo->data->meal_lock);
	philo->last_meal_time = get_time();
	pthread_mutex_unlock(&philo->data->meal_lock);
}

static void	drop_forks(t_philo *philo)
{
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}

void	philo_eat(t_philo *philo)
{
	take_forks(philo);
	print_status(philo, "is eating");
	pthread_mutex_lock(&philo->data->meal_lock);
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->data->meal_lock);
	ft_usleep(philo->data->time_to_eat);
	drop_forks(philo);
}

void	philo_sleep(t_philo *philo)
{
	print_status(philo, "is sleeping");
	ft_usleep(philo->data->time_to_sleep);
}

void	philo_think(t_philo *philo)
{
	print_status(philo, "is thinking");
	if (philo->data->num_philos % 2 != 0)
		usleep(1000);
}
