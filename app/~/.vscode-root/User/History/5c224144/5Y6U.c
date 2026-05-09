/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   network.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirona <marvin@42.fr>                    +#+  +:+       +#+         */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 17:15:13 by agirona           #+#    #+#             */
/*   Updated: 2026/05/02 22:25:00 by agirona          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_ping.h"

double	get_time_seconds(void)
{
	struct timespec	ts;

	clock_gettime(CLOCK_MONOTONIC, &ts);
	return (ts.tv_sec + ts.tv_nsec / 1e9);
}

bool	is_echo_request(char *buffer)
{
	struct ip			*ip_header;
	struct icmphdr		*receive_header;

	ip_header = (struct ip *)buffer;
	receive_header = (struct icmphdr *)((char *)buffer
			+ (ip_header->ip_hl * 4));
		
	if (receive_header->type == ICMP_ECHO)
		return true;
	return false;
}

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

void	create_header(t_parameters *params, t_pcket *packet)
{
	int				i;

	i = 0;
	packet->send_header.code = 0;
	packet->send_header.type = ICMP_ECHO;
	packet->send_header.un.echo.id = getpid();
	fflush(stdout);
	packet->send_header.un.echo.sequence = params->iteration - 1;
	while (i < params->paquet_size)
	{
		packet->message[i] = i + '0';
		i++;
	}
	packet->send_header.checksum = 0;
	packet->send_header.checksum = checksum(packet,
			sizeof(struct icmphdr) + (params->paquet_size));
}

int	create_socket(t_parameters *params)
{
	struct timeval	timeout;
	int				socket_fd;

	timeout.tv_sec = params->timeout;
	timeout.tv_usec = 0;
	socket_fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (socket_fd < 0)
	{
		perror("Can't create socket ! ");
		exit(1);
	}
	if (setsockopt(socket_fd, SOL_IP, IP_TTL,
			&params->time_to_live, sizeof(params->time_to_live)) != 0)
	{
		perror("Error while setting TTL option ! ");
		exit(1);
	}
	if (setsockopt(socket_fd, SOL_SOCKET,
			SO_RCVTIMEO, &timeout, sizeof(timeout)))
	{
		perror("could not set sockopts timeout!");
		exit(1);
	}
	return (socket_fd);
}