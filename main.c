/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbany <mbany@student.42warsaw.pl>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 19:54:12 by mbany             #+#    #+#             */
/*   Updated: 2024/11/07 19:57:06 by mbany            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# define TRUE 1
# define SUCCESS 1
# define FALSE 0
# define ERROR 0
# define FAILURE 0

# include <limits.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_philo
{
	int				id;
	int				last_meal;
	int				is_eating;
	int				time_to_die;
	int				time_to_sleep;
	int				time_to_eat;
	int				meals_eaten;
	int				hungry;
	pthread_t		thread_id;
	struct s_table	*tab;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
}					t_philo;

typedef struct s_table
{
	int				number_of_philosophers;
	int				time_to_die;
	int				time_to_sleep;
	int				time_to_eat;
	int				number_of_meals;
	int				starting_time;
	int				philosopher_dead;
	pthread_mutex_t	*fork_mutex;
	pthread_mutex_t	mutex;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	t_philo			*philosophers;
}					t_table;

int					initialize_table(t_table *tab, char **argv);
void				print_limits_error(void);
void				print_right_syntax(void);
void				philosophers_is_eating(t_philo *philosophers);
void				philosophers_is_sleeping(t_philo *philosophers);
void				philosophers_is_thinking(t_philo *philosophers);
void				*philosophers_routine(void *arguments);
void				begin_philosophers_routine(t_table *data);
void				free_philosophers(t_table *philosophers);
void				*begin_monitoring(void *arg);
void				join_threads(t_table *data);
void				print_message(char *str, t_philo *philosopher);
void				ft_sleep(int time, t_philo *philosophers);

long				ft_atoi_long(const char *str);

int					philosopher_is_dead(t_philo *philosophers);
int					philosopher_takes_forks(t_philo *philosophers);
int					ft_atoi(const char *str);
int					init_mutex(t_table *tab);
int					get_time(void);
int					validate_arguments(int argc, char **argv);
long				ft_atol(const char *str);

#endif

int	init_philos(t_table *tab)
{
	int	i;

	i = 0;
	tab->philosophers = malloc(sizeof(t_philo) * tab->number_of_philosophers);
	if (!tab->philosophers)
		return (FAILURE);
	while (i < tab->number_of_philosophers)
	{
		tab->philosophers[i].id = i + 1;
		tab->philosophers[i].left_fork = &tab->fork_mutex[i];
		tab->philosophers[i].right_fork = &tab->fork_mutex[(i + 1)
			% tab->number_of_philosophers];
		tab->philosophers[i].tab = tab;
		tab->philosophers[i].time_to_die = tab->time_to_die;
		tab->philosophers[i].time_to_eat = tab->time_to_eat;
		tab->philosophers[i].time_to_sleep = tab->time_to_sleep;
		tab->philosophers[i].last_meal = 0;
		tab->philosophers[i].is_eating = 0;
		tab->philosophers[i].meals_eaten = 0;
		tab->philosophers[i].hungry = 1;
		i++;
	}
	return (SUCCESS);
}

int	init_mutex(t_table *tab)
{
	int	i;

	i = -1;
	tab->fork_mutex = malloc(sizeof(pthread_mutex_t)
			* tab->number_of_philosophers);
	if (!tab->fork_mutex)
		return (FAILURE);
	while (++i < tab->number_of_philosophers)
		pthread_mutex_init(&tab->fork_mutex[i], NULL);
	pthread_mutex_init(&tab->mutex, NULL);
	return (SUCCESS);
}

void	unlock_philosophers_mutex(t_table *philosophers)
{
	pthread_mutex_unlock(philosophers->left_fork);
	pthread_mutex_unlock(philosophers->right_fork);
}

int	initialize_table(t_table *tab, char **argv)
{
	tab->number_of_philosophers = ft_atoi(argv[1]);
	tab->time_to_die = ft_atoi(argv[2]);
	tab->time_to_eat = ft_atoi(argv[3]);
	tab->time_to_sleep = ft_atoi(argv[4]);
	if (argv[5])
		tab->number_of_meals = ft_atoi(argv[5]);
	else
		tab->number_of_meals = -1;
	tab->philosopher_dead = 0;
	if (init_mutex(tab) == FAILURE)
		return (FAILURE);
	if (init_philos(tab) == FAILURE)
		return (FAILURE);
	return (SUCCESS);
}

int	is_alive(t_philo *philo)
{
	if (get_time() - philo->tab->starting_time > philo->time_to_die)
	{
		philo->tab->philosopher_dead = 1;
		if (philo->hungry)
			printf("%d %d died\n", get_time() - philo->tab->starting_time,
				philo->id);
		return (0);
	}
	else
		return (1);
}

int	check_if_one_is_dead_or_full(t_table *tab)
{
	int	i;

	i = 0;
	while (i < tab->number_of_philosophers)
	{
		if (tab->philosophers[i].meals_eaten == tab->number_of_meals)
			tab->philosophers[i].hungry = 0;
		if (!is_alive(&tab->philosophers[i]))
			return (ERROR);
		i++;
	}
	return (1);
}

void	free_philosophers(t_table *philosophers)
{
	free(philosophers->philosophers);
	free(philosophers->fork_mutex);
}

void	*begin_monitoring(void *arg)
{
	t_table	*table;
	int		i;

	i = 1;
	table = (t_table *)arg;
	while (i)
	{
		pthread_mutex_lock(&table->mutex);
		if (check_if_one_is_dead_or_full(table) == ERROR)
		{
			usleep(100);
			i = 0;
			pthread_mutex_unlock(&table->mutex);
			return (NULL);
		}
		pthread_mutex_unlock(&table->mutex);
		usleep(200);
	}
	return (NULL);
}

int	philosopher_takes_forks(t_philo *philosophers)
{
	if (philosophers->id % 2 == 0)
		pthread_mutex_lock(philosophers->right_fork);
	else
		pthread_mutex_lock(philosophers->left_fork);
	if (!philosopher_is_dead(philosophers))
		print_message("has taken a fork", philosophers);
	if (philosophers->id % 2 == 0)
	{
		if (pthread_mutex_lock(philosophers->left_fork) != 0)
			return (pthread_mutex_unlock(philosophers->right_fork), 1);
		if (!philosopher_is_dead(philosophers))
			print_message("has taken a fork", philosophers);
	}
	else
	{
		if (pthread_mutex_lock(philosophers->right_fork) != 0)
			return (pthread_mutex_unlock(philosophers->left_fork), 1);
		if (!philosopher_is_dead(philosophers))
			print_message("has taken a fork", philosophers);
	}
	return (0);
}

void	philosophers_is_eating(t_philo *philosophers)
{
	print_message("is eating", philosophers);
	pthread_mutex_lock(&philosophers->tab->mutex);
	philosophers->last_meal = get_time() - philosophers->tab->starting_time;
	philosophers->time_to_die = philosophers->last_meal
		+ philosophers->tab->time_to_die;
	pthread_mutex_unlock(&philosophers->tab->mutex);
	ft_sleep(philosophers->tab->time_to_eat, philosophers);
	pthread_mutex_lock(&philosophers->tab->mutex);
	if (philosophers->meals_eaten != -1)
		philosophers->meals_eaten++;
	pthread_mutex_unlock(&philosophers->tab->mutex);
	pthread_mutex_unlock(philosophers->left_fork);
	pthread_mutex_unlock(philosophers->right_fork);
}

void	philosophers_is_sleeping(t_philo *philosophers)
{
	print_message("is sleeping", philosophers);
	ft_sleep(philosophers->tab->time_to_sleep, philosophers);
}

void	philosophers_is_thinking(t_philo *philosophers)
{
	print_message("is thinking", philosophers);
}

int	philosopher_is_dead(t_philo *philosophers)
{
	pthread_mutex_lock(&philosophers->tab->mutex);
	if (philosophers->tab->philosopher_dead == 1)
	{
		pthread_mutex_unlock(&philosophers->tab->mutex);
		return (1);
	}
	pthread_mutex_unlock(&philosophers->tab->mutex);
	return (0);
}

int	is_hungry(t_philo *philo)
{
	pthread_mutex_lock(&philo->tab->mutex);
	if (philo->hungry)
	{
		pthread_mutex_unlock(&philo->tab->mutex);
		return (1);
	}
	pthread_mutex_unlock(&philo->tab->mutex);
	return (0);
}

int	one_philo(t_philo *philo)
{
	if (philo->tab->number_of_philosophers == 1)
	{
		pthread_mutex_lock(&philo->tab->mutex);
		printf("0 1 has taken a fork\n");
		pthread_mutex_unlock(&philo->tab->mutex);
		usleep(philo->tab->time_to_die * 1000);
		return (1);
	}
	return (0);
}

void	*philosophers_routine(void *arguments)
{
	t_philo	*philo;

	philo = (t_philo *)arguments;
	if (one_philo(philo))
		return (NULL);
	while (philo->tab->philosopher_dead == 0 && is_hungry(philo))
	{
		if (philosopher_takes_forks(philo))
			return (0);
		if (philosopher_is_dead(philo))
		{
			pthread_mutex_unlock(philo->left_fork);
			pthread_mutex_unlock(philo->right_fork);
			return (0);
		}
		philosophers_is_eating(philo);
		philosophers_is_sleeping(philo);
		philosophers_is_thinking(philo);
	}
	return (0);
}

void	join_threads(t_table *data)
{
	int	i;

	i = 0;
	while (i < data->number_of_philosophers)
	{
		pthread_join(data->philosophers[i].thread_id, NULL);
		i++;
	}
}

void	begin_philosophers_routine(t_table *data)
{
	int	i;

	i = 0;
	data->starting_time = get_time();
	while (i < data->number_of_philosophers)
	{
		pthread_create(&data->philosophers[i].thread_id, NULL,
			&philosophers_routine, (void *)&data->philosophers[i]);
		i++;
	}
}

void	print_message(char *str, t_philo *philosopher)
{
	if (!philosopher_is_dead(philosopher))
	{
		pthread_mutex_lock(&philosopher->tab->mutex);
		printf("%d %d %s\n", get_time()
			- philosopher->tab->starting_time, philosopher->id, str);
		pthread_mutex_unlock(&philosopher->tab->mutex);
	}
}

int	get_time(void)
{
	static struct timeval	t;

	gettimeofday(&t, NULL);
	return ((t.tv_sec * 1000) + (t.tv_usec / 1000));
}

void	ft_sleep(int time, t_philo *philosophers)
{
	int	start;

	start = get_time();
	while ((get_time() - start) < time
		&& philosopher_is_dead(philosophers) == FALSE)
		usleep(50);
}

void	ft_putstr(char *str)
{
	int	i;

	i = -1;
	while (str[++i])
		write(1, &str[i], 1);
}

void	print_limits_error(void)
{
	ft_putstr("\nNo arguments - should be above the int limits!\n\n");
}

void	print_right_syntax(void)
{
	ft_putstr("\n./philo[philos][death_time][eating_time][sleeping_time]\n\n");
}

int	ft_atoi(const char *str)
{
	int	is_atoi;
	int	is_negative;
	int	i;

	is_atoi = 0;
	is_negative = 1;
	i = 0;
	while (str[i])
	{
		while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
			i++;
		if (str[i] == '+' || str[i] == '-')
		{
			if (str[i] == '-')
				is_negative = -1;
			i++;
		}
		while (str[i] >= '0' && str[i] <= '9')
		{
			is_atoi = (is_atoi * 10) + str[i] - 48;
			i++;
		}
		return (is_atoi * is_negative);
	}
	return (0);
}

long	ft_atoi_long(const char *str)
{
	long	long_atoi;
	long	is_negative;
	int		i;

	long_atoi = 0;
	is_negative = 1;
	i = 0;
	while (str[i])
	{
		while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
			i++;
		if (str[i] == '+' || str[i] == '-')
		{
			if (str[i] == '-')
				is_negative = -1;
			i++;
		}
		while (str[i] >= '0' && str[i] <= '9')
		{
			long_atoi = (long_atoi * 10) + str[i] - 48;
			i++;
		}
		return (long_atoi * is_negative);
	}
	return (0);
}

long	ft_atol(const char *str)
{
	long	result;
	int		sign;

	result = 0;
	sign = 1;
	while (*str == ' ' || (*str >= '\t' && *str <= '\r'))
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

int	is_positive_integer(const char *str)
{
	if (*str == '\0')
		return (0);
	while (*str)
	{
		if (*str < '0' || *str > '9')
			return (0);
		str++;
	}
	return (1);
}

int	validate_arguments(int argc, char **argv)
{
	int	i;

	i = 1;
	if (argc != 5 && argc != 6)
	{
		printf("Usage: %s num_philos time_to_die ", argv[0]);
		printf("time_to_eat time_to_sleep [meals_required]\n");
		return (1);
	}
	while (i < argc)
	{
		if (!is_positive_integer(argv[i]))
		{
			printf("Error: All args must be positive integers.\n");
			return (1);
		}
		i++;
	}
	return (0);
}

int	main(int argc, char **argv)
{
	t_table	table;

	if (!validate_arguments(argc, argv))
	{
		initialize_table(&table, argv);
		begin_philosophers_routine(&table);
		begin_monitoring(&table);
		join_threads(&table);
		free_philosophers(&table);
	}
	return (0);
}