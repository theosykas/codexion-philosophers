/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thsykas <thsykas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 21:07:26 by theo              #+#    #+#             */
/*   Updated: 2026/03/06 12:46:26 by thsykas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	init_coders(t_table *table)
{
	int	i;

	i = -1;
	while (++i < table->nb_coders)
	{
		table->coders[i].id_coders = i + 1; // id_1/2.. start 1
		table->coders[i].nb_compile = 0;
		table->coders[i].burnout = false;
		table->coders[i].last_compile = get_time();
		table->coders[i].left_dongle = &table->dongles[i];
		table->coders[i].right_dongle = &table->dongles[(i + 1) %
			table->nb_coders];
		table->coders[i].table = table; // acces a toute la table
		if (pthread_mutex_init(&table->coders[i].mutex, NULL) != 0) // init utilisation lock
			return (1);
	}
	return (0);
}

int	init_dongle(t_table *table)
{
	int	i;

	i = -1;
	while (++i < table->nb_coders)
	{
		table->dongles[i].id_dongle = i + 1;
		table->dongles[i].cool_down = 0;
		table->dongles[i].last_drop = 0;
		table->dongles[i].is_used = false;
		table->dongles[i].waiting[0] = NULL;
		table->dongles[i].waiting[1] = NULL;
		if (pthread_mutex_init(&table->dongles[i].mutex, NULL) != 0)
			return (1);
	}
	return (0);
}

int	init_table(t_table *table)
{
	table->start_time = get_time();
	table->stop = 0;
	table->coders = ft_calloc(sizeof(t_coders), table->nb_coders);
	if (!table->coders)
		return (1);
	table->dongles = ft_calloc(sizeof(t_dongles), table->nb_coders);
	if (!table->dongles)
		return (1);
	if (pthread_mutex_init(&table->secure_mutex, NULL) != 0)
		return (1);
	return (0);
}
