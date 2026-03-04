/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thsykas <thsykas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 13:14:47 by thsykas           #+#    #+#             */
/*   Updated: 2026/03/04 16:45:11 by thsykas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	is_active(t_coders *coders)
{
	if (coders->nb_compile != coders->table->arg.number_of_compiles_required)
		return (1);
	return (0);
}

int	check_burnedout(t_coders *coders)
{
	int	check_burnout;

	pthread_mutex_lock(&coders->mutex);
	check_burnout = coders->burnout;
	pthread_mutex_unlock(&coders->mutex);
	return (check_burnout);
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
			pthread_mutex_lock(&coders[i].mutex);
			last_exec = coders[i].last_compile;
			pthread_mutex_unlock(&coders[i].mutex);
			if (get_time() - last_exec > coders->table->arg.time_to_burn_out)
			{
				pthread_mutex_lock(&coders[i].mutex); //lock pour flag == true
				coders[i].burnout = true; // burned out
				printf("%ld %d burned out\n", get_time() - coders[i].table->start_time, coders[i].id_coders);
				pthread_mutex_unlock(&coders[i].mutex);
				break; // exit routine
			}
		}
		usleep(1000);
	}
	return (NULL);
}

//Si nb_coders == 1, il va forcément burn out (cas particulier)
void	*coders_routine(void *arg)
{
	t_coders	*coders = (t_coders *)arg;

	while (is_active(coders) && !check_burnedout(coders))
	{
		take_dongle(coders);
		pthread_mutex_lock(&coders->mutex);
		coders->last_compile = get_time();
		pthread_mutex_unlock(&coders->mutex);
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

void	take_dongle(t_coders *coders)
{
	t_dongles	*left;
	t_dongles	*right;

	left = coders->left_dongle;
	right = coders->right_dongle;
	if (coders->id_coders % 2 == 0) //pair == gauche + droite
	{
		pthread_mutex_lock(&left->mutex);
		print_state(coders, "has taken a dongle");
		pthread_mutex_lock(&right->mutex);
		print_state(coders, "has taken a dongle");
	}
	else
	{
		pthread_mutex_lock(&right->mutex);
		print_state(coders, "has taken a dongle");
		pthread_mutex_lock(&left->mutex);
		print_state(coders, "has taken a dongle");
	}
}

void	print_state(t_coders *coders, char *state)
{
	long	current_time;

	pthread_mutex_lock(&coders->mutex);
	current_time = get_time() - coders->table->start_time;
	if (!coders->burnout)
		printf("%ld %d %s\n",current_time, coders->id_coders, state);
	pthread_mutex_unlock(&coders->mutex);
}

// 2045 last compile coder 1 / burn out 21346 21346 - 2045 == 1201