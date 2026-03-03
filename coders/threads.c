/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thsykas <thsykas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 21:09:11 by theo              #+#    #+#             */
/*   Updated: 2026/03/03 15:18:06 by thsykas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

//table contient tout coders/dongle.. central
int	start_thread(t_table *table)
{
	int		i;
	long	start_time;

	i = 0;
	start_time = get_time();
	while (i < table->nb_coders)
	{
		table->coders[i].last_compile = start_time;
		if (pthread_create(&table->coders[i].thread, NULL, &coders_routine,
			&table->coders[i])) //exec
			return (1);
		i++;
	}
	if (pthread_create(&table->monitoring_thread, NULL, &monitoring, table))
		return (1);
	pthread_join(table->monitoring_thread, NULL);
	i = 0;
	while (i < table->nb_coders)
	{
		pthread_join(table->coders[i].thread, NULL); // attend fin exec thread
		i++;
	}
	return (0);
}
