/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thsykas <thsykas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/03 14:29:06 by thsykas           #+#    #+#             */
/*   Updated: 2026/03/05 15:36:27 by thsykas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
#define CODEXION_H

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

typedef struct s_dongles {
	long			cool_down;
	int				id_dongle;
	bool			is_used;
	struct t_coders	*waiting[2];
	pthread_mutex_t	mutex;
}	t_dongles;

//un coder == 2 dongle /codrs==pointeur vers 2 dongles
typedef struct s_coders { // 1 coders/son_mutex
	int				id_coders;
	int				nb_compile;
	long			last_compile;
	t_dongles		*left_dongle;
	t_dongles		*right_dongle;
	bool			burnout;
	struct s_table	*table;
	pthread_t		thread;
	pthread_mutex_t	mutex;
}	t_coders;

typedef struct s_arg {
	int		nb_coders;
	int		time_to_burn_out;
	int		time_to_compile;
	int		time_to_debug;
	int		time_to_refactor;
	int		dongle_cooldown;
	char	*scheduler;
	int		number_of_compiles_required;
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

//parser.c
int		check_coders(char *coder);
long	get_time(void);

//utils.c
void	*ft_calloc(size_t count, size_t size);
void	ft_bzero(void *s, size_t n);
int		is_num_param(char *str);
int		ft_isdigit(int c);

//thread.c
int		start_thread(t_table *table);
void	*monitoring(void *arg);
void	update_last_compile(t_coders *coders);

//init.c
int		init_coders(t_table *table);
int		init_dongle(t_table *table);
int		init_table(t_table *table);

//routine.c
// int		print_state(t_coders *coders, char *state);
// int		check_burnedout(t_coders *coders);
// void	take_dongle(t_coders *coders);
// int		is_active(t_coders *coders);
// void	*coders_routine(void *arg);

void	*coders_routine(void *arg);
void	*monitoring(void *arg);
int		check_burnedout(t_coders *coders, long action);
int		is_active(t_coders *coders);
void	take_dongle(t_coders *coders, t_dongles *dongles[2]);
int 	compiling(t_coders *coders, t_dongles *dongles[2]);
int		debbuging(t_coders *coders);
int		refactoring(t_coders *coders);
int		print_state(t_coders *coders, char *state);

#endif