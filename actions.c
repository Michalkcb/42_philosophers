/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbany <mbany@student.42warsaw.pl>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 19:54:12 by mbany             #+#    #+#             */
/*   Updated: 2024/12/05 19:45:23 by mbany            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// // Own version of atoi
// int	ft_atoi(char *str)
// {
// 	unsigned long long	nb;
// 	int					sign;
// 	int					i;

// 	nb = 0;
// 	sign = 1;
// 	i = 0;
// 	while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n' || str[i] == '\v'
// 		|| str[i] == '\f' || str[i] == '\r')
// 		i++;
// 	if (str[i] == '-')
// 		sign = -1;
// 	if (str[i] == '-' || str[i] == '+')
// 		i++;
// 	while (str[i] >= '0' && str[i] <= '9')
// 	{
// 		nb = nb * 10 + (str[i] - '0');
// 		i++;
// 	}
// 	return (sign * nb);
// }

// // Checks that the input is only numbers
// int	check_arg_content(char *arg)
// {
// 	int	i;

// 	i = 0;
// 	while (arg[i] != '\0')
// 	{
// 		if (arg[i] < '0' || arg[i] > '9')
// 			return (1);
// 		i++;
// 	}
// 	return (0);
// }

// // Checks if the program input is correct
// int	check_valid_args(char **argv)
// {
// 	if (ft_atoi(argv[1]) > PHILO_MAX || ft_atoi(argv[1]) <= 0
// 		|| check_arg_content(argv[1]) == 1)
// 		return (write(2, "Invalid philosophers number\n", 29), 1);
// 	if (ft_atoi(argv[2]) <= 0 || check_arg_content(argv[2]) == 1)
// 		return (write(2, "Invalid time to die\n", 21), 1);
// 	if (ft_atoi(argv[3]) <= 0 || check_arg_content(argv[3]) == 1)
// 		return (write(2, "Invalid time to eat\n", 21), 1);
// 	if (ft_atoi(argv[4]) <= 0 || check_arg_content(argv[4]) == 1)
// 		return (write(2, "Invalid time to sleep\n", 23), 1);
// 	if (argv[5] && (ft_atoi(argv[5]) < 0 || check_arg_content(argv[5]) == 1))
// 		return (write(2, "Invalid number of times each philosopher must eat\n",
// 				51), 1);
// 	return (0);
// }

// // Checks if the value of dead_flag changed
// int	dead_loop(t_philo *philo)
// {
// 	pthread_mutex_lock(philo->dead_lock);
// 	if (*philo->dead == 1)
// 		return (pthread_mutex_unlock(philo->dead_lock), 1);
// 	pthread_mutex_unlock(philo->dead_lock);
// 	return (0);
// }

// // Thread routine
// void	*philo_routine(void *pointer)
// {
// 	t_philo	*philo;

// 	philo = (t_philo *)pointer;
// 	if (philo->id % 2 == 0)
// 		ft_usleep(1);
// 	while (!dead_loop(philo))
// 	{
// 		eat(philo);
// 		dream(philo);
// 		think(philo);
// 	}
// 	return (pointer);
// }

// // Creates all the threads
// int	thread_create(t_program *program, pthread_mutex_t *forks)
// {
// 	pthread_t	observer;
// 	int			i;

// 	if (pthread_create(&observer, NULL, &monitor, program->philos) != 0)
// 		destory_all("Thread creation error", program, forks);
// 	i = 0;
// 	while (i < program->philos[0].num_of_philos)
// 	{
// 		if (pthread_create(&program->philos[i].thread, NULL, &philo_routine,
// 				&program->philos[i]) != 0)
// 			destory_all("Thread creation error", program, forks);
// 		i++;
// 	}
// 	i = 0;
// 	if (pthread_join(observer, NULL) != 0)
// 		destory_all("Thread join error", program, forks);
// 	while (i < program->philos[0].num_of_philos)
// 	{
// 		if (pthread_join(program->philos[i].thread, NULL) != 0)
// 			destory_all("Thread join error", program, forks);
// 		i++;
// 	}
// 	return (0);
// }

// Think routine funtion
void	think(t_philo *philo)
{
	print_message("is thinking", philo, philo->id);
}

// Dream routine funtion
void	dream(t_philo *philo)
{
	print_message("is sleeping", philo, philo->id);
	ft_usleep(philo->time_to_sleep);
}

// Eat routine funtion
void	eat(t_philo *philo)
{
	pthread_mutex_lock(philo->r_fork);
	print_message("has taken a fork", philo, philo->id);
	if (philo->num_of_philos == 1)
	{
		ft_usleep(philo->time_to_die);
		pthread_mutex_unlock(philo->r_fork);
		return ;
	}
	pthread_mutex_lock(philo->l_fork);
	print_message("has taken a fork", philo, philo->id);
	philo->eating = 1;
	print_message("is eating", philo, philo->id);
	pthread_mutex_lock(philo->meal_lock);
	philo->last_meal = get_current_time();
	philo->meals_eaten++;
	pthread_mutex_unlock(philo->meal_lock);
	ft_usleep(philo->time_to_eat);
	philo->eating = 0;
	pthread_mutex_unlock(philo->l_fork);
	pthread_mutex_unlock(philo->r_fork);
	
}

// // Print message funtion
// void	print_message(char *str, t_philo *philo, int id)
// {
// 	size_t	time;

// 	pthread_mutex_lock(philo->write_lock);
// 	time = get_current_time() - philo->start_time;
// 	if (!dead_loop(philo))
// 		printf("%zu %d %s\n", time, id, str);
// 	pthread_mutex_unlock(philo->write_lock);
// }

// // Checks if the philosopher is dead
// int	philosopher_dead(t_philo *philo, size_t time_to_die)
// {
// 	pthread_mutex_lock(philo->meal_lock);
// 	if (get_current_time() - philo->last_meal >= time_to_die
// 		&& philo->eating == 0)
// 		return (pthread_mutex_unlock(philo->meal_lock), 1);
// 	pthread_mutex_unlock(philo->meal_lock);
// 	return (0);
// }
// // Check if any philo died
// int	check_if_dead(t_philo *philos)
// {
// 	int	i;

// 	i = 0;
// 	while (i < philos[0].num_of_philos)
// 	{
// 		if (philosopher_dead(&philos[i], philos[i].time_to_die))
// 		{
// 			print_message("died", &philos[i], philos[i].id);
// 			pthread_mutex_lock(philos[0].dead_lock);
// 			*philos->dead = 1;
// 			pthread_mutex_unlock(philos[0].dead_lock);
// 			return (1);
// 		}
// 		i++;
// 	}
// 	return (0);
// }

// // Checks if all the philos ate the num_of_meals
// int	check_if_all_ate(t_philo *philos)
// {
// 	int	i;
// 	int	finished_eating;

// 	i = 0;
// 	finished_eating = 0;
// 	if (philos[0].num_times_to_eat == -1)
// 		return (0);
// 	while (i < philos[0].num_of_philos)
// 	{
// 		pthread_mutex_lock(philos[i].meal_lock);
// 		if (philos[i].meals_eaten >= philos[i].num_times_to_eat)
// 			finished_eating++;
// 		pthread_mutex_unlock(philos[i].meal_lock);
// 		i++;
// 	}
// 	if (finished_eating == philos[0].num_of_philos)
// 	{
// 		pthread_mutex_lock(philos[0].dead_lock);
// 		*philos->dead = 1;
// 		pthread_mutex_unlock(philos[0].dead_lock);
// 		return (1);
// 	}
// 	return (0);
// }

// Monitor thread routine
// void	*monitor(void *pointer)
// {
// 	t_philo	*philos;

// 	philos = (t_philo *)pointer;
// 	while (1)
// 		if (check_if_dead(philos) == 1 || check_if_all_ate(philos) == 1)
// 			break ;
// 	return (pointer);
// }

// // Checks the len of the string
// int	ft_strlen(char *str)
// {
// 	int	i;

// 	if (str == NULL)
// 		return (0);
// 	i = 0;
// 	while (str[i] != '\0')
// 		i++;
// 	return (i);
// }

// // Destroys all the mutexes
// void	destory_all(char *str, t_program *program, pthread_mutex_t *forks)
// {
// 	int	i;

// 	i = 0;
// 	if (str)
// 	{
// 		write(2, str, ft_strlen(str));
// 		write(2, "\n", 1);
// 	}
// 	pthread_mutex_destroy(&program->write_lock);
// 	pthread_mutex_destroy(&program->meal_lock);
// 	pthread_mutex_destroy(&program->dead_lock);
// 	while (i < program->philos[0].num_of_philos)
// 	{
// 		pthread_mutex_destroy(&forks[i]);
// 		i++;
// 	}
// }

// // Improved version of sleep function
// int	ft_usleep(size_t milliseconds)
// {
// 	size_t	start;

// 	start = get_current_time();
// 	while ((get_current_time() - start) < milliseconds)
// 		usleep(500);
// 	return (0);
// }

// // Gets the current time in milliseconds
// size_t	get_current_time(void)
// {
// 	struct timeval	time;

// 	if (gettimeofday(&time, NULL) == -1)
// 		write(2, "gettimeofday() error\n", 22);
// 	return (time.tv_sec * 1000 + time.tv_usec / 1000);
// }

// // Initializing the input from user
// void	init_input(t_philo *philo, char **argv)
// {
// 	philo->time_to_die = ft_atoi(argv[2]);
// 	philo->time_to_eat = ft_atoi(argv[3]);
// 	philo->time_to_sleep = ft_atoi(argv[4]);
// 	philo->num_of_philos = ft_atoi(argv[1]);
// 	if (argv[5])
// 		philo->num_times_to_eat = ft_atoi(argv[5]);
// 	else
// 		philo->num_times_to_eat = -1;
// }

// // Inicjalizacja filozofów
// void	init_philos(t_philo *philos, t_program *program, pthread_mutex_t *forks,
// 		char **argv)
// {
// 	int	i;

// 	i = 0; // Inicjalizuje licznik pętli, aby przechodzić przez wszystkich filozofów
// 	while (i < ft_atoi(argv[1])) // Iteruje tyle razy, ile podano filozofów w argv[1]
// 	{
// 		philos[i].id = i + 1; // Przypisuje unikalne ID każdemu filozofowi (zaczynając od 1)
// 		philos[i].eating = 0; // Ustawia status jedzenia na 0 (nie je)
// 		philos[i].meals_eaten = 0; // Ustawia liczbę zjedzonych posiłków na 0
// 		init_input(&philos[i], argv); // Uzupełnia pozostałe dane wejściowe jak czas życia, jedzenia, spania
// 		philos[i].start_time = get_current_time(); // Ustawia czas rozpoczęcia symulacji
// 		philos[i].last_meal = get_current_time(); // Ustawia czas ostatniego posiłku jako czas startowy
// 		philos[i].write_lock = &program->write_lock; // Łączy filozofa z globalnym zamkiem na wyjście (synchronizacja wypisywania komunikatów)
// 		philos[i].dead_lock = &program->dead_lock; // Łączy filozofa z globalnym zamkiem na status śmierci
// 		philos[i].meal_lock = &program->meal_lock; // Łączy filozofa z zamkiem chroniącym dane o posiłkach
// 		philos[i].dead = &program->dead_flag; // Wskazuje na wspólną flagę określającą, czy któryś filozof umarł
// 		philos[i].l_fork = &forks[i]; // Przypisuje lewy widelec (aktualny indeks w tablicy widelców)
// 		if (i == 0) // Specjalny przypadek dla pierwszego filozofa
// 			philos[i].r_fork = &forks[philos[i].num_of_philos - 1]; // Jego prawy widelec to ostatni widelec (okrągły stół)
// 		else
// 			philos[i].r_fork = &forks[i - 1]; // Dla innych filozofów prawy widelec to poprzedni indeks w tablicy widelców
// 		i++; // Przejście do następnego filozofa
// 	}
// }



// // Initializing the forks mutexes
// void	init_forks(pthread_mutex_t *forks, int philo_num)
// {
// 	int	i;

// 	i = 0;
// 	while (i < philo_num)
// 	{
// 		pthread_mutex_init(&forks[i], NULL);
// 		i++;
// 	}
// }

// // Initializing the program structure
// void	init_program(t_program *program, t_philo *philos)
// {
// 	program->dead_flag = 0;
// 	program->philos = philos;
// 	pthread_mutex_init(&program->write_lock, NULL);
// 	pthread_mutex_init(&program->dead_lock, NULL);
// 	pthread_mutex_init(&program->meal_lock, NULL);
// }
// // Main function
// int	main(int argc, char **argv)
// {
// 	t_program		program;
// 	t_philo			philos[PHILO_MAX];
// 	pthread_mutex_t	forks[PHILO_MAX];

// 	if (argc != 5 && argc != 6)
// 		return (write(2, "Wrong argument count\n", 22), 1);
// 	if (check_valid_args(argv) == 1)
// 		return (1);
// 	init_program(&program, philos);
// 	init_forks(forks, ft_atoi(argv[1]));
// 	init_philos(philos, &program, forks, argv);
// 	thread_create(&program, forks);
// 	destory_all(NULL, &program, forks);
// 	return (0);
// }
