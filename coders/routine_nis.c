/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_nis.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thsykas <thsykas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/05 14:11:16 by thsykas           #+#    #+#             */
/*   Updated: 2026/03/05 16:03:49 by thsykas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	is_active(t_coders *coders)
{
	if (coders->nb_compile == coders->table->arg.number_of_compiles_required)
		return (0);
	if (check_burnedout(coders, 0))
		return (0);
	return (1);
}

int	check_burnedout(t_coders *coders, long action)
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

int compiling(t_coders *coders, t_dongles *dongles[2])
{
	int	i;

	i = -1;
	if (check_burnedout(coders, 0)) /// 0 pour recompiler 
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
	if (check_burnedout(coders, coders->table->arg.time_to_debug))
		return (0);
	if (!print_state(coders, "is debugging"))
		return (0);
	usleep(coders->table->arg.time_to_debug * 1000);
	return (1);
}

int	refactoring(t_coders *coders)
{
	if (check_burnedout(coders, coders->table->arg.time_to_refactor))
		return (0);
	if (!print_state(coders, "is refactor"))
		return (0);
	usleep(coders->table->arg.time_to_refactor * 1000); // 10000 conv ms/ arg[3] = 48 = usleep(480000)
	return (1);
}

int	print_state(t_coders *coders, char *state)
{
	long	current_time;

	if (!is_active(coders) || coders->table->stop == 1) // si stop == 1 on arrete tout les print a un burnout
		return (0);
	pthread_mutex_lock(&coders->table->secure_mutex); //mutex write 
	current_time = get_time() - coders->table->start_time;
	printf("%ld %d %s\n",current_time, coders->id_coders, state);
	pthread_mutex_unlock(&coders->table->secure_mutex);
	return (1);
}
