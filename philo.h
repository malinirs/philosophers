/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoods <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/23 22:29:45 by awoods            #+#    #+#             */
/*   Updated: 2021/08/23 22:29:48 by awoods           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# include <unistd.h>
# include <pthread.h>
# include <stdlib.h>
# include <sys/time.h>
# include <stdio.h>

typedef struct s_table
{
	int				philo;
	int				time_life;
	int				time_eat;
	int				time_sleep;
	int				eat;

	int				count_eat;
	pthread_mutex_t	sms;
	pthread_mutex_t	death;
	pthread_t		thread_death;
	unsigned long	time_start;
}			t_table;

typedef struct s_philo
{
	pthread_mutex_t	*left;
	pthread_mutex_t	right;
	pthread_t		thread_phil;
	int				index_phil;
	int				i_eat;
	unsigned long	eat_start;
	t_table			*tabl;
}			t_philo;

void			ft_trouble(int number);
void			init(t_table *table, int count);
void			create_pthread(t_philo **phil, t_table *table);
unsigned long	get_time(void);
void			touch_fork(t_philo *philo);
void			*send_sms(t_philo *philo, char *str);
void			usleep_mod(unsigned long time);
void			break_down(t_philo **phil);

#endif
