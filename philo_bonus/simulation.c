/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/01 11:44:51 by hsarhan           #+#    #+#             */
/*   Updated: 2022/09/05 11:23:48 by hsarhan          ###   ########.fr       */
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
	if (sim->start_time == NULL)
	{
		free_sim(sim);
		return (NULL);
	}
	return (sim);
}

void	*run_sim(void *phil_ptr)
{
	t_phil		*phil;
	pthread_t	thread;

	phil = phil_ptr;
	gettimeofday(phil->phil_eat_time, NULL);
	pthread_create(&thread, NULL, check_time, phil);
	while (1)
	{
		if (phil->state == THINKING)
			eating_phase(phil);
		if (phil->state == EATING)
			sleep_phase(phil);
		if (phil->state == SLEEPING)
			think_phase(phil);
		if (phil->state == DEAD)
			return (NULL);
	}
	return (NULL);
}

// Frees a simulation struct
void	free_sim(t_sim *sim)
{
	if (sim->sems != NULL)
	{
		sem_close(sim->sems->logging);
		sem_close(sim->sems->num_eats);
		sem_close(sim->sems->num_forks);
		sem_close(sim->sems->time);
		sem_close(sim->sems->turn);
		sem_close(sim->sems->status);
	}
	ft_free(&sim->start_time);
	ft_free(&sim->sems);
	ft_free(&sim->philo_pids);
	ft_free(&sim);
}

t_sim	*init_sim(int argc, char **argv, t_phil ***philosophers)
{
	bool	success;
	t_sim	*sim;

	sim = create_simulation();
	if (sim == NULL)
		return (NULL);
	parse_args(sim, argc, argv, &success);
	if (success == false)
	{
		write(STDERR_FILENO, "Invalid arguments\n", 18);
		free_sim(sim);
		return (NULL);
	}
	*philosophers = create_philosophers(sim);
	if (*philosophers == NULL)
	{
		free_sim(sim);
		return (NULL);
	}
	return (sim);
}

void	*check_time(void *phil_ptr)
{
	t_phil	*phil;

	phil = phil_ptr;
	while (1)
	{
		if (check_time_since_eat(phil) == END)
			return (NULL);
		usleep(500);
	}
	return (NULL);
}
