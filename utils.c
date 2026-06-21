/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaizan <mhaizan@student.42amman.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/18 00:00:00 by mhaizan           #+#    #+#             */
/*   Updated: 2026/06/18 17:57:30 by mhaizan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((long long)tv.tv_sec * 1000 + (long long)tv.tv_usec / 1000);
}

long long	get_time_us(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((long long)tv.tv_sec * 1000000 + (long long)tv.tv_usec);
}

void	ft_usleep(long long ms, t_data *data)
{
	long long	start;

	start = get_time_ms();
	while ((get_time_ms() - start) < ms)
	{
		if (is_sim_over(data))
			break ;
		usleep(500);
	}
}

void	print_state(t_philo *philo, const char *state)
{
	long long	ts;

	pthread_mutex_lock(&philo->data->print_mutex);
	if (!is_sim_over(philo->data))
	{
		ts = get_time_ms() - philo->data->start_time;
		printf("%lld %d %s\n", ts, philo->id, state);
	}
	pthread_mutex_unlock(&philo->data->print_mutex);
}

int	is_sim_over(t_data *data)
{
	int	result;

	pthread_mutex_lock(&data->data_mutex);
	result = data->sim_over;
	pthread_mutex_unlock(&data->data_mutex);
	return (result);
}
