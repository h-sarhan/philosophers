/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/01 10:54:25 by hsarhan           #+#    #+#             */
/*   Updated: 2022/09/03 20:09:35 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

// Tests: 
// ? valgrind --tool=helgrind --history-level=none  ./philo 5 800 200 200 7
// * ./philo 1 800 200 200: Should not eat and die
// * ./philo 5 800 200 200 7: No one should die
// * ./philo 4 410 200 200: No one should die
// * ./philo 4 310 200 100: A philosopher should die
// * ./philo 3 1000 500 600  A philosopher should die
// * ./philo 5 200 100 60  A philosopher should die
// * ./philo 4 500 400 300  A philosopher should die
// * ./philo 3 700 200 200 Philosophers shouldnt die here
// * ./philo 2 200 200 100  A philosopher should die
static void	cleanup(t_sim *sim, pthread_t *threads, t_phil **philosophers)
{
	size_t	temp;

	temp = 0;
	while (temp < sim->num_phils)
	{
		pthread_join(threads[temp], NULL);
		temp++;
	}
	free_philosophers(philosophers);
	free_sim(sim);
	ft_free(&threads);
}

static t_sim	*init_sim(int argc, char **argv, t_phil ***philosophers)
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

static void	check_sim(t_sim *sim, t_phil **philosophers)
{
	bool	all_ate;
	size_t	i;

	while (read_sim_status(sim) == true)
	{
		if (sim->min_eats > 0)
		{
			i = 0;
			all_ate = true;
			while (i < sim->num_phils)
			{
				if (read_num_eats(philosophers[i]) < sim->min_eats)
					all_ate = false;
				i++;
			}
			if (all_ate == true)
			{
				pthread_mutex_lock(&sim->status_mutex);
				sim->status = false;
				pthread_mutex_unlock(&sim->status_mutex);
				break ;
			}
		}
	}
}

int	main(int argc, char **argv)
{
	t_sim		*sim;
	t_phil		**philosophers;
	pthread_t	*threads;
	size_t		i;

	sim = init_sim(argc, argv, &philosophers);
	if (sim == NULL)
		return (EXIT_FAILURE);
	threads = ft_calloc(sim->num_phils, sizeof(pthread_t));
	if (threads == NULL)
	{
		free_philosophers(philosophers);
		free_sim(sim);
		return (EXIT_FAILURE);
	}
	i = 0;
	while (i < sim->num_phils)
	{
		pthread_create(&threads[i], NULL, run_sim, philosophers[i]);
		usleep(350);
		i++;
	}
	check_sim(sim, philosophers);
	cleanup(sim, threads, philosophers);
}
