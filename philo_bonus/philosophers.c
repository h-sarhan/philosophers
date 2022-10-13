/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/01 18:40:21 by hsarhan           #+#    #+#             */
/*   Updated: 2022/09/04 09:14:36 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static t_phil	*create_philosopher(t_sim *sim, const unsigned int num)
{
	t_phil	*phil;

	phil = ft_calloc(1, sizeof(t_phil));
	if (phil == NULL)
		return (NULL);
	phil->num = num;
	phil->sim = sim;
	phil->state = THINKING;
	phil->phil_eat_time = ft_calloc(1, sizeof(t_timeval));
	if (phil->phil_eat_time == NULL)
		return (NULL);
	phil->num_eats = 0;
	return (phil);
}

static void	*phil_creation_error(t_phil **phils, unsigned int num)
{
	unsigned int	temp;

	temp = 0;
	while (temp < num)
	{
		ft_free(&phils[temp]);
		temp++;
	}
	ft_free(&phils);
	return (NULL);
}

t_phil	**create_philosophers(t_sim *sim)
{
	t_phil			**phils;
	unsigned int	num;

	num = 0;
	phils = ft_calloc(sim->num_phils, sizeof(t_sim *));
	if (phils == NULL)
		return (NULL);
	sim->philo_pids = ft_calloc(sim->num_phils, sizeof(int));
	if (sim->philo_pids == NULL)
		return (NULL);
	while (num < sim->num_phils)
	{
		phils[num] = create_philosopher(sim, num + 1);
		if (phils[num] == NULL)
			return (phil_creation_error(phils, num));
		num++;
	}
	return (phils);
}

void	free_philosophers(t_phil **phils)
{
	unsigned int	num_phils;
	unsigned int	i;

	num_phils = phils[0]->sim->num_phils;
	i = 0;
	while (i < num_phils)
	{
		ft_free(&phils[i]->phil_eat_time);
		ft_free(&phils[i]);
		i++;
	}
	ft_free(&phils);
}
