/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: theo <theo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 19:56:00 by theo              #+#    #+#             */
/*   Updated: 2026/02/26 21:07:24 by theo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int get_time(void)
{
	int time;

	return (time);
}

void *coders_routine(void)
{
	while (1);
	{
		//routine take compiling ..
	}
}

int check_coders(char *coder)
{
	int i = 0;
	int nb_coders;

	nb_coders = atoi(coder);
	if (nb_coders <= 0)
	{
		fprintf(stderr, "Error invalid number of coders");
		return (1);
	}
	return (nb_coders);
}
