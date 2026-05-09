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
#include <stdio.h>

bool	g_run;

void	stop_run(int sig)
{
	(void)sig;
	g_run = false;
}

int	send_ping(t_parameters *params)
{
	t_pcket	packet;
	int		sent_bytes;

	bzero(&packet, sizeof(t_pcket));
	create_header(params, &packet);
	clock_gettime(CLOCK_MONOTONIC, &params->start);
	sent_bytes = sendto(params->socket_fd, &packet, sizeof(struct icmphdr)
			+ params->paquet_size, 0, params->ip_address->ai_addr,
			sizeof(struct sockaddr));
	if (sent_bytes < 0)
		perror("Could not send packet ! ");
	else
		params->stats.send_count++;
	return (sent_bytes);
}

char	*receive_response(t_parameters *params)
{
	int		ret;
	char	buffer[PACKET_MAX_SIZE];
	char	*result;

	ret = recv(params->socket_fd, &buffer, sizeof(buffer), 0);

	if (ret <= 0)
	{
		return (NULL);
	}
	clock_gettime(CLOCK_MONOTONIC, &params->end);
	result = malloc(PACKET_MAX_SIZE);
	memcpy(result, buffer, PACKET_MAX_SIZE);




	/*uint16_t id = getpid() & 0xFFFF;

	struct ip *ip_hdr = (struct ip *)buffer;
	int ip_hdr_len = ip_hdr->ip_hl * 4;

	struct icmphdr *icmp_hdr = (struct icmphdr *)(buffer + ip_hdr_len);
	if (icmp_hdr->type == ICMP_ECHOREPLY && ntohs(icmp_hdr->un.echo.id) == id)
	{
	    printf("Reply valide seq=%d\n", icmp_hdr->un.echo.sequence);
	}
	else
	{
		printf("looking for = %d and %d\n", ICMP_ECHOREPLY, id);
	    printf("type = %d\n", icmp_hdr->type);
	    printf("id = %d\n", ntohs(icmp_hdr->un.echo.id));

	}*/


	return (result);
}

void	ft_ping(t_parameters *params)
{
	int			sent_bytes;
	char		*response_buffer;
	double		last_time;
	double		current_time;

	last_time = 0.0;
	current_time = 0.0;
	params->socket_fd = create_socket(params);
	signal(SIGINT, stop_run);
	while (g_run)
	{
		current_time = get_time_seconds();
		if (current_time - last_time > params->interval || last_time == 0.0)
		{
			last_time = current_time;
			sent_bytes = send_ping(params);
			response_buffer = receive_response(params);
			if (response_buffer != NULL)
				print_response(params, sent_bytes, response_buffer);
			if (params->iteration >= params->count)
				g_run = false;
			params->iteration++;
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
		if (params.string_original_target == NULL)
			error_exit(1, true, MISSING_HOST);
		g_run = true;
		print_preamble(params);
		ft_ping(&params);
		return (0);
	}
	else
	{
		error_exit(1, true, MISSING_HOST);
	}
	return (0);
}
