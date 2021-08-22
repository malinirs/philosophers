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
	int	flag_sms;
}			t_table;

typedef struct s_philo
{
	pthread_mutex_t	*left; //на левую вилку
	pthread_mutex_t	right; //на правую вилку
	pthread_t 		thread_phil; //поток на философов
	int				index_phil; //индекс философа
	int				i_eat; //счетчик кол-ва приемов пищи на 1 фила
	unsigned long	eat_start; // время начала приема пищи
//	int	flag_sms;
	t_table	*tabl;
}			t_philo;
#endif


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

unsigned long	get_time(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
		ft_close(4);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

unsigned long	diff_time(t_philo *phil)
{
	unsigned long	time;

	time = get_time() - phil->tabl->time_start;
	return (time);
}

unsigned long	diff_eating_time(t_philo *phil)
{
	unsigned long	time;

	time = get_time() - phil->eat_start;
	return (time);
}

void	usleep_mod(unsigned long arg_time)
{
	unsigned long	end_time;

	end_time = get_time() + arg_time;
	while (get_time() < end_time)
		usleep(100);
}

void	*send_sms(t_philo *philo, char *str, int i)
{
	if (i == 0)
	{
		pthread_mutex_lock(&philo->tabl->sms);
		printf("%lu %d %s\n", diff_time(philo), philo->index_phil, str);
		pthread_mutex_unlock(&philo->tabl->sms);
		return (NULL);
	}
	else if (i == 1)
	{
		pthread_mutex_lock(&philo->tabl->sms);
		printf("%lu %d %s\n", diff_time(philo), philo->index_phil, str);
		usleep (1001);
		pthread_mutex_unlock(&philo->tabl->sms);
		return (NULL);
	}
	else if (i == 2)
	{
		pthread_mutex_lock(&philo->tabl->sms);
		printf("%s\n", str);
		pthread_mutex_unlock(&philo->tabl->sms);
		return (NULL);
	}
	return (0);

//	if (count == 0)
//	{
//		if (pthread_mutex_lock(&philo->tabl->sms))
//			ft_close(3);
//		printf("%lu %d %s\n", diff_time(philo), philo->index_phil, ptr);
//		if (pthread_mutex_unlock(&philo->tabl->sms))
//			ft_close(3);
//		return (NULL);
//	}
//	else if (count == 1)
//	{
//		if (pthread_mutex_lock(&philo->tabl->sms))
//			ft_close(3);
//		printf("%lu %d %s\n", diff_time(philo), philo->index_phil, ptr);
//		usleep (1000);
//		if (pthread_mutex_unlock(&philo->tabl->death))
//			ft_close(3);
//		return (NULL);
//	}
//	else if (count == 2)
//	{
//		if (pthread_mutex_lock(&philo->tabl->sms))
//			ft_close(3);
//		printf("%s\n", ptr);
//		if (pthread_mutex_unlock(&philo->tabl->death))
//			ft_close(3);
//		return (NULL);
//	}
//	return (0);
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
		temp->eat_start = get_time();
		send_sms(temp, "eatting", 0);
		temp->i_eat++;
		if (temp->i_eat == temp->tabl->eat)
			temp->tabl->count_eat++;
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
	t_philo	**ph;
	unsigned long	time_for_die;
	int				i;

	ph = phil;
	while (1)
	{
		i = -1;
		while (++i < ph[0]->tabl->philo)
		{
			time_for_die = ph[i]->tabl->time_life + ph[i]->eat_start;
			if (ph[i]->tabl->count_eat == ph[i]->tabl->philo)
			{
				send_sms(ph[i], "The meal is over!", 2);
				pthread_mutex_lock(&ph[i]->tabl->sms);
				return (NULL);
			}
			if ((int)diff_eating_time(ph[i]) > ph[i]->tabl->time_life \
			|| get_time() > time_for_die)
			{
				send_sms(ph[i], "is dead", 1);
				pthread_mutex_lock(&ph[i]->tabl->sms);
				return (NULL);
			}
		}
	}
//	return (NULL);
}

void	create_pthread(t_philo **phil, t_table *table)
{
	int	count;

	count = 0;
	pthread_mutex_lock(&table->death);
	pthread_create(&table->thread_death, NULL, death, phil);
											//мониторит всех философов, умерли ли
//		ft_close(3);
	while (count < table->philo)
	{
		pthread_create(&phil[count]->thread_phil, NULL, start_thread, phil[count]);
		count++;
		usleep(10);
	}
//	if (pthread_join(table->thread_death, NULL) || \
//			pthread_mutex_lock(&table->death))
//		ft_close(3);


	pthread_join(table->thread_death, NULL);
//	printf("ssssssssssssssssssssssssssss\n");
//	pthread_mutex_lock(&table->death);

//	printf("qqqqqqqqqqqqqqq\n");

//	int	i;
//
//	i = -1;
//	while (++i < table->philo)
//	{
//		pthread_create(&phil[i]->thread_phil, NULL, start_thread, phil[i]);
//		usleep (10);
//	}
//	pthread_join(table->thread_death, NULL);
//	pthread_mutex_lock(&table->death);
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
		ft_close(1);
	count = 0;
	while (count < table->philo)
	{
		phil[count] = malloc(sizeof(t_philo));
		if (!phil[count])
			ft_close(1);
		phil[count]->index_phil = count + 1;
		phil[count]->tabl = table;
		if (pthread_mutex_init(&phil[count]->right, NULL))
			ft_close(3);
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
	break_down(phil);
	usleep(5000);
	free(phil);
	return (0);
}

int	ft_atoi(const char *str)
{
	int						znak;
	unsigned long long		result;

	result = 0;
	znak = 1;
	while (*str == ' ' || (*str > 8 && *str < 14))
		str++;
	if (*str == '-')
	{
		znak = -1;
		str++;
	}
	else if (*str == '+')
		str++;
	while (*str >= '0' && *str <= '9')
	{
		result = result * 10 + (*str - '0');
		if (result > 9223372036854775807 && (znak == 1))
			return (-1);
		if (result > 9223372036854775807 && (znak == -1))
			return (0);
		str++;
	}
	return ((int)result * znak);
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
	table->time_start = get_time();
	table->count_eat = 0;
	table->flag_sms = 0;
	if (pthread_mutex_init(&table->sms, NULL) || \
			pthread_mutex_init(&table->death, NULL))
	{
		ft_close(3);
		return (-1);
	}
	init_p(table);
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
