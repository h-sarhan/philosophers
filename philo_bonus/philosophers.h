/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/01 10:29:49 by hsarhan           #+#    #+#             */
/*   Updated: 2022/09/05 11:20:36 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

// Includes
# include <sys/time.h>
# include <sys/wait.h>
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <pthread.h>
# include <stdbool.h>
# include <limits.h>
# include <string.h>
# include <stdint.h>
# include <semaphore.h>
# include <fcntl.h>
# include <signal.h>

// Constants
# define FAIL -1
# define SUCCESS 0
# define TIME_FAILURE 0
# define CONTINUE 1
# define END 0

// Typedefs
typedef struct s_sim		t_sim;
typedef struct s_phil		t_phil;
typedef unsigned long		t_time_ms;
typedef enum e_phil_state	t_phil_state;
typedef struct timeval		t_timeval;
typedef void				(*t_log_func)(const t_time_ms *time,
								const size_t phil_num);
typedef struct s_sems		t_sems;

struct s_sems
{
	sem_t	*num_forks;
	sem_t	*logging;
	sem_t	*num_eats;
	sem_t	*turn;
	sem_t	*time;
	sem_t	*status;
};

// Simulation Struct
struct s_sim
{
	unsigned int	num_phils;
	t_time_ms		time_to_die;
	t_time_ms		time_to_eat;
	t_time_ms		time_to_sleep;
	unsigned int	min_eats;
	t_timeval		*start_time;
	bool			status;
	int				*philo_pids;
	t_sems			*sems;
	t_phil			**philosophers;
};

// Philosopher States
enum e_phil_state
{
	EATING,
	SLEEPING,
	THINKING,
	DEAD
};

// Philosopher Struct
struct s_phil
{
	t_phil_state	state;
	unsigned int	num;
	unsigned int	num_eats;
	t_sim			*sim;
	t_timeval		*phil_eat_time;
};

// Philosopher Functions
t_phil			**create_philosophers(t_sim *sim);
void			free_philosophers(t_phil **phils);

// Simulation Functions
t_sim			*create_simulation(void);
void			free_sim(t_sim *sim);
void			*run_sim(void *phil);
void			think_phase(t_phil *phil);
int				check_time_since_eat(t_phil *phil);
void			sleep_phase(t_phil *phil);
void			eating_phase(t_phil *phil);
void			pick_up_fork(t_phil *phil);
void			put_back_fork(t_phil *phil);
void			kill_philosophers(t_sim *sim);
t_sim			*init_sim(int argc, char **argv, t_phil ***philosophers);

// Utils
size_t			ft_strlen(const char *str);
void			ft_free(void *memory);
void			*ft_calloc(size_t count, size_t size);
void			sleepsleep(t_phil *phil, t_time_ms sleep_time);
int				ft_strncmp(const char *s1, const char *s2, size_t n);

// Time
t_time_ms		get_time(const t_timeval *sim);
t_time_ms		get_utime(const t_timeval *start_time);
void			*check_time(void *phil_ptr);
// Logging Functions
void			log_action(t_sim *sim, const size_t phil_num,
					const char *action);

// Parsing
long			atoui(const char *str);
void			parse_args(t_sim *sim, const int argc, char **argv,
					bool *success);

#endif