/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thsykas <thsykas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 19:55:43 by theo              #+#    #+#             */
/*   Updated: 2026/02/28 18:10:31 by thsykas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int parse_args(t_arg *arg, int ac, char **av)
{
	(void)ac;
	arg->nb_coders = atoi(av[1]);
	arg->time_to_burn_out = atoi(av[2]);
	arg->time_to_compile = atoi(av[3]);
	arg->time_to_debug = atoi(av[4]);
	arg->time_to_refactor = atoi(av[5]);
	arg->number_of_compiles_required = atoi(av[6]);
	arg->dongle_cooldown = atoi(av[7]);
	return (0);
}

int check_args(int ac, char **av)
{
	int i;

	i = 1;
	if (ac != 9)
	{
		fprintf(stderr, "Error");
		return (EXIT_FAILURE);
	}
	while (i <= 7)
	{
		if (!is_num_param(av[i]))
		{
			fprintf(stderr, "Error you must try numeric value\n");
			return (1);
		}
		i++;
	}
	if (strcmp(av[8], "fifo") != 0 && strcmp(av[8], "edf") != 0)
	{
		fprintf(stderr, "Error input you need to fifo or edf\n");
		return (1);
	}
	return (0);
}

int main(int ac, char **av)
{
	t_table	table;

	if (ac != 9)
	{
		fprintf(stderr, "Error invalid input\n");
		return (EXIT_FAILURE);
	}
	if (check_args(ac, av))
		return (EXIT_FAILURE);
	parse_args(&table.arg, ac, av);
	table.nb_coders = table.arg.nb_coders;
	init_table(&table);
	table.time = get_time();
	init_coders(&table);
	init_dongle(&table);
	start_thread(&table);
	return (EXIT_FAILURE);
}
