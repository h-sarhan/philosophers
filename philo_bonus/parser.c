/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsarhan <hsarhan@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/01 15:17:17 by hsarhan           #+#    #+#             */
/*   Updated: 2022/08/13 11:27:12 by hsarhan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

// Checks if a number is a digit
static bool	ft_isdigit(const int c)
{
	return (c >= '0' && c <= '9');
}

// Checks if an argument is numeric
static bool	is_num(const char *str)
{
	size_t	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (!ft_isdigit(str[i]) && str[i] != '+')
			return (false);
		if (str[i] == '+' && i != 0)
			return (false);
		if (str[i] == '+' && ft_strlen(str) == 1)
			return (false);
		i++;
	}
	return (true);
}

// Reads command line arguments and fills the simulation struct with the
// appropriate values
void	parse_args(t_sim *sim, const int argc, char **argv, bool *success)
{
	size_t	i;

	*success = true;
	if (argc != 5 && argc != 6)
	{
		*success = false;
		return ;
	}
	i = 1;
	while (i < (size_t) argc)
	{
		if (!is_num(argv[i]) || atoui(argv[i]) == FAIL || atoui(argv[i]) == 0)
			*success = false;
		i++;
	}
	if (!*success)
		return ;
	sim->num_phils = atoui(argv[1]);
	sim->time_to_die = atoui(argv[2]);
	sim->time_to_eat = atoui(argv[3]);
	sim->time_to_sleep = atoui(argv[4]);
	sim->min_eats = 0;
	if (argc == 6)
		sim->min_eats = atoui(argv[5]);
	*success = true;
}
