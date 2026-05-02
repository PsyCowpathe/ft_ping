/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirona <marvin@42.fr>                    +#+  +:+       +#+         */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 22:02:24 by agirona           #+#    #+#             */
/*   Updated: 2026/04/12 11:28:12 by agirona          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_ping.h"

void	store_extra_flag(t_parameters *params, char *flag_id, char *flag_value)
{
	if (strcmp(flag_id, "ttl") == 0)
	{
		params->time_to_live = atoi(flag_value);
		params->string_time_to_live = flag_value;
	}
	else
		error_exit(1, true, INVALID_OPTION, flag_id);
}

void	store_flag(t_parameters *params, char *flag_id, char *flag_value)
{
	if (strcmp(flag_id, "c") == 0)
	{
		params->count = atoi(flag_value);
		params->string_count = flag_value;
	}
	else if (strcmp(flag_id, "i") == 0)
	{
		params->interval = atof(flag_value);
		params->string_interval = flag_value;
	}
	else if (strcmp(flag_id, "w") == 0)
	{
		params->timeout = atoi(flag_value);
		params->string_timeout = flag_value;
	}
	else if (strcmp(flag_id, "s") == 0)
	{
		params->paquet_size = atoi(flag_value);
		params->string_paquet_size = flag_value;
	}
	else
		store_extra_flag(params, flag_id, flag_value);
}

void	error_exit(int code, bool print_try, const char *msg, ...)
{
	va_list	args;
	int		len;
	char	*fmt_msg;

	va_start(args, msg);
	len = vsnprintf(NULL, 0, msg, args);
	va_end(args);
	if (len < 0)
		exit (67);
	fmt_msg = malloc(sizeof(char) * (len + 1));
	if (!fmt_msg)
		exit (68);
	va_start(args, msg);
	vsnprintf(fmt_msg, len + 1, msg, args);
	va_end(args);
	if (print_try == true)
		printf(ERROR_PRINT_TRY, fmt_msg);
	else
		printf(ERROR_PRINT, fmt_msg);
	free(fmt_msg);
	exit(code);
}

void	init_flag_structure(t_parameters *params)
{
	bzero(params, sizeof(t_parameters));
	params->iteration = 1;
	params->version = false;
	params->help = false;
	params->count = -1;
	params->interval = 1;
	params->timeout = 4;
	params->paquet_size = 56;
	params->time_to_live = 64;
	params->reverse_dns = false;
	params->verbose = false;
	params->dns_name[0] = '\0';
	params->stats.rtt_min = -1;
}

int	check_is_float(char *flag_value)
{
	int		i;
	bool	dot_found;

	i = 0;
	dot_found = false;
	while (flag_value[i] != '\0')
	{
		if (isdigit(flag_value[i]) == 0)
		{
			if (flag_value[i] != '.' || dot_found == true)
				return (-1);
			if (flag_value[i] == '.')
				dot_found = true;
		}
		i++;
	}
	return (0);
}
