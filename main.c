/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: relvan <relvan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 16:28:31 by relvan            #+#    #+#             */
/*   Updated: 2024/06/29 16:28:33 by relvan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	writes(t_philo *p, int num)
{
	if (num == 0)
	{
		pthread_mutex_lock(&p->data->write_mutex);
		printf("%lu %d has taken a fork\n", get_mss(p->data), p->id + 1);
		pthread_mutex_unlock(&p->data->write_mutex);
	}
	else if (num == 1)
	{
		pthread_mutex_lock(&p->data->write_mutex);
		printf("%lu %d is sleeping\n", get_mss(p->data), p->id + 1);
		pthread_mutex_unlock(&p->data->write_mutex);
	}
	else if (num == 2)
	{
		pthread_mutex_lock(&p->data->write_mutex);
		printf("%lu %d is thinking\n", get_mss(p->data), p->id + 1);
		pthread_mutex_unlock(&p->data->write_mutex);
	}
	else if (num == 3)
	{
		pthread_mutex_lock(&p->data->write_mutex);
		printf("%lu %d is eating\n", get_mss(p->data), p->id + 1);
		pthread_mutex_unlock(&p->data->write_mutex);
	}
}

void	main_thread(t_list *data)
{
	int	i;

	i = 1;
	while (1)
	{
		pthread_mutex_lock(&data->is_eat);
		if (data->must_eat != -1
			&& data->eat_count >= data->must_eat * data->number_philo)
		{
			pthread_mutex_lock(&data->write_mutex);
			return ;
		}
		data->cont = (i % data->number_philo);
		if (get_mss(data) - data->philos[data->cont].last_eat
			>= data->time_to_die)
		{
			pthread_mutex_lock(&data->write_mutex);
			printf("%lu %d died\n", get_mss(data), data->cont + 1);
			return ;
		}
		pthread_mutex_unlock(&data->is_eat);
		i++;
		usleep(100);
	}
}

long	get_mss(t_list *data)
{
	long	t;

	t = data->start_time;
	gettimeofday(&data->tv, NULL);
	return ((data->tv.tv_usec / 1000 + data->tv.tv_sec * 1000) - t);
}

void	ms_sleep(t_philo *p, long time)
{
	long	dest_time;

	dest_time = get_mss(p->data) + time;
	while (get_mss(p->data) < dest_time)
		usleep(100);
}

int	main(int ac, char **av)
{
	t_list	*data;

	if (ac != 5 && ac != 6)
		return (1);
	data = (t_list *)malloc(sizeof(t_list));
	if (!data)
		return (1);
	data->eat_count = 0;
	data->must_eat = -1;
	data->start_time = 0;
	if (av[1][0] == '0' || num_control(av, data) == 1)
	{
		free(data);
		return (1);
	}
	if (data_creat(av, data) == 1)
	{
		free(data);
		return (1);
	}
	main_thread(data);
	destroydetach(data);
	return (0);
}
