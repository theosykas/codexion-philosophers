/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_thread.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: theo <theo@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/05 18:52:18 by theo              #+#    #+#             */
/*   Updated: 2026/03/05 18:56:05 by theo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

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

void	update_last_compile(t_coders *coders)
{
	pthread_mutex_lock(&coders->table->secure_mutex);
	coders->last_compile = get_time();
	pthread_mutex_unlock(&coders->table->secure_mutex);
}

long	get_time(void)
{
	long			time_in_ms;
	struct timeval	tv;

	if (gettimeofday(&tv, NULL))
		return (0);
	time_in_ms = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
	return (time_in_ms);
}
