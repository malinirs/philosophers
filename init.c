/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoods <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/23 22:14:20 by awoods            #+#    #+#             */
/*   Updated: 2021/08/23 22:22:01 by                  ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	break_down(t_philo **phil)
{
	int	i;

	i = 0;
	while (i < phil[0]->tabl->philo)
	{
		pthread_mutex_destroy(&phil[i]->right);
		i++;
	}
	pthread_mutex_destroy(&phil[0]->tabl->sms);
	pthread_mutex_destroy(&phil[0]->tabl->death);
}

void	init(t_table *table, int count)
{
	t_philo		**phil;

	phil = malloc(sizeof(t_philo) * table->philo);
	if (!phil)
		ft_trouble(1);
	while (count < table->philo)
	{
		phil[count] = malloc(sizeof(t_philo));
		if (!phil[count])
			ft_trouble(1);
		phil[count]->index_phil = count + 1;
		phil[count]->tabl = table;
		pthread_mutex_init(&phil[count]->right, NULL);
		phil[count]->eat_start = table->time_start;
		phil[count]->i_eat = 0;
		count++;
	}
	count = 0;
	while (count < table->philo)
	{
		phil[count]->left = &phil[count - 1]->right;
		count++;
	}
	phil[0]->left = &phil[count - 1]->right;
	create_pthread(phil, table);
}
