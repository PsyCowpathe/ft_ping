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
# include <time.h>
# include <unistd.h>
# include <stdarg.h>
# include <netinet/ip_icmp.h>
# include <arpa/inet.h>
# include <netdb.h>
# include <signal.h>
# include <math.h>

/****************** LIMITS ******************/

# define TTL_MAX 255
# define TTL_MIN 1
# define PACKET_MAX_SIZE 65399

/****************** ERRORS ******************/

# define ERROR_PRINT_TRY "ft_ping: %s\n\
Try \'ft_ping -?\' for more information.\n"
# define ERROR_PRINT "ft_ping: %s\n"
# define UNRECOGNIZED "unrecognized option '%s'"
# define TOO_SMALL "option value too small: %s"
# define TOO_BIG "option value too big: %s"
# define INVALID_OPTION "invalid option -- '%s'"
# define MISSING_VALUE "option requires an argument -- '%s'"
# define INVALID_VALUE "invalid value '%s'"
# define MISSING_HOST "missing host operand"
# define UNKNOW_HOST "unknow host"
# define ERROR_DNS "Error while converting IP address to text ! "
# define ERROR_REVERSE_DNS "could not resolve reverse dns of %s,\
 with error : \"%s\""
# define HOST_UNREACHABLE "%d bytes from %s: Destination Host Unreachable\n"
# define TTL_EXCEEDED "%d bytes from %s: Time to live exceeded\n"

/****************** SUCCESS ******************/

# define PREAMBLE "ft_ping %s (%s): %d data bytes\n"
# define VERBOSE_PREAMBLE "ft_ping %s (%s): %d data bytes, id 0x%04x:\n"
# define TICK_RESPONSE "%d bytes from %s: icmp_seq=%d ttl=%d time=%.3Lf\n"
# define TICK_RESPONSE_RDNS "%d bytes from %s (%s):\
 icmp_seq=%d ttl=%d time=%.3Lf\n"
# define TRANSMITED "%d packets transmitted, %d packets received,\
%.0lf%% packet loss\n"
# define RTT_STATS "round-trip min/avg/max/stddev =\
%.3Lf/%.3Lf/%.3Lf/%.3Lf ms\n"
# define ECHOREQUEST "Found an Echorequest, are you pinging yourself ?!\n"

typedef struct s_stats
{
	long double	rtt_min;
	long double	rtt_max;
	long double	total_rtt;
	long double	sqrd;
	uint32_t	send_count;
	uint32_t	receive_count;
}				t_stats;

typedef struct s_parameters
{
	struct addrinfo		*ip_address;
	struct icmphdr		*receive_header;
	struct ip			*ip_header;
	struct timespec		start;
	struct timespec		end;
	struct s_stats		stats;
	uint32_t			iteration;
	char				dns_name[NI_MAXHOST];
	int					socket_fd;

	/****************** FLAGS ******************/
	char				*string_original_target;
	char				string_ip_address[INET_ADDRSTRLEN];

	// Show Help menu (-?)
	bool				help;

	// Enable verbose output (-v)
	bool				verbose;

	// [BONUS] Show version (-V)
	bool				version;

	// [BONUS] Number of packet to send before end of ping execution (-c)
	uint32_t			count;
	char				*string_count;

	// [BONUS] Interval between two packet transmissions in seconds (-i)
	float				interval;
	char				*string_interval;

	// [BONUS] Time limit in second between start and end of ping execution (-w)
	int					timeout;
	char				*string_timeout;

	// [BONUS] Paquet size (in bytes) (-s)
	int					paquet_size;
	char				*string_paquet_size;

	// [BONUS] Number of router a paquet can go through before expiration (-ttl)
	int					time_to_live;
	char				*string_time_to_live;

	// [BONUS] Convert IP to a string address (-rdns)
	bool				reverse_dns;
}						t_parameters;

typedef struct s_pcket
{
	struct icmphdr	send_header;
	char			message[PACKET_MAX_SIZE];
}				t_pcket;

/****************** print_menu ******************/
void		print_version_menu(void);
void		print_help_menu(void);
int			enable_rdns(t_parameters *params);
int			enable_verbose(t_parameters *params);
void		print_preamble(t_parameters params);

/****************** parsing ******************/
char		*parse_flag_identifier(char *to_parse);
int			verify_flag_value(char *flag_id, char *flag_value);
int			verify_flag_limits(t_parameters *params);
int			parse_flag(t_parameters *params, char **args, int argc, int index);
int			parse_args(char **args, int argc, t_parameters *params);

/****************** utils ******************/
void		store_flag(t_parameters *params, char *flag_id, char *flag_value);
void		error_exit(int code, bool print_try, const char *msg, ...);
void		init_flag_structure(t_parameters *params);
int			check_is_float(char *flag_value);

/****************** dns ******************/
void		verify_target_address(t_parameters *params);

/****************** stats ******************/
long double	get_elapsed_time(t_parameters *params);
void		print_stats(t_parameters *params);
void		print_response(t_parameters *params, int sent, char *buffer);

/****************** network ******************/
void		create_header(t_parameters *params, t_pcket *packet);
int			create_socket(t_parameters *params);
double		get_time_seconds(void);

#endif