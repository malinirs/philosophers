/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoods <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/19 12:50:23 by awoods            #+#    #+#             */
/*   Updated: 2021/08/19 12:50:26 by awoods           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_close(int number)
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

unsigned long	get_hour(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) < 0)
		ft_close(4);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

unsigned long	time_d(t_philo *philo)
{
	unsigned long	time;

	time = get_hour() - philo->tabl->time_start;
	return (time);
}

unsigned long	time_d_e(t_philo *philo)
{
	unsigned long	time;

	time = get_hour() - philo->eat_start;
	return (time);
}

void	usleep_mod(unsigned long eat)
{
	unsigned long	end;

	end = get_hour() + eat;
	while (get_hour() < end)
		usleep(100);
}

void	*send_sms(t_philo *philo, char *ptr, int count)
{
	if (count == 0)
	{
		if (pthread_mutex_lock(&philo->tabl->sms))
			ft_close(3);
		printf("%lu %d %s\n", time_d(philo), philo->index_phil, ptr);
		if (pthread_mutex_unlock(&philo->tabl->sms))
			ft_close(3);
		return (NULL);
	}
	else if (count == 1)
	{
		if (pthread_mutex_lock(&philo->tabl->sms))
			ft_close(3);
		printf("️%lu %d %s\n", time_d(philo), philo->index_phil, ptr);
		usleep (1001);
		if (pthread_mutex_unlock(&philo->tabl->death))
			ft_close(3);
		return (NULL);
	}
	else if (count == 2)
	{
		if (pthread_mutex_lock(&philo->tabl->sms))
			ft_close(3);
		printf("%s\n", ptr);
		if (pthread_mutex_unlock(&philo->tabl->death))
			ft_close(3);
		return (NULL);
	}
	return (0);
}

void	touch_fork(t_philo *philo)
{
	if (philo->index_phil % 2)
	{
		if (pthread_mutex_lock(&philo->right))
			ft_close(3);
		send_sms(philo, "taken a fork", 0);
		if (pthread_mutex_lock(philo->left))
			ft_close(3);
		send_sms(philo, "taken a fork", 0);
	}
	else
	{
		if (pthread_mutex_lock(philo->left))
			ft_close(3);
		send_sms(philo, "taken a fork", 0);
		if (pthread_mutex_lock(&philo->right))
			ft_close(3);
		send_sms(philo, "taken a fork", 0);
	}
}

void	*start_thread(void *phil)
{
	t_philo 	*temp;

	temp = phil;
	pthread_detach(temp->thread_phil);
	while (1)
	{
		touch_fork(temp);
		temp->eat_start = get_hour();
		send_sms(temp, "eatting", 0);
		temp->i_eat++;
		if (temp->i_eat == temp->tabl->eat)
			temp->tabl->time_eat++;
		usleep_mod(temp->tabl->time_eat);
		if (pthread_mutex_unlock(&temp->right) || \
		pthread_mutex_unlock(temp->left))
			ft_close(3);
		send_sms(temp, "sleeping", 0);
		usleep_mod(temp->tabl->time_sleep);
		send_sms(temp, "thinking", 0);
	}
}

void	*death(void *phil)
{
	t_philo	**temp;
	unsigned long	time_death;
	int				i;

	temp = phil;
	while (1)
	{
		i = -1;
		while (++i < temp[0]->tabl->philo)
		{
			time_death = temp[i]->tabl->time_life + temp[i]->eat_start;
			if (temp[i]->tabl->count_eat == temp[i]->tabl->philo)
			{
				send_sms(temp[i], "Finished eating!", 2);
				return (NULL);
			}
			if ((int)time_d_e(temp[i]) > temp[i]->tabl->time_life || \
					get_hour() > time_death)
			{
				send_sms(temp[i], "Dead", 1);
				return (NULL);
			}
		}
	}
}

void	create_pthread(t_philo **phil, t_table *table)
{
	int	count;

	count = 0;
	if (pthread_mutex_lock(&table->death) || \
			pthread_create(&table->thread_death, NULL, death, phil))
											//мониторит всех философов, умерли ли
		ft_close(3);
	while (count < table->philo)
	{
		pthread_create(&phil[count]->thread_phil, NULL, start_thread, phil[count]);
		count++;
		usleep(10);
	}
	if (pthread_join(table->thread_death, NULL) || \
			pthread_mutex_lock(&table->death))
		ft_close(3);
}

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

int	init_p(t_table *table)
{
	int			count;
	t_philo		**phil;

	phil = malloc(sizeof(t_philo) * table->philo);
	if (!phil)
	{
		ft_close(1);
		return (1);
	}
	count = table->philo;
	while (count > 0)
	{
		phil[count] = malloc(sizeof(t_philo));
		if (!phil[count])
		{
			ft_close(1);
			return (1);
		}
		phil[count]->index_phil = count + 1;
		phil[count]->tabl = table;
		if (pthread_mutex_init(&phil[count]->right, NULL))
			ft_close(3);
		phil[count]->eat_start = table->time_start;
		phil[count]->i_eat = 0;
		count--;
	}
	count = 0;
	while (count < table->philo)
	{
		phil[count]->left = &phil[count - 1]->right;
		count++;
	}
	phil[0]->left = &phil[count - 1]->right;
	create_pthread(phil, table);
	break_down(phil);
	free(phil);
	return (0);
}

int pars_argv(int argc, char **argv, t_table *table)
{
	table->philo = ft_atoi(argv[1]);
	table->time_life = ft_atoi(argv[2]);
	table->time_eat = ft_atoi(argv[3]);
	table->time_sleep = ft_atoi(argv[4]);
	if (table->philo < 1 || table->time_life < 1 || table->time_eat < 1 || \
		table->time_sleep < 1)
	{
		ft_close(2);
		return (-1);
	}
	if (argc == 6)
		table->eat = ft_atoi(argv[5]);
	else
		table->eat = -1;
	if (pthread_mutex_init(&table->sms, NULL) || \
			pthread_mutex_init(&table->death, NULL))
	{
		ft_close(3);
		return (-1);
	}
	if (init_p(table) == 0)
		return (0);
	return (0);
}
int	ft_isdigit(int c)
{
	if (c > 47 && c < 58)
		return (1);
	else
		return (0);
}


int	check_str(char *str)
{
	int	i;
	int	flag;

	i = 0;
	flag = 0;
	while (ft_isdigit(str[i]) > 0)
	{
		i++;
		flag++;
	}
	if (!str[i] && flag)
		flag = 1;
	else
		flag = 0;
	return (flag);
}

int	main(int argc, char **argv)
{
	t_table	table;
	int		i;
	int		flag;

	i = 1;
	if (argc == 5 || argc == 6)
	{
		while (argv[i])
		{
			flag = check_str(argv[i]);
			if (!flag)
				ft_close(2);
			i++;
		}
		if (pars_argv(argc, argv, &table) != 0)
			ft_close(0);
	}
	else
		ft_close(0);
	return (0);
}
