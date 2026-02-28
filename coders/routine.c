/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thsykas <thsykas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 13:14:47 by thsykas           #+#    #+#             */
/*   Updated: 2026/02/28 18:16:10 by thsykas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	*coders_routine(void *arg)
{
	t_coders	*coders = (t_coders *)arg;
	// long		time_past;

	while (1)
	{
		take_dongle(coders);
		// Travailler : C'est ici que tu utilises usleep(table->arg.time_to_compile * 1000).
	}
	return (NULL);
}

void	take_dongle(t_coders *coders)
{
	t_dongles	*left;
	t_dongles	*right;
	long		current_time;

	left = coders->left_dongle;
	right = coders->right_dongle;
	current_time = get_time() - coders->table->time;
	printf("\ncurr time %ld\n", current_time);
	if (coders->id_coders % 2 == 0) //pair == gauche + droite
	{
		pthread_mutex_lock(&left->mutex);
		current_time = get_time() - coders->table->time;
		printf("%ld %d has taken a dongle\n",current_time, coders->id_coders);
		pthread_mutex_lock(&right->mutex);
		current_time = get_time() - coders->table->time;
		printf("%ld %d has taken a dongle\n",current_time, coders->id_coders);
	}
	else
	{
		pthread_mutex_lock(&right->mutex);
		current_time = get_time() - coders->table->time;
		printf("%ld %d has taken a dongle\n",current_time, coders->id_coders);
		pthread_mutex_lock(&left->mutex);
		current_time = get_time() - coders->table->time;
		printf("%ld %d has taken a dongle\n", current_time, coders->id_coders);
	}
}

//refactor unlock
