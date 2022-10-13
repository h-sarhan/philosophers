/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/01 11:44:51 by hsarhan           #+#    #+#             */
/*   Updated: 2022/09/03 20:08:39 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

// Creates a simulation struct
t_sim	*create_simulation(void)
{
	t_sim	*sim;

	sim = ft_calloc(1, sizeof(t_sim));
	if (sim == NULL)
		return (NULL);
	sim->start_time = ft_calloc(1, sizeof(t_timeval));
	if (sim->start_time == NULL)
		return (NULL);
	gettimeofday(sim->start_time, NULL);
	pthread_mutex_init(&sim->logging_mutex, NULL);
	pthread_mutex_init(&sim->status_mutex, NULL);
	if (sim->start_time == NULL)
	{
		free_sim(sim);
		return (NULL);
	}
	sim->status = true;
	return (sim);
}

bool	read_sim_status(t_sim *sim)
{
	bool	status;

	pthread_mutex_lock(&sim->status_mutex);
	status = sim->status;
	pthread_mutex_unlock(&sim->status_mutex);
	return (status);
}

void	*run_sim(void *phil_ptr)
{
	t_phil	*phil;

	phil = (t_phil *) phil_ptr;
	gettimeofday(phil->phil_eat_time, NULL);
	while (1)
	{
		if (phil->state == THINKING)
			if (eating_phase(phil) == END)
				return (NULL);
		if (phil->state == EATING)
		{
			if (sleep_phase(phil) == END)
				return (NULL);
			phil->state = SLEEPING;
			if (check_time_since_eat(phil) == END)
				return (NULL);
		}
		if (phil->state == SLEEPING)
			if (think_phase(phil) == END)
				return (NULL);
		if (phil->state == DEAD)
			return (NULL);
	}
	return (NULL);
}

unsigned int	read_num_eats(t_phil *phil)
{
	unsigned int	num_eats;

	pthread_mutex_lock(&phil->num_eats_mutex);
	num_eats = phil->num_eats;
	pthread_mutex_unlock(&phil->num_eats_mutex);
	return (num_eats);
}

// Frees a simulation struct
void	free_sim(t_sim *sim)
{
	size_t	i;

	i = 0;
	while (i < sim->num_phils)
	{
		pthread_mutex_destroy(&sim->fork_mutexes[i]);
		i++;
	}
	pthread_mutex_destroy(&sim->logging_mutex);
	pthread_mutex_destroy(&sim->status_mutex);
	ft_free(&sim->start_time);
	ft_free(&sim->forks);
	ft_free(&sim->fork_mutexes);
	ft_free(&sim->fork_takers);
	ft_free(&sim);
}
