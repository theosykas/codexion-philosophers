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

typedef struct s_coders { // 1 coders/son_mutex
	int				id_coders;
	int				nb_compile;
	int				last_compile;
	pthread_t		thread;
	pthread_mutex_t	mutex;
	struct s_table	*table;
}	t_coders;


typedef struct s_dongles {
	long			cool_down;
	int				id_dongle;
	bool			is_used;
	pthread_mutex_t mutex;
}	t_dongles;

typedef struct s_arg {
	int		nb_coders;
	int		time_to_burn_out;
	int		time_to_compile;
	int		time_to_debug;
	int		time_to_refactor;
	int		number_of_compiles_required;
	int		dongle_cooldown;
	char	*scheduler;
}	t_arg;

typedef struct s_table {
	int				nb_coders;
	t_arg			arg;
	t_coders		*coders;
	t_dongles		*dongles;
}	t_table;


int		main(int ac, char **av);
int		check_args(int ac, char **av);
int		parse_args(t_arg *arg, int ac, char **av);

//parser.c
int		check_coders(char *coder);
int		get_time(void);

//utils.c
int		ft_isdigit(int c);
int		is_num_param(char *str);
void	*ft_calloc(size_t count, size_t size);
void	ft_bzero(void *s, size_t n);

//thread.c
void	*coders_routine(void *arg);
int		start_thread(t_table *table);

//init.c
int		init_coders(t_table *table);
int		init_dongle(t_table *table);
int		init_table(t_table *table);

#endif

// 0 1 has taken a dongle  prend une fourchette (0 == temps) (1 == coders id_1)
// 1 1 has taken a dongle attend une fourchhette (1 == 10000ms) (1 == coders id_1)
// 2 1 is compiling commence a compiler
// 201 1 is debugging 201ms apres coders id_1 __arm_legacy_debug_state

