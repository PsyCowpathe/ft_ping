/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirona <marvin@42.fr>                    +#+  +:+       +#+         */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/11 22:02:24 by agirona           #+#    #+#             */
/*   Updated: 2026/04/12 11:28:12 by agirona          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_ping.h"

int	get_neg_index(char *flag_value, int *current_index)
{
	int	i;

	i = 0;
	while (isspace(flag_value[i]))
		i++;
	*current_index = i;
	if (flag_value[i] == '-')
		return (i);
	return (-1);
}

int	check_is_float(char *flag_value)
{
	int	i;
	int	neg_i;
	int	dot_i;

	neg_i = get_neg_index(flag_value, &i);
	dot_i = -1;
	if (flag_value[i] == '\0')
		return (-1);
	while (flag_value[i])
	{
		if (isdigit(flag_value[i]) == 0)
		{
			if (flag_value[i] == '.')
			{
				if (dot_i == -1)
					dot_i = i;
				else
					return (-1);
			}
			if (i != neg_i && i != dot_i)
				return (-1);
		}
		i++;
	}
	return (0);
}

int	check_is_int(char *flag_value)
{
	int	i;
	int	neg_index;

	neg_index = get_neg_index(flag_value, &i);
	if (flag_value[i] == '\0')
		return (-1);
	while (flag_value[i])
	{
		if (isdigit(flag_value[i]) == 0)
		{
			if (i != neg_index)
				return (-1);
		}
		i++;
	}
	return (0);
}
