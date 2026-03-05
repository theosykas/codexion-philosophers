/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thsykas <thsykas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 13:14:47 by thsykas           #+#    #+#             */
/*   Updated: 2026/03/05 14:45:14 by thsykas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	is_active(t_coders *coders)
{
	if (check_burnedout(coders))
		return (0);
	if (coders->nb_compile == coders->table->arg.number_of_compiles_required)
		return (0);
	return (1);
}

int	check_burnedout(t_coders *coders)
{
	pthread_mutex_lock(&coders->table->secure_mutex);
	if (get_time() - coders->last_compile > coders->table->arg.time_to_burn_out)
			coders->burnout = true;
	pthread_mutex_unlock(&coders->table->secure_mutex);
	return (coders->burnout);
}

// usleep(coders->table->arg.time_to_debug * 1000);
// quand on lit avec le monitoring (last_compile) pour check - le last_compile va etre mis a jour
//si le temps ecouler depuis la derniere compilation depasse le time_bur_out -> coders == burn out
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
			if (get_time() - last_exec > coders->table->arg.time_to_burn_out)
			{
				pthread_mutex_lock(&coders->table->secure_mutex);
				coders[i].burnout = true; // burned out
				printf("%ld %d burned out\n", get_time() - coders[i].table->start_time, coders[i].id_coders);
				pthread_mutex_unlock(&coders->table->secure_mutex);
				break;
			}
		}
		usleep(1000); // laisse le temps de unlock les mutex
		if (coders[i].burnout)
			return (NULL);
		}
	return (NULL);
}
//le usleep de (100) laisse le temps a chaques coders sa routine de prendre dongle and drop
// update last_compile, routine

//Si nb_coders == 1, il va forcément burn out (cas particulier)
void	*coders_routine(void *arg)
{
	t_coders	*coders = (t_coders *)arg;
	t_dongles	*dongles[2];

	dongles[coders->id_coders & 1] = coders->left_dongle; // 1 = 00000001 & 1
	dongles[!(coders->id_coders & 1)] = coders->right_dongle
	while (is_active(coders))
	{
		take_dongle(coders);
		pthread_mutex_lock(&coders->table->secure_mutex);
		coders->last_compile = get_time();
		pthread_mutex_unlock(&coders->table->secure_mutex);
		print_state(coders, "is compiling");
		usleep(coders->table->arg.time_to_compile * 1000); // 10000 conv ms/ arg[3] = 48 = usleep(480000)
		coders->nb_compile++;
		pthread_mutex_unlock(&coders->left_dongle->mutex);
		pthread_mutex_unlock(&coders->right_dongle->mutex);
		if (coders->nb_compile == coders->table->arg.number_of_compiles_required)
			return (NULL);
		if (!print_state(coders, "is debugging"))
			return (NULL);
		usleep(coders->table->arg.time_to_debug * 1000);
		if (!print_state(coders, "is refactoring"))
			return (NULL);
		usleep(coders->table->arg.time_to_refactor * 1000);
	}
	return (NULL);
}

void	debugging(t_coders *coder)
{
	if (check_burnout(coder, coder->table->arg.time_to_debug))
		return (0);
	usleep(coder->table->arg.time_to_debug * 1000);
	print_state(coder, "is_debugging");
}

void	take_dongle(t_coders *coders, t_dongles *dongles[2])
{
	int	i;

	i = -1;
	while (++i < 2)
	{
		add_waiting(coders, dongles[i])
		if (!dongles[i]->is_used && dongles[i]->cool_down < get_time() && waiting[0])
		{
			pthread_mutex_lock(&dongles[i]);
			dongles[i]->is_used = 1;
			pthread_mutex_unlock(&dongles[i]);
			print_state(coders, "has taken a dongle");
		}
	}
}

int	print_state(t_coders *coders, char *state)
{
	long	current_time;

	if (!is_active(coders))
		return (0);
	pthread_mutex_lock(&coders->table->secure_mutex); //mutex write 
	current_time = get_time() - coders->table->start_time;
	if (!coders->burnout)
		printf("%ld %d %s\n",current_time, coders->id_coders, state);
	pthread_mutex_unlock(&coders->table->secure_mutex);
	return (1);
}

2045 last compile coder 1 / burn out 21346 21346 - 2045 == 1201

./codexion 2 1500 400 300 300 400 500 fifo 1500 > 400 + 300

2 200 20 25 100 50 500 fifo cycle 200ms burnout > time_comp = 20 + time_deb = 25 + time refactor = 100 == 145 
145 < 200 = temps complet de cylce 