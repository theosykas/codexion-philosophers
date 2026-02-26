/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: theo <theo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 19:55:43 by theo              #+#    #+#             */
/*   Updated: 2026/02/26 20:34:12 by theo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int check_args(int ac, char **av)
{
	int i;

	i = 1;
	(void)ac;
	while (i <= 7)
	{
		if (!is_num_param(av[i]))
		{
			fprintf(stderr, "Error you must try numeric value\n");
			return (1);
		}
		i++;
	}
	// if (ft_strcmp(av[8], "FIFO") != 0 && ft_strcmp(av[8], "EDF") != 0)
	// {
	// 	fprintf(stderr, "Error input you need to FIFO or EDF\n");
	// 	return (false);
	// }
	return (0);
}


int main(int ac, char **av)
{
	int count_coders;
	
	if (ac < 2)
	{
		fprintf(stderr, "Error");
		return (EXIT_FAILURE);
	}
	if (!check_args(ac, av))
		return (EXIT_FAILURE);
	count_coders = (nb_coders(av[1]));
	fprintf(stderr, "nb coders %d", count_coders);
	return (EXIT_FAILURE);
}


	//char *sceduleur_type;