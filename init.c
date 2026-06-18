/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaizan <mhaizan@student.42amman.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/18 00:00:00 by mhaizan           #+#    #+#             */
/*   Updated: 2026/06/18 17:57:15 by mhaizan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	fill_data(t_data *data, char **argv, int argc)
{
	data->num_philos = (int)ft_atoll(argv[1]);
	data->time_to_die = ft_atoll(argv[2]);
	data->time_to_eat = ft_atoll(argv[3]);
	data->time_to_sleep = ft_atoll(argv[4]);
	data->must_eat_count = -1;
	if (argc == 6)
		data->must_eat_count = (int)ft_atoll(argv[5]);
}

int	parse_args(int argc, char **argv, t_data *data)
{
	int			i;
	long long	val;

	if (argc < 5 || argc > 6)
	{
		write(2, "Error\n", 6);
		return (0);
	}
	i = 1;
	while (i < argc)
	{
		val = ft_atoll(argv[i]);
		if (val <= 0)
		{
			write(2, "Error\n", 6);
			return (0);
		}
		i++;
	}
	fill_data(data, argv, argc);
	return (1);
}

static int	init_forks(t_data *data)
{
	int	i;

	data->forks = malloc(sizeof(pthread_mutex_t) * data->num_philos);
	if (!data->forks)
		return (0);
	i = 0;
	while (i < data->num_philos)
	{
		if (pthread_mutex_init(&data->forks[i], NULL) != 0)
		{
			while (--i >= 0)
				pthread_mutex_destroy(&data->forks[i]);
			free(data->forks);
			data->forks = NULL;
			return (0);
		}
		i++;
	}
	return (1);
}

static int	init_philos(t_data *data)
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
		data->philos[i].data = data;
		data->philos[i].left_fork = &data->forks[i];
		data->philos[i].right_fork
			= &data->forks[(i + 1) % data->num_philos];
		i++;
	}
	return (1);
}

int	init_data(t_data *data)
{
	data->sim_over = 0;
	data->start_time = get_time_ms();
	if (pthread_mutex_init(&data->print_mutex, NULL) != 0)
		return (0);
	if (pthread_mutex_init(&data->data_mutex, NULL) != 0)
	{
		pthread_mutex_destroy(&data->print_mutex);
		return (0);
	}
	if (!init_forks(data))
	{
		pthread_mutex_destroy(&data->print_mutex);
		pthread_mutex_destroy(&data->data_mutex);
		return (0);
	}
	if (!init_philos(data))
	{
		destroy_data(data);
		return (0);
	}
	return (1);
}
