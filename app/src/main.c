/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirona <marvin@42.fr>                    +#+  +:+       +#+         */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/10 17:15:13 by agirona           #+#    #+#             */
/*   Updated: 2026/04/18 22:25:00 by agirona          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_ping.h"
#include <netinet/in.h>
#include <netinet/ip_icmp.h>
#include <strings.h>
#include <sys/socket.h>

bool	g_run;

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

void	create_header(t_parameters *params, t_pcket *packet, int iteration)
{
	int				i;

	i = 0;
	packet->send_header.code = 0;
	packet->send_header.type = ICMP_ECHO;
	packet->send_header.un.echo.id = getpid();
	packet->send_header.un.echo.sequence = iteration;
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
			&params->time_to_live, sizeof(params->time_to_live)) != 0)
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

void	stop_run(int sig)
{
	(void)sig;
	g_run = false;
}

int	send_ping(int socket_fd, t_parameters *params)
{
	t_pcket	packet;
	int		sent_bytes;

	bzero(&packet, sizeof(t_pcket));
	create_header(params, &packet, params->send_count);
	clock_gettime(CLOCK_MONOTONIC, &params->start);
	sent_bytes = sendto(socket_fd, &packet, sizeof(struct icmphdr)
			+ params->paquet_size, 0, params->ip_address->ai_addr,
			sizeof(struct sockaddr));
	if (sent_bytes <= 0)
	{
		printf("failure");
		//todo: variable pour dire quón a pas envoyer le packet et donc qu'on doit pas listen pour le retour
	}
	params->send_count++;
	return (sent_bytes);
}

char	*receive_response(int socket_fd, t_parameters *params)
{
	int		ret;
	char	buffer[PACKET_MAX_SIZE];

	ret = recv(socket_fd, &buffer, sizeof(buffer), 0);
	if (ret <= 0)
	{
		printf("%d + failed packet reception ! + %d\n",
			ret, params->send_count);
		//todo: do a thing ?
	}
	clock_gettime(CLOCK_MONOTONIC, &params->end);
	return (strdup(buffer));
}

long double	get_elapsed_time(t_parameters *params)
{
	double		elapsed;
	long double	rtt;

	elapsed = ((double)(params->end.tv_nsec
				- params->start.tv_nsec)) / 1000000.0;
	rtt = (params->end.tv_sec - params->start.tv_sec) * 1000.0 + elapsed;
	if (rtt > params->rtt_max)
		params->rtt_max = rtt;
	if (rtt < params->rtt_min)
		params->rtt_min = rtt;
	return (rtt);
}

void	print_stats(t_parameters *params)
{
	int	loss;

	loss = (params->send_count - params->receive_count) / params->send_count * 100.0;
	(void)loss;
	//double long mdev = sqrt((g_ping.time.sqrd / send_count) - (g_ping.time.avg * g_ping.time.avg));
	printf("%d packets transmitted, %d packets received, %d%% packet loss",
		params->send_count, params->receive_count, 0);
	//printf("rtt min/avg/max/mdev = %.3Lf/%.3Lf/%.3Lf/%.3Lf ms\n", min, g_ping.time.avg, max, mdev);
}

void	print_response(t_parameters *params, int sent,
			char *buffer, long double rtt)
{
	params->receive_count++;
	params->ip_header = (struct ip *)buffer;
	params->receive_header = (struct icmphdr *)((char *)buffer
			+ (params->ip_header->ip_hl * 4));
	params->ip_header->ip_ttl--;
	printf("%d bytes from %s: icmp_seq=%d ttl=%d time=%.3Lf\n",
		sent, params->dns_name, params->receive_header->un.echo.sequence,
		params->ip_header->ip_ttl, rtt);
	fflush(stdout);
	free(buffer);
}

void	ft_ping(t_parameters *params)
{
	int			socket_fd;
	int			sent_bytes;
	char		*response_buffer;
	long double	rtt;

	socket_fd = create_socket(params);
	signal(SIGINT, stop_run);
	while (g_run)
	{
		sent_bytes = send_ping(socket_fd, params);
		response_buffer = receive_response(socket_fd, params);
		rtt = get_elapsed_time(params);
		print_response(params, sent_bytes, response_buffer, rtt);
		usleep(1000000 * params->interval);
		if (params->send_count >= params->count)
		{
			// need to print ping resume
		}
	}
	print_stats(params);
}

int	main(int argc, char **argv)
{
	t_parameters	params;

	if (argc >= 2)
	{
		init_flag_structure(&params);
		if (parse_args(argv, argc, &params) == -1)
			return (1);
		g_run = true;
		ft_ping(&params);
		return (0);
	}
	else
	{
		error_exit(1, true, MISSING_HOST);
	}
	return (0);
}
