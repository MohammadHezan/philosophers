/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaizan <mhaizan@student.42amman.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/18 00:00:00 by mhaizan           #+#    #+#             */
/*   Updated: 2026/06/18 17:54:30 by mhaizan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	single_philo(t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork);
	print_state(philo, "has taken a fork");
	ft_usleep(philo->data->time_to_die + 1, philo->data);
	pthread_mutex_unlock(philo->left_fork);
}

static void	take_forks(t_philo *philo)
{
	if (philo->id == philo->data->num_philos)
	{
		pthread_mutex_lock(philo->right_fork);
		print_state(philo, "has taken a fork");
		pthread_mutex_lock(philo->left_fork);
	}
	else
	{
		pthread_mutex_lock(philo->left_fork);
		print_state(philo, "has taken a fork");
		pthread_mutex_lock(philo->right_fork);
	}
	print_state(philo, "has taken a fork");
}

void	philo_eat(t_philo *philo)
{
	if (philo->data->num_philos == 1)
	{
		single_philo(philo);
		return ;
	}
	take_forks(philo);
	pthread_mutex_lock(&philo->meal_mutex);
	philo->last_meal_time = get_time_ms();
	pthread_mutex_unlock(&philo->meal_mutex);
	print_state(philo, "is eating");
	pthread_mutex_lock(&philo->meal_mutex);
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->meal_mutex);
	ft_usleep(philo->data->time_to_eat, philo->data);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}

void	philo_sleep(t_philo *philo)
{
	print_state(philo, "is sleeping");
	ft_usleep(philo->data->time_to_sleep, philo->data);
}

void	philo_think(t_philo *philo)
{
	long long	t_eat;
	long long	t_sleep;
	long long	t_think;

	print_state(philo, "is thinking");
	if (philo->data->num_philos % 2 == 0)
		return ;
	t_eat = philo->data->time_to_eat;
	t_sleep = philo->data->time_to_sleep;
	t_think = t_eat - t_sleep;
	if (t_think < 0)
		t_think = 0;
	ft_usleep(t_think + 5, philo->data);
}
