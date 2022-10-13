/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/13 15:35:52 by hsarhan           #+#    #+#             */
/*   Updated: 2022/09/03 20:08:19 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static bool	check_right_fork(t_phil *phil, const unsigned int left,
							const unsigned int right, const bool old)
{
	bool	held;

	if (left == right)
		return (true);
	pthread_mutex_lock(&phil->sim->fork_mutexes[right]);
	if (phil->sim->fork_takers[right] == 0
		|| phil->sim->fork_takers[right] != phil->num)
	{
		held = phil->sim->forks[right];
		return (held);
	}
	return (old);
}

static bool	check_left_fork(t_phil *phil, const unsigned int left,
								const bool old)
{
	bool	held;

	pthread_mutex_lock(&phil->sim->fork_mutexes[left]);
	if (phil->sim->fork_takers[left] == 0
		|| phil->sim->fork_takers[left] != phil->num)
	{
		held = phil->sim->forks[left];
		return (held);
	}
	return (old);
}

static int	look_for_fork(t_phil *phil, const unsigned int left,
					const unsigned int right)
{
	bool	left_right[2];

	left_right[0] = true;
	left_right[1] = true;
	while ((left_right[0] == true || left_right[1] == true))
	{
		if (left < right)
		{
			left_right[0] = check_left_fork(phil, left, left_right[0]);
			left_right[1] = check_right_fork(phil, left, right, left_right[1]);
		}
		else
		{
			left_right[1] = check_right_fork(phil, left, right, left_right[1]);
			left_right[0] = check_left_fork(phil, left, left_right[0]);
		}
		if (left_right[0] == false && left_right[1] == false)
			break ;
		pthread_mutex_unlock(&phil->sim->fork_mutexes[left]);
		pthread_mutex_unlock(&phil->sim->fork_mutexes[right]);
		if (check_time_since_eat(phil) == END)
			return (END);
	}
	return (CONTINUE);
}

static int	eat_spaghetti(t_phil *phil, const unsigned int left,
			const unsigned int right)
{
	if (check_time_since_eat(phil) == END)
	{
		pthread_mutex_unlock(&phil->sim->fork_mutexes[left]);
		pthread_mutex_unlock(&phil->sim->fork_mutexes[right]);
		return (END);
	}
	pick_up_forks(phil, left, right);
	if (log_action(phil->sim, phil->num, log_eat) == false)
		return (END);
	if (phil->sim->min_eats > 0)
	{
		pthread_mutex_lock(&phil->num_eats_mutex);
		phil->num_eats++;
		pthread_mutex_unlock(&phil->num_eats_mutex);
	}
	gettimeofday(phil->phil_eat_time, NULL);
	if (sleepsleep(phil, phil->sim->time_to_eat * 1000) == FAIL)
		return (END);
	put_back_forks(phil, left, right);
	phil->state = EATING;
	return (CONTINUE);
}

int	eating_phase(t_phil *phil)
{
	unsigned int	left;
	unsigned int	right;

	if (phil->sim->num_phils == 1)
	{
		left = 0;
		right = 0;
	}
	else
	{
		left = phil->num - 1;
		right = phil->num;
	}
	if (phil->num == phil->sim->num_phils)
		right = 0;
	if (look_for_fork(phil, left, right) == END)
		return (END);
	if (eat_spaghetti(phil, left, right) == END)
		return (END);
	return (CONTINUE);
}
