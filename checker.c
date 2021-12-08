/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: awoods <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/08/23 22:11:58 by awoods            #+#    #+#             */
/*   Updated: 2021/08/23 22:23:12 by                  ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	ft_isdigit(int c)
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

static int	ft_atoi(const char *str)
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

int	pars_argv(int argc, char **argv, t_table *table)
{
	table->philo = ft_atoi(argv[1]);
	table->time_life = ft_atoi(argv[2]);
	table->time_eat = ft_atoi(argv[3]);
	table->time_sleep = ft_atoi(argv[4]);
	if (table->philo < 1 || table->time_life < 1 || table->time_eat < 1 || \
	table->time_sleep < 1)
	{
		ft_trouble(2);
		return (-1);
	}
	if (argc == 6)
		table->eat = ft_atoi(argv[5]);
	else
		table->eat = -1;
	table->time_start = get_time();
	table->count_eat = 0;
	if (pthread_mutex_init(&table->sms, NULL) || \
	pthread_mutex_init(&table->death, NULL))
	{
		ft_trouble(3);
		return (-1);
	}
	init(table, 0);
	return (0);
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
			{
				ft_trouble(2);
				return (0);
			}
			i++;
		}
		if (pars_argv(argc, argv, &table) != 0)
			ft_trouble(0);
	}
	else
		ft_trouble(0);
	return (0);
}
