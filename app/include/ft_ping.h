/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ping.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirona <marvin@42.fr>                    +#+  +:+       +#+         */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 16:08:53 by agirona           #+#    #+#             */
/*   Updated: 2026/04/11 19:50:42 by agirona          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PING_H
# define FT_PING_H

# include <ctype.h>
# include <stdio.h>
# include <string.h>
# include <stdbool.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdarg.h>

# define TTL_MAX 255
# define TTL_MIN 0

# define ERROR_PRINT_TRY "ft_ping: %s\n\
		Try \'ft_ping -?\' for more information.\n"
# define ERROR_PRINT "ft_ping: %s\n"
# define UNRECOGNIZED "unrecognized option '%s'"
# define TOO_SMALL "option value too small: %d"
# define TOO_SMALL_TTL "option value too small: %f"
# define TOO_BIG "option value too big: %d"
# define MISSING_VALUE "option requires an argument -- '%s'"
# define INVALID_VALUE "invalid value '%s'"
# define MISSING_HOST "missing host operand"

typedef struct s_parameters
{
	// Target IP address
	char		*ip_address;
	// Show version
	bool		version;
	// Show Help menu
	bool		help;
	// Number of packet to send before end of ping execution
	int			count;
	// Interval between two packet transmissions (in seconds)
	float		interval;
	// Time limit between start and end of ping execution (in seconds)
	int			timeout;
	// Paquet size (in bytes).
	int			paquet_size;
	// Number of router a paquet can fo through before expiration
	int			time_to_live;
}				t_parameters;

// print_menu
void	print_version_menu(void);
void	print_help_menu(void);

// parsing
char	*parse_flag_identifier(char *to_parse);
int		verify_flag_value(char *flag_id, char *flag_value);
int		verify_flag_limits(t_parameters *params);
int		parse_flag(t_parameters *params, char **args,
			int argc, int current_index);
int		parse_args(char **args, int argc, t_parameters *params);

// utils
void	store_flag(t_parameters *params, char *flag_id, char *flag_value);
void	error_exit(int code, bool print_try, const char *msg, ...);
void	init_flag_structure(t_parameters *params);
int		check_is_float(char *flag_value);

#endif