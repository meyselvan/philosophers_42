/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destroydetach.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: relvan <relvan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 16:28:39 by relvan            #+#    #+#             */
/*   Updated: 2024/06/29 16:28:41 by relvan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	destroydetach(t_list *data)
{
	int	i;

	i = 0;
	pthread_mutex_destroy(&data->is_eat);
	pthread_mutex_destroy(&data->write_mutex);
	while (i < data->number_philo)
	{
		pthread_mutex_destroy(&data->forks[i]);
		i++;
	}
	i = 0;
	while (i < data->number_philo)
	{
		pthread_detach(data->philos[i].thread);
		i++;
	}
	free(data->forks);
	free(data->philos);
	free(data);
}
