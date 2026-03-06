/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thsykas <thsykas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/03 14:29:06 by thsykas           #+#    #+#             */
/*   Updated: 2026/03/06 11:02:48 by thsykas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <stdio.h>
# include <pthread.h>
# include <stdbool.h>
# include <sys/time.h>
# include <stdint.h>
# include <unistd.h>
# include <stddef.h>

typedef struct s_coders t_coders;

typedef struct s_dongles {
	long			cool_down;
	long			last_drop;
	t_coders		*waiting[2];
	int				id_dongle;
	bool			is_used;
	pthread_mutex_t	mutex;
}	t_dongles;

typedef struct s_coders { // 1 coders/son_mutex
	long			last_compile;
	t_dongles		*left_dongle;
	t_dongles		*right_dongle;
	int				nb_compile;
	int				id_coders;
	bool			burnout;
	struct s_table	*table;
	pthread_t		thread;
	pthread_mutex_t	mutex;
}	t_coders;

typedef struct s_arg {
	int		number_of_compiles_required;
	int		time_to_refactor;
	int		time_to_burn_out;
	int		time_to_compile;
	int		dongle_cooldown;
	int		time_to_debug;
	char	*scheduler;
	int		nb_coders;
}	t_arg;

typedef struct s_table {
	pthread_mutex_t	secure_mutex;
	long			start_time;
	int				nb_coders;
	bool			stop;
	t_dongles		*dongles;
	t_coders		*coders;
	t_arg			arg;
}	t_table;

//codexion.c
int		parse_args(t_arg *arg, int ac, char **av);
int		check_args(int ac, char **av);
void	free_and_exit(t_table *table);
int		main(int ac, char **av);

//utils.c
void	*ft_calloc(size_t count, size_t size);
void	ft_bzero(void *s, size_t n);
int		is_num_param(char *str);
int		ft_isdigit(int c);

//thread.c
int		start_thread(t_table *table);

//init.c
int		init_coders(t_table *table);
int		init_dongle(t_table *table);
int		init_table(t_table *table);

//routine.c
int		is_active(t_coders *coders);
void	*coders_routine(void *arg);
void	*monitoring(void *arg);

//utils_thread.c
int		add_waiting(t_coders *coders, t_dongles *dongles);
int		print_state(t_coders *coders, char *state);
void	update_last_compile(t_coders *coders);
long	get_time(void);

//state_coders.c
int		take_dongle(t_coders *coders, t_dongles *dongles[2]);
int		compiling(t_coders *coders, t_dongles *dongles[2]);
int		check_burnout(t_coders *coders, long action);
int		refactoring(t_coders *coders);
int		debbuging(t_coders *coders);

#endif