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
#include <netinet/in.h>
#include <netinet/ip_icmp.h>
#include <strings.h>
#include <sys/socket.h>

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

void	create_header(t_parameters params, t_pcket *packet, int iteration_count)
{
	int				i;

	i = 0;
	packet->header.code = 0;
	packet->header.type = ICMP_ECHO;
	packet->header.un.echo.id = getpid();
	packet->header.un.echo.sequence = iteration_count;
	while (i < params.paquet_size - 1)
	{
		packet->message[i] = i + '0';
		i++;
	}
	packet->header.checksum = 0;
	packet->header.checksum = checksum(&packet,
			sizeof(struct icmphdr) + params.paquet_size);
}

int	create_socket(t_parameters params)
{
	struct timeval	tv_out;
	int				socket_fd;

	tv_out.tv_sec = 4;
	tv_out.tv_usec = 0;
	socket_fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (socket_fd < 0)
	{
		perror("Can't create socket ! ");
		exit(1);
	}
	if (setsockopt(socket_fd, SOL_IP, IP_TTL,
			&params.time_to_live, sizeof(params.time_to_live)) != 0)
	{
		perror("Error while setting TTL option ! ");
		exit(1);
	}
	if (setsockopt(socket_fd, SOL_SOCKET, SO_RCVTIMEO, &tv_out, sizeof(tv_out)))
	{
		perror("could not set sockopts timeout!");
		exit(1);
	}
	return (socket_fd);
}

void	ft_ping(t_parameters params)
{
	t_pcket	packet;
	int		iteration_count;
	int		socket_fd;

	iteration_count = 1;
	socket_fd = create_socket(params);
	while (true)
	{
		bzero(&packet, sizeof(t_pcket));
		create_header(params, &packet, iteration_count);
		if (sendto(socket_fd, &packet, sizeof(struct icmphdr)
				+ params.paquet_size, 0, params.ip_address->ai_addr,
				sizeof(struct sockaddr)) <= 0)
		{
			printf("failure");
			//todo: variable pour dire quón a pas envoyer le packet et donc qu'on doit pas listen pour le retour
		}
		else
		{
			printf("succedd");
		}
		usleep(1000);
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
