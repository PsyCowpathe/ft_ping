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

unsigned short	checksum(void *b, int len)
{
	unsigned short	*buf;
	unsigned int	sum;
	unsigned short	result;

	buf = b;
	sum = 0;
	while (len > 1)
	{
		sum += *buf++;
		len -= 2;
	}
	if (len == 1)
		sum += *(unsigned char *)buf;
	sum = (sum >> 16) + (sum & 0xFFFF);
	sum += (sum >> 16);
	result = ~sum;
	return (result);
}

void	create_header(t_pcket *packet, int iteration_count)
{
	long unsigned int	i;

	i = 0;
	packet->header.type = ICMP_ECHO;
	packet->header.un.echo.id = getpid();
	packet->header.un.echo.sequence = iteration_count;
	while (i < sizeof(packet->message) - 1)
	{
		packet->message[i] = i + '0';
		i++;
	}
	packet->header.checksum = 0;
	packet->header.checksum = checksum(&packet, sizeof(packet));
}

void	ft_ping(t_parameters params)
{
	t_pcket	packet;
	int		iteration_count;

	iteration_count = 1;
	while (true)
	{
		create_header(&packet, iteration_count);
		if (iteration_count >= params.count)
		{
			// need to print ping resume
			exit(0);
		}
		iteration_count++;
	}
}

int	main(int argc, char **argv)
{
	t_parameters	params;

	if (argc >= 2)
	{
		init_flag_structure(&params);
		if (parse_args(argv, argc, &params) == -1)
			return (1);
		ft_ping(params);
		return (0);
	}
	else
	{
		error_exit(1, true, MISSING_HOST);
	}
	return (0);
}
