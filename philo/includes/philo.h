#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>
# include <string.h>

typedef struct s_data	t_data;

typedef struct s_philo
{
	int				id;
	int				meals_eaten;
	long long		last_meal_time;
	pthread_t		thread;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	t_data			*data;
}	t_philo;

typedef struct s_data
{
	int				num_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				must_eat_count;
	int				all_ate_enough;
	int				someone_died;
	long long		start_time;
	pthread_mutex_t	*forks;
	pthread_mutex_t	write_lock;
	pthread_mutex_t	death_lock;
	pthread_mutex_t	meal_lock;
	t_philo			*philos;
}	t_data;

/* Initialization */
int			init_data(t_data *data, int argc, char **argv);
int			init_philos(t_data *data);
void		cleanup(t_data *data);

/* Utils */
long long	get_time(void);
void		ft_usleep(long long time);
int			ft_atoi(const char *str);
void		print_status(t_philo *philo, char *status);

/* Philosopher routine */
void		*philosopher_routine(void *arg);
void		philo_eat(t_philo *philo);
void		philo_sleep(t_philo *philo);
void		philo_think(t_philo *philo);

/* Monitor */
int			check_death(t_data *data);
int			is_simulation_over(t_data *data);
void		monitor_philos(t_data *data);

#endif
