/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaizan <mhaizan@student.42amman.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/18 00:00:00 by mhaizan           #+#    #+#             */
/*   Updated: 2026/06/21 14:51:47 by mhaizan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (1)
	{
		pthread_mutex_lock(&philo->data->data_mutex);
		if (philo->data->ready)
			break ;
		pthread_mutex_unlock(&philo->data->data_mutex);
		usleep(100);
	}
	pthread_mutex_unlock(&philo->data->data_mutex);
	if (philo->id % 2 == 0)
		ft_usleep(philo->data->time_to_eat / 10 + 1, philo->data);
	while (!is_sim_over(philo->data))
	{
		philo_eat(philo);
		if (is_sim_over(philo->data))
			break ;
		philo_sleep(philo);
		if (is_sim_over(philo->data))
			break ;
		philo_think(philo);
	}
	return (NULL);
}

static void	join_all(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
		pthread_join(data->philos[i++].thread, NULL);
}

static int	create_philos(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		if (pthread_create(&data->philos[i].thread, NULL,
				philo_routine, &data->philos[i]) != 0)
		{
			pthread_mutex_lock(&data->print_mutex);
			data->sim_over = 1;
			pthread_mutex_unlock(&data->print_mutex);
			while (--i >= 0)
				pthread_join(data->philos[i].thread, NULL);
			return (0);
		}
		i++;
	}
	return (1);
}

static void	set_start_time(t_data *data)
{
	int	i;

	pthread_mutex_lock(&data->data_mutex);
	data->start_time = get_time_ms();
	i = 0;
	while (i < data->num_philos)
	{
		pthread_mutex_lock(&data->philos[i].meal_mutex);
		data->philos[i].last_meal_time = data->start_time;
		pthread_mutex_unlock(&data->philos[i].meal_mutex);
		i++;
	}
	data->ready = 1;
	pthread_mutex_unlock(&data->data_mutex);
}

int	start_simulation(t_data *data)
{
	pthread_t	monitor;

	if (data->num_philos == 1)
		return (handle_single_philo(data));
	if (!create_philos(data))
		return (0);
	set_start_time(data);
	if (pthread_create(&monitor, NULL, monitor_routine, data) != 0)
	{
		pthread_mutex_lock(&data->print_mutex);
		data->sim_over = 1;
		pthread_mutex_unlock(&data->print_mutex);
		join_all(data);
		return (0);
	}
	pthread_join(monitor, NULL);
	join_all(data);
	return (1);
}
