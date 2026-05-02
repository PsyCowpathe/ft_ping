/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirona <marvin@42.fr>                    +#+  +:+       +#+         */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 22:01:13 by agirona           #+#    #+#             */
/*   Updated: 2026/04/12 11:24:34 by agirona          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_ping.h"

char	*parse_flag_identifier(char *to_parse)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (to_parse[i] != '\0' && to_parse[i] == '-')
	{
		count++;
		i++;
	}
	if (count >= 3)
		error_exit(1, true, UNRECOGNIZED, to_parse);
	return (to_parse + i);
}

int	verify_flag_value(char *flag_id, char *flag_value)
{
	int		i;

	i = 0;
	if (flag_value[0] == '-')
		flag_value = flag_value + 1;
	if (flag_value[0] == '\0')
		return (-1);
	if (strcmp(flag_id, "i") == 0)
	{
		if (check_is_float(flag_value) == -1)
			return (-1);
	}
	else
	{
		while (flag_value[i] != '\0')
		{
			if (isdigit(flag_value[i]) == 0)
				return (-1);
			i++;
		}
	}
	return (0);
}

int	verify_flag_limits(t_parameters *params)
{
	if (params->time_to_live == 0)
		error_exit(1, false, TOO_SMALL, params->string_time_to_live);
	else if (params->time_to_live > TTL_MAX || params->time_to_live < TTL_MIN)
		error_exit(1, false, TOO_BIG, params->string_time_to_live);
	if (params->interval < 0.2)
	{
		if (getuid() != 0)
			error_exit(1, false, TOO_SMALL, params->string_interval);
	}
	if (params->paquet_size > PACKET_MAX_SIZE)
		error_exit(1, false, TOO_BIG, params->string_paquet_size);
	return (0);
}

int	parse_flag(t_parameters *params, char **args, int argc, int index)
{
	char	*flag_id;
	char	*flag_value;

	flag_id = parse_flag_identifier(args[index]);
	if (strcmp(flag_id, "V") == 0)
		print_version_menu();
	if (strcmp(flag_id, "?") == 0)
		print_help_menu();
	if (strcmp(flag_id, "rdns") == 0)
		return (enable_rdns(params));
	if (strcmp(flag_id, "v") == 0)
		return (enable_verbose(params));
	index++;
	if (index == argc)
		error_exit(1, true, MISSING_VALUE, flag_id);
	flag_value = args[index];
	if (verify_flag_value(flag_id, flag_value))
	{
		if (flag_id[0] == '\0')
			return (1);
		error_exit(1, true, INVALID_VALUE, flag_value);
	}
	store_flag(params, flag_id, flag_value);
	verify_flag_limits(params);
	return (0);
}

int	parse_args(char **args, int argc, t_parameters *params)
{
	int	i;
	int	ret;

	i = 1;
	while (i < argc)
	{
		if (args[i][0] == '-')
		{
			ret = parse_flag(params, args, argc, i);
			if (ret == -1)
				return (-1);
			if (ret == 0)
				i++;
		}
		else
		{
			params->string_original_target = args[i];
			verify_target_address(params);
		}
		i++;
	}
	return (0);
}
