/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   touch_fork.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoods <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/23 22:27:17 by awoods            #+#    #+#             */
/*   Updated: 2021/08/23 22:27:21 by awoods           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_trouble(int number)
{
	if (number == 0)
		write(2, "Error: wrong number of arguments\n", 33);
	else if (number == 1)
		write(2, "Error: memory was not allocated\n", 32);
	else if (number == 2)
		write(2, "Error: arguments are incorrectly filled\n", 40);
	else if (number == 3)
		write(2, "Error: problem with pthread\n", 27);
	else if (number == 4)
		write(2, "Error: problem with time\n", 25);
}

unsigned long	get_time(void)
{
	struct timeval	time;

	if (0 > gettimeofday(&time, NULL))
		ft_trouble(4);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

void	usleep_mod(unsigned long time)
{
	unsigned long	death;

	death = get_time() + time;
	while (get_time() < death)
		usleep(100);
}

void	*send_sms(t_philo *philo, char *str)
{
	pthread_mutex_lock(&philo->tabl->sms);
	printf("%lu %d %s\n", get_time() - philo->tabl->time_start, \
	philo->index_phil, str);
	pthread_mutex_unlock(&philo->tabl->sms);
	return (NULL);
}

void	touch_fork(t_philo *philo)
{
	if (philo->index_phil % 2)
	{
		if (pthread_mutex_lock(&philo->right))
			ft_trouble(3);
		send_sms(philo, "taken a fork");
		if (pthread_mutex_lock(philo->left))
			ft_trouble(3);
		send_sms(philo, "taken a fork");
	}
	else
	{
		if (pthread_mutex_lock(philo->left))
			ft_trouble(3);
		send_sms(philo, "taken a fork");
		if (pthread_mutex_lock(&philo->right))
			ft_trouble(3);
		send_sms(philo, "taken a fork");
	}
}
