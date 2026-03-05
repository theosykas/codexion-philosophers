/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state_coders.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: theo <theo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/05 18:51:36 by theo              #+#    #+#             */
/*   Updated: 2026/03/05 19:02:12 by theo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	check_burnout(t_coders *coders, long action)
{
	(void)action;
	pthread_mutex_lock(&coders->table->secure_mutex);
	if (coders->burnout)
	{
		pthread_mutex_unlock(&coders->table->secure_mutex);
		return (1);
	}
	pthread_mutex_unlock(&coders->table->secure_mutex);
	return (coders->burnout);
}

// && dongles[i]->cool_down < get_time() && waiting[0]
void	take_dongle(t_coders *coders, t_dongles *dongles[2])
{
	int	i;

	i = -1;
	while (++i < 2)
	{
		// add_waiting(coders, dongles[i]);
		pthread_mutex_lock(&dongles[i]->mutex);
		if (!dongles[i]->is_used)
		{
			dongles[i]->is_used = 1; //taken
			print_state(coders, "has taken a dongle");
		}
		pthread_mutex_unlock(&dongles[i]->mutex);
	}
}

int	compiling(t_coders *coders, t_dongles *dongles[2])
{
	int	i;

	i = -1;
	if (check_burnout(coders, 0)) /// 0 pour recompiler 
		return (0);
	update_last_compile(coders);
	if (!print_state(coders, "is compiling"))
		return (0);
	usleep(coders->table->arg.time_to_compile * 1000); // 10000 conv ms/ arg[3] = 48 = usleep(480000)
	while (++i < 2)
	{
		pthread_mutex_lock(&dongles[i]->mutex);
		dongles[i]->is_used = false; //drop
		pthread_mutex_unlock(&dongles[i]->mutex);
	}
	pthread_mutex_lock(&coders->table->secure_mutex);
	coders->nb_compile++;
	pthread_mutex_unlock(&coders->table->secure_mutex);
	return (1);
}

int	debbuging(t_coders *coders)
{
	if (check_burnout(coders, coders->table->arg.time_to_debug))
		return (0);
	if (!print_state(coders, "is debugging"))
		return (0);
	usleep(coders->table->arg.time_to_debug * 1000);
	return (1);
}

int	refactoring(t_coders *coders)
{
	if (check_burnout(coders, coders->table->arg.time_to_refactor))
		return (0);
	if (!print_state(coders, "is refactor"))
		return (0);
	usleep(coders->table->arg.time_to_refactor * 1000); // 10000 conv ms/ arg[3] = 48 = usleep(480000)
	return (1);
}
