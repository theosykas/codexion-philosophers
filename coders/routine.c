/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thsykas <thsykas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 13:14:47 by thsykas           #+#    #+#             */
/*   Updated: 2026/03/02 15:42:34 by thsykas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	is_active(t_coders *coders)
{
	if (coders->nb_compile == coders->table->arg.number_of_compiles_required)
		return (0); //stop
	return (1);
}

void	*coders_routine(void *arg)
{
	t_coders	*coders = (t_coders *)arg;

	while (is_active(coders))
	{
		take_dongle(coders);
		print_state(coders, "is compiling");
		usleep(coders->table->arg.time_to_compile * 1000); // 10000 conv ms/ arg[3] = 48 = usleep(480000)
		pthread_mutex_unlock(&coders->left_dongle->mutex);
		pthread_mutex_unlock(&coders->right_dongle->mutex);
		print_state(coders, "is debugging");
		usleep(coders->table->arg.time_to_debug * 1000);
		print_state(coders, "is refactoring");
		usleep(coders->table->arg.time_to_refactor * 1000);
		coders->nb_compile++;
	}
	return (NULL);
}

// print_state(coders, "burned out");
// usleep(coders->table->arg.time_to_burn_out * 1000);

void	take_dongle(t_coders *coders)
{
	t_dongles	*left;
	t_dongles	*right;
	long		current_time;

	left = coders->left_dongle;
	right = coders->right_dongle;
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

void	print_state(t_coders *coders, char *state)
{
	long current_time;

	current_time = get_time() - coders->table->time;
	printf("%ld %d %s\n", current_time, coders->id_coders, state);
}
