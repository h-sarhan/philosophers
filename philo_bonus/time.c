/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/01 11:39:22 by hsarhan           #+#    #+#             */
/*   Updated: 2022/09/05 11:23:51 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

// Get the current time in ms
t_time_ms	get_time(const t_timeval *start_time)
{
	t_timeval	curr_time;
	t_time_ms	curr_time_in_ms;
	t_time_ms	start_time_in_ms;

	start_time_in_ms = start_time->tv_sec * 1000 + start_time->tv_usec / 1000;
	if (gettimeofday(&curr_time, NULL) == FAIL)
	{
		return (TIME_FAILURE);
	}
	curr_time_in_ms = curr_time.tv_sec * 1000 + curr_time.tv_usec / 1000;
	return (curr_time_in_ms - start_time_in_ms);
}

unsigned long	get_utime(const t_timeval *start_time)
{
	t_timeval		curr_time;
	unsigned long	curr_time_in_us;
	unsigned long	start_time_in_us;

	start_time_in_us = start_time->tv_sec * 1000000 + \
						start_time->tv_usec;
	if (gettimeofday(&curr_time, NULL) == FAIL)
		return (TIME_FAILURE);
	curr_time_in_us = curr_time.tv_sec * 1000000 + curr_time.tv_usec;
	return (curr_time_in_us - start_time_in_us);
}

void	sleepsleep(t_phil *phil, t_time_ms sleep_time)
{
	t_time_ms	time_slept;
	t_timeval	start_time;

	(void)phil;
	time_slept = 0;
	gettimeofday(&start_time, NULL);
	while (time_slept < sleep_time)
	{
		usleep(100);
		time_slept = get_utime(&start_time);
	}
}

int	check_time_since_eat(t_phil *phil)
{
	if (sem_wait(phil->sim->sems->time) != 0)
		perror(NULL);
	if (get_utime(phil->phil_eat_time) >= phil->sim->time_to_die * 1000)
	{
		sem_post(phil->sim->sems->time);
		log_action(phil->sim, phil->num, "has died");
		sem_post(phil->sim->sems->status);
		return (END);
	}
	sem_post(phil->sim->sems->time);
	return (CONTINUE);
}
