/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoods <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/19 12:51:00 by awoods            #+#    #+#             */
/*   Updated: 2021/08/19 12:51:02 by awoods           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <pthread.h>
# include <stdlib.h>
# include <sys/time.h>

#include <stdio.h>


typedef struct s_table
{
	int	philo;
	int	time_life; //общее время до смерти
	int	time_eat;
	int	time_sleep;
	int	eat; //кол-во приемов еды

	int	count_eat; //счетчик для кол-ва приемов пищи
	pthread_mutex_t	sms; //на сообщение
	pthread_mutex_t	death; //на смерть
	pthread_t		thread_death; // поток для мониторинга смерти
	unsigned long	time_start; // время начала симуляции
}			t_table;

typedef struct s_philo
{
	pthread_mutex_t	*left; //на левую вилку
	pthread_mutex_t	right; //на правую вилку
	pthread_t 		thread_phil; //поток на философов
	int				index_phil; //индекс философа
	int				i_eat; //счетчик кол-ва приемов пищи на 1 фила
	unsigned long	eat_start; // время начала приема пищи
	t_table	*tabl;
}			t_philo;

int	ft_atoi(const char *str);

#endif
