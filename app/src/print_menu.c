/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_options.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirona <marvin@42.fr>                    +#+  +:+       +#+         */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 17:15:13 by agirona           #+#    #+#             */
/*   Updated: 2026/04/10 19:45:23 by agirona          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_ping.h"

void	print_version_menu(void)
{
	printf("ft_ping (AGIRONA utils-net) version: 4.2\n");
	exit(0);
}

void	print_help_menu(void)
{
	printf("Usage: ft_ping [OPTION...] HOST ...\n\n");
	printf("Available options :\n\n");
	printf("-c, --c			stop after sending NUMBER packets\n");
	printf("-i, --i			wait NUMBER seconds between sending each packet\n");
	printf("-w, --w			stop after N seconds\n");
	printf("-s, --s			send NUMBER data octets\n");
	printf("-ttl, --ttl		specify N as time-to-live\n\n");
	printf("-V, --V			give this help list\n");
	printf("-?, --?			print program version\n");
	exit(0);
}

int	print_dns_option(t_parameters *params)
{
	params->reverse_dns = true;
	return (1);
}
