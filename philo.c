/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoods <awoods@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/23 18:40:12 by awoods            #+#    #+#             */
/*   Updated: 2021/08/23 22:25:25 by                  ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*start_thread(void *phil)
{
	t_philo	*temp;

	temp = phil;
	pthread_detach(temp->thread_phil);
	while (1)
	{
		touch_fork(temp);
		temp->eat_start = get_time();
		send_sms(temp, "eatting");
		temp->i_eat++;
		if (temp->i_eat == temp->tabl->eat)
			temp->tabl->count_eat++;
		usleep_mod(temp->tabl->time_eat);
		if (pthread_mutex_unlock(&temp->right) || \
		pthread_mutex_unlock(temp->left))
			ft_trouble(3);
		send_sms(temp, "sleeping");
		usleep_mod(temp->tabl->time_sleep);
		send_sms(temp, "thinking");
	}
}

void	*cheack_death(t_philo	**temp, int count, int death)
{
	if (temp[count]->tabl->count_eat == temp[count]->tabl->philo)
	{
		pthread_mutex_lock(&temp[count]->tabl->sms);
		printf("%s\n", "All the philosophers finished their lunch!");
		pthread_mutex_unlock(&temp[count]->tabl->sms);
		pthread_mutex_lock(&temp[count]->tabl->sms);
		return (NULL);
	}
	if ((int)(get_time() - temp[count]->eat_start) > \
	temp[count]->tabl->time_life || (int)get_time() > death)
	{
		pthread_mutex_lock(&temp[count]->tabl->sms);
		printf("%lu %d %s\n", get_time() - temp[count]->tabl->time_start,
			   temp[count]->index_phil, "DEAD");
		usleep (1001);
		pthread_mutex_unlock(&temp[count]->tabl->sms);
		pthread_mutex_lock(&temp[count]->tabl->sms);
		return (NULL);
	}
	return ((void *)1);
}

void	*death(void *philo)
{
	t_philo			**temp;
	unsigned long	death;
	int				count;

	temp = philo;
	while (1)
	{
		count = 0;
		while (count < temp[0]->tabl->philo)
		{
			death = temp[count]->tabl->time_life + temp[count]->eat_start;
			if (cheack_death(temp, count, death) == NULL)
				return (NULL);
			count++;
		}
	}
}

void	create_pthread(t_philo **phil, t_table *table)
{
	int	count;

	count = 0;
	pthread_mutex_lock(&table->death);
	pthread_create(&table->thread_death, NULL, death, phil);
	while (count < table->philo)
	{
		pthread_create(&phil[count]->thread_phil, NULL, start_thread, \
					   phil[count]);
		count++;
		usleep(10);
	}
	pthread_join(table->thread_death, NULL);
	break_down(phil);
	usleep(5000);
	free(phil);
}
