/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirona <marvin@42.fr>                    +#+  +:+       +#+         */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 17:15:13 by agirona           #+#    #+#             */
/*   Updated: 2026/04/10 19:45:23 by agirona          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_ping.h"

int	main(int argc, char **argv)
{
	t_parameters	params;

	if (argc >= 2)
	{
		init_flag_structure(&params);
		if (parse_args(argv, argc, &params) == -1)
			return (1);
		return (0);
	}
	else
	{
		error_exit(1, true, MISSING_HOST);
	}
	return (0);
}
