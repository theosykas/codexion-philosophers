/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state_coders.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thsykas <thsykas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 11:07:00 by thsykas           #+#    #+#             */
/*   Updated: 2026/03/06 15:56:14 by thsykas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	check_burnout(t_coders *coders, long action)
{
	(void)action;
	pthread_mutex_lock(&coders->table->secure_mutex);
	if (coders->burnout || coders->table->stop)
	{
		pthread_mutex_unlock(&coders->table->secure_mutex);
		return (1);
	}
	pthread_mutex_unlock(&coders->table->secure_mutex);
	return (0);
}

// (get_time() >= dongles[i]->cool_down (1100 >= 1500 == false) dongle bloquer
int	take_dongle(t_coders *coders, t_dongles *dongles[2])
{
	int		i;

	i = -1;
	while (++i < 2)
	{
		add_waiting(coders, dongles[i]);
		while (1)
		{
			if (check_burnout(coders, 0))
				return (0);
			pthread_mutex_lock(&dongles[i]->mutex);
			if (!dongles[i]->is_used &&
				(dongles[i]->waiting[0] == coders) &&
				(get_time() >= dongles[i]->last_drop))
			{
				dongles[i]->is_used = true; //taken
				dongles[i]->waiting[0] = dongles[i]->waiting[1];
				dongles[i]->waiting[1] = NULL;
				print_state(coders, "has taken a dongle");
				pthread_mutex_unlock(&dongles[i]->mutex);
				break ;
			}
			pthread_mutex_unlock(&dongles[i]->mutex);
		}
	}
	return (1);
}

//100ms (get_time()) + dongle_cool_down = 500
//dongle_cool[i] = 1000 + 500
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
		dongles[i]->last_drop = get_time() + coders->table->arg.dongle_cooldown;//lock dongle -> que le temps passe 
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
