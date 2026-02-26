#ifndef CODEXION_H
#define CODEXION_H

# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <pthread.h>
# include <stdbool.h>

typedef struct s_coders {
	int 			id_coders;
	pthread_mutex_t	*right;
	pthread_mutex_t	*left;
	struct s_tabe 	*table;
}	t_coders;


typedef struct s_tabe {
	int 			nb_coders;
	pthread_mutex_t *all_dongles;
	t_coders 		*all_coders;
}	t_tabe;

int main(int ac, char **av);
int check_args(int ac, char **av);

//parser.c
int check_coders(char *coder);

//utils.c
int	ft_isdigit(int c);
int is_num_param(char *str);
int ft_strcmp(char *s1, char *s2);


//init.c
int init_table(t_tabe *table, char **av);
int init_coders(t_coders *coders, char **av);

#endif

// 0 1 has taken a dongle  prend une fourchette (0 == temps) (1 == coders id_1)
// 1 1 has taken a dongle attend une fourchhette (1 == 10000ms) (1 == coders id_1)
// 2 1 is compiling commence a compiler
// 201 1 is debugging 201ms apres coders id_1 __arm_legacy_debug_state

