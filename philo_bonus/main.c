/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/01 10:54:25 by hsarhan           #+#    #+#             */
/*   Updated: 2022/09/04 18:37:57 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

// Tests: 
// ? valgrind --tool=helgrind --history-level=none ./philo_bonus 5 800 200 200 7
// * ./philo_bonus 1 800 200 200: Should not eat and die
// * ./philo_bonus 5 800 200 200 7: No one should die
// * ./philo_bonus 4 410 200 200: No one should die
// * ./philo_bonus 4 310 200 100: A philosopher should die
// * ./philo_bonus 3 1000 500 600
// * ./philo_bonus 5 200 100 60
// * ./philo_bonus 4 500 400 300
// * ./philo_bonus 3 700 200 200 Philosophers shouldnt die here
// * ./philo_bonus 199 800 200 100
static void	*check_for_min_eats(void *sim_ptr)
{
	t_sim	*sim;
	size_t	i;

	sim = sim_ptr;
	i = 0;
	while (i < sim->num_phils)
	{
		if (sem_wait(sim->sems->num_eats) != 0)
			perror(NULL);
		i++;
	}
	sem_post(sim->sems->status);
	return (NULL);
}

static void	cleanup(t_sim *sim, t_phil **philosophers)
{
	size_t		i;
	pthread_t	min_eats_thread;

	if (sim->min_eats > 0)
		pthread_create(&min_eats_thread, NULL, check_for_min_eats, sim);
	if (sem_wait(sim->sems->status) != 0)
		perror(NULL);
	i = 0;
	while (sim->min_eats > 0 && i < sim->num_phils)
	{
		sem_post(sim->sems->num_eats);
		i++;
	}
	if (sim->min_eats > 0)
		pthread_join(min_eats_thread, NULL);
	kill_philosophers(sim);
	free_philosophers(philosophers);
	free_sim(sim);
	sem_unlink("/forks");
	sem_unlink("/logging");
	sem_unlink("/turn");
	sem_unlink("/time");
	sem_unlink("/num_eats");
	sem_unlink("/status");
	exit(EXIT_SUCCESS);
}

void	kill_philosophers(t_sim *sim)
{
	size_t	i;

	i = 0;
	while (i < sim->num_phils)
	{
		if (sim->philo_pids[i] != 0)
			kill(sim->philo_pids[i], SIGKILL);
		i++;
	}
}

void	init_sems(t_sim *sim)
{
	t_sems			*sems;

	sems = ft_calloc(1, sizeof(t_sems));
	if (sems == NULL)
		exit(EXIT_FAILURE);
	sem_unlink("/forks");
	sems->num_forks = sem_open("/forks", O_CREAT, 0644, sim->num_phils);
	sem_unlink("/logging");
	sems->logging = sem_open("/logging", O_CREAT, 0644, 1);
	sem_unlink("/turn");
	sems->turn = sem_open("/turn", O_CREAT, 0644, 1);
	sem_unlink("/time");
	sems->time = sem_open("/time", O_CREAT, 0644, 1);
	sem_unlink("/num_eats");
	sems->num_eats = sem_open("/num_eats", O_CREAT, 0644, 0);
	sem_unlink("/status");
	sems->status = sem_open("/status", O_CREAT, 0644, 0);
	if (sems->num_forks == SEM_FAILED || sems->logging == SEM_FAILED
		|| sems->turn == SEM_FAILED || sems->time == SEM_FAILED
		|| sems->num_eats == SEM_FAILED || sems->status == SEM_FAILED)
		exit(EXIT_FAILURE);
	sim->sems = sems;
}

int	main(int argc, char **argv)
{
	t_sim		*sim;
	t_phil		**philosophers;
	size_t		i;

	sim = init_sim(argc, argv, &philosophers);
	if (sim == NULL)
		return (EXIT_FAILURE);
	sim->philosophers = philosophers;
	i = 0;
	init_sems(sim);
	while (i < sim->num_phils)
	{
		sim->philo_pids[i] = fork();
		if (sim->philo_pids[i] == 0)
		{
			run_sim(philosophers[i]);
			free_philosophers(philosophers);
			free_sim(sim);
			exit(EXIT_SUCCESS);
		}
		i++;
	}
	cleanup(sim, philosophers);
}
