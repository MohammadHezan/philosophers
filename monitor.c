/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaizan <mhaizan@student.42amman.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/18 00:00:00 by mhaizan           #+#    #+#             */
/*   Updated: 2026/06/18 17:54:30 by mhaizan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	philo_is_dead(t_data *data, int i, long long *ts)
{
	long long	elapsed;
	int			dead;

	pthread_mutex_lock(&data->data_mutex);
	elapsed = get_time_ms() - data->philos[i].last_meal_time;
	dead = (elapsed >= data->time_to_die);
	if (dead)
		*ts = get_time_ms() - data->start_time;
	pthread_mutex_unlock(&data->data_mutex);
	return (dead);
}

static int	all_ate_enough(t_data *data)
{
	int	i;

	if (data->must_eat_count == -1)
		return (0);
	pthread_mutex_lock(&data->data_mutex);
	i = 0;
	while (i < data->num_philos)
	{
		if (data->philos[i].meals_eaten < data->must_eat_count)
		{
			pthread_mutex_unlock(&data->data_mutex);
			return (0);
		}
		i++;
	}
	pthread_mutex_unlock(&data->data_mutex);
	return (1);
}

static int	check_death(t_data *data, int i)
{
	long long	ts;

	if (!philo_is_dead(data, i, &ts))
		return (0);
	pthread_mutex_lock(&data->print_mutex);
	data->sim_over = 1;
	printf("%lld %d died\n", ts, data->philos[i].id);
	pthread_mutex_unlock(&data->print_mutex);
	return (1);
}

static int	check_all_ate(t_data *data)
{
	if (!all_ate_enough(data))
		return (0);
	pthread_mutex_lock(&data->print_mutex);
	data->sim_over = 1;
	pthread_mutex_unlock(&data->print_mutex);
	return (1);
}

void	*monitor_routine(void *arg)
{
	t_data	*data;
	int		i;

	data = (t_data *)arg;
	while (1)
	{
		if (check_all_ate(data))
			return (NULL);
		i = 0;
		while (i < data->num_philos)
		{
			if (check_death(data, i))
				return (NULL);
			i++;
		}
		usleep(1000);
	}
	return (NULL);
}
