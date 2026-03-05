/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: theo <theo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/05 14:11:16 by thsykas           #+#    #+#             */
/*   Updated: 2026/03/05 19:02:27 by theo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	is_active(t_coders *coders)
{
	if (coders->nb_compile == coders->table->arg.number_of_compiles_required)
		return (0);
	if (check_burnout(coders, 0))
		return (0);
	return (1);
}

void	*monitoring(void *arg)
{
	t_coders	*coders = (t_coders *)arg;
	long		last_exec;
	int			i;
	
	while (1)
	{
		i = -1;
		while (++i < coders->table->nb_coders)
		{
			pthread_mutex_lock(&coders->table->secure_mutex);
			last_exec = coders[i].last_compile;
			pthread_mutex_unlock(&coders->table->secure_mutex);
			if (!is_active(&coders[i]))
				return (NULL);
			if (get_time() - last_exec > coders->table->arg.time_to_burn_out)
			{
				pthread_mutex_lock(&coders->table->secure_mutex);
				coders[i].burnout = true; // burned out
				printf("%ld %d burned out\n", get_time() - coders[i].table->start_time, coders[i].id_coders);
				coders->table->stop = 1; // stop write 
				pthread_mutex_unlock(&coders->table->secure_mutex);
				return (NULL);
			}
		}
		usleep(1000); // laisse le temps de unlock les mutex
	}
	return (NULL);
}

void	*coders_routine(void *arg)
{
	t_coders	*coders = (t_coders *)arg;
	t_dongles	*dongles[2];

	dongles[coders->id_coders & 1] = coders->left_dongle; // 1 = 00000001 & 1 // pair impair
	dongles[!(coders->id_coders & 1)] = coders->right_dongle;
	while (is_active(coders))
	{
		take_dongle(coders, dongles);
		if (!compiling(coders, dongles))
			break;
		if (!debbuging(coders))
			break;
		if (!refactoring(coders))
			break;
	}
	return (NULL);
}
