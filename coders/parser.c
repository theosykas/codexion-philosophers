/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thsykas <thsykas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/26 19:56:00 by theo              #+#    #+#             */
/*   Updated: 2026/02/27 13:42:01 by thsykas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int get_time(void)
{
	long			time_in_ms;
	struct timeval	tv;

	if (gettimeofday(&tv, NULL))
		return (0);
	time_in_ms = (tv.tv_sec * 1000L) + (tv.tv_usec / 1000);
	return (time_in_ms);
}

int check_coders(char *coder)
{
	int nb_coders;

	nb_coders = atoi(coder);
	if (nb_coders <= 0)
	{
		fprintf(stderr, "Error invalid number of coders");
		return (1);
	}
	return (nb_coders);
}
