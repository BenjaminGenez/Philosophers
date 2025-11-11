#include "philo.h"

void	*philosopher_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 == 0)
		ft_usleep(1);
	while (!is_simulation_over(philo->data))
	{
		philo_eat(philo);
		if (is_simulation_over(philo->data))
			break ;
		philo_sleep(philo);
		if (is_simulation_over(philo->data))
			break ;
		philo_think(philo);
	}
	return (NULL);
}
