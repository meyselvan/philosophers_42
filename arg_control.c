/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arg_control.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: relvan <relvan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 16:28:46 by relvan            #+#    #+#             */
/*   Updated: 2024/06/29 16:28:48 by relvan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

long	fatoi(char *str)
{
	long	num;
	long	sign;
	int		i;

	i = 0;
	num = 0;
	sign = 1;
	while (str[i] && (str[i] == 32 || (str[i] >= 9 && str[i] <= 13)))
		i++;
	if (str[i] == '-')
		sign = -1;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i] && str[i] >= '0' && str[i] <= '9')
	{
		num *= 10;
		num += str[i] - 48;
		i++;
	}
	num *= sign;
	if (num > 2147483647 || num < -2147483648)
		return (-2);
	return (sign * num);
}

int	philo_creat(t_list *data, bool ok)
{
	int	i;

	if (ok == true)
	{
		i = -1;
		while (++i < data->number_philo)
		{
			data->philos[i].id = i;
			data->philos[i].data = data;
			data->philos[i].is_dead = false;
			data->philos[i].last_eat = get_mss(data);
			if (pthread_create(&data->philos[i].thread, NULL, life,
					&data->philos[i]) != 0)
				return (-1);
			usleep(100);
		}
		return (-2);
	}
	return (0);
}

int	data_creat(char **str, t_list *data)
{
	int	i;

	data->time_to_eat = fatoi(str[3]);
	data->time_to_sleep = fatoi(str[4]);
	data->start_time = get_mss(data);
	if (fatoi(str[1]) == -2 || fatoi(str[2]) == -2 || fatoi(str[3]) == -2
		|| fatoi(str[4]) == -2 || (str[5] && fatoi(str[5]) == -2) || (str[5]
			&& fatoi(str[5]) == 0))
		return (1);
	if (str[5])
		data->must_eat = fatoi(str[5]);
	i = -1;
	if (pthread_mutex_init(&data->is_eat, NULL) != 0
		|| pthread_mutex_init(&data->write_mutex, NULL) != 0)
		return (1);
	data->philos = malloc(sizeof(t_philo) * data->number_philo);
	data->forks = malloc(sizeof(pthread_mutex_t) * data->number_philo);
	if (!(data->philos) || !(data->forks))
		return (1);
	while (++i < data->number_philo)
		if (pthread_mutex_init(&data->forks[i], NULL))
			return (1);
	if (philo_creat(data, true) == -1)
		return (1);
	return (0);
}

int	num_control(char **str, t_list *data)
{
	int	i;
	int	j;

	j = 1;
	while (str[j])
	{
		i = 0;
		while (str[j][i] && (str[j][i] == 32 || (str[j][i] >= 9
					&& str[j][i] <= 13)))
			i++;
		while (str[j][i])
		{
			if (str[j][i] >= '0' && str[j][i] <= '9')
				i++;
			else
				return (1);
		}
		j++;
	}
	data->eat_count = 0;
	data->must_eat = -1;
	data->start_time = 0;
	data->number_philo = (int)fatoi(str[1]);
	data->time_to_die = fatoi(str[2]);
	return (0);
}

void	*life(void *philo)
{
	t_philo	*p;

	p = (t_philo *)philo;
	while (1)
	{
		pthread_mutex_lock(&p->data->forks[p->id]);
		writes(p, 0);
		pthread_mutex_lock(&p->data->forks[(p->id + 1)
			% p->data->number_philo]);
		writes(p, 0);
		writes(p, 3);
		pthread_mutex_lock(&p->data->is_eat);
		p->data->eat_count++;
		p->last_eat = get_mss(p->data);
		pthread_mutex_unlock(&p->data->is_eat);
		ms_sleep(p, p->data->time_to_eat);
		writes(p, 1);
		pthread_mutex_unlock(&p->data->forks[p->id]);
		pthread_mutex_unlock(&p->data->forks[(p->id + 1)
			% p->data->number_philo]);
		ms_sleep(p, p->data->time_to_sleep);
		writes(p, 2);
	}
	return (NULL);
}
