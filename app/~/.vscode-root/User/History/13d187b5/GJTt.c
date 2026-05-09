/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stats.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirona <marvin@42.fr>                    +#+  +:+       +#+         */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/01 17:15:13 by agirona           #+#    #+#             */
/*   Updated: 2026/05/02 22:25:00 by agirona          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_ping.h"

long double	get_elapsed_time(t_parameters *params)
{
	double		elapsed;
	long double	rtt;

	elapsed = (double)((params->end.tv_nsec
				- params->start.tv_nsec) / 1000000.0);
	rtt = (params->end.tv_sec - params->start.tv_sec) * 1000.0 + elapsed;
	if (rtt > params->stats.rtt_max)
		params->stats.rtt_max = rtt;
	if (rtt < params->stats.rtt_min || params->stats.rtt_min == -1)
		params->stats.rtt_min = rtt;
	params->stats.total_rtt += rtt;
	params->stats.sqrd += rtt * rtt;
	return (rtt);
}

static long double	calculate_stddev(t_parameters *params)
{
	long double	mean;
	long double	smean;
	long double	stddev;

	mean = params->stats.total_rtt / params->stats.receive_count;
	smean = params->stats.sqrd / params->stats.receive_count;
	stddev = sqrt(smean - (mean * mean));
	return (stddev);
}

void	print_stats(t_parameters *params)
{
	double		loss;
	long double	avg;
	long double	stddev;

	if (params->stats.send_count != 0)
		loss = (params->stats.send_count - params->stats.receive_count)
			/ params->stats.send_count * 100.0;
	else
		loss = 100;
	stddev = calculate_stddev(params);
	avg = params->stats.total_rtt / params->stats.receive_count;
	printf(TRANSMITED,
		params->stats.send_count, params->stats.receive_count, loss);
	if (params->stats.receive_count != 0)
		printf(RTT_STATS,
			params->stats.rtt_min,
			avg,
			params->stats.rtt_max,
			stddev);
}

int	check_receive_response(t_parameters *params, int sent)
{
	if (params->receive_header->type == 3)
	{
		if (params->verbose == true)
			printf(HOST_UNREACHABLE, sent, params->dns_name);
		return (3);
	}
	else if (params->receive_header->type == 11)
	{
		if (params->verbose == true)
			printf(TTL_EXCEEDED, sent, params->dns_name);
		return (11);
	}
	else if (params->receive_header->type == 8)
	{
		printf("echoreply");
		return (8);
	}
	return (0);
}

void	print_response(t_parameters *params, int sent, char *buffer)
{
	long double	rtt;

	params->ip_header = (struct ip *)buffer;
	params->receive_header = (struct icmphdr *)((char *)buffer
			+ (params->ip_header->ip_hl * 4));
	if (check_receive_response(params, sent) == 0)
	{
		printf("printing response for iteration %d\n", params->iteration - 1);
		params->stats.receive_count++;
		rtt = get_elapsed_time(params);
		if (params->reverse_dns == true)
		{
			printf(TICK_RESPONSE_RDNS, sent, params->dns_name,
				params->string_ip_address,
				params->receive_header->un.echo.sequence,
				params->ip_header->ip_ttl, rtt);
		}
		else
		{
			printf(TICK_RESPONSE, sent, params->string_ip_address,
				params->receive_header->un.echo.sequence,
				params->ip_header->ip_ttl, rtt);
		}
	}
	fflush(stdout);
	free(buffer);
}
