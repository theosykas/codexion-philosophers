/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: theo <theo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 21:09:11 by theo              #+#    #+#             */
/*   Updated: 2026/03/05 18:55:56 by theo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	start_thread(t_table *table)
{
	int		i;
	long	start_time;

	i = 0;
	start_time = table->start_time;
	while (i < table->nb_coders)
	{
		table->coders[i].last_compile = start_time;
		if (pthread_create(&table->coders[i].thread, NULL, &coders_routine,
			&table->coders[i])) //exec
			return (1);
		i++;
	}
	monitoring(table->coders);
	i = 0;
	while (i < table->nb_coders)
	{
		pthread_join(table->coders[i].thread, NULL); // attend fin exec thread
		i++;
	}
	return (0);
}
