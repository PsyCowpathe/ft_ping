/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dns.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agirona <marvin@42.fr>                    +#+  +:+       +#+         */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/18 19:48:10 by agirona           #+#    #+#             */
/*   Updated: 2026/04/22 20:42:43 by agirona          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_ping.h"

//convert DNS en IP
void	dns_lookup(t_parameters *params)
{
	struct addrinfo	hints;
	struct addrinfo	*result;

	bzero(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_RAW;
	hints.ai_protocol = IPPROTO_ICMP;
	if (getaddrinfo(params->string_ip_address, NULL, &hints, &result) != 0)
		error_exit(1, false, "unknow host");
	params->ip_address = (struct addrinfo *)result;
	if (inet_ntop(AF_INET,
			&(((struct sockaddr_in *)params->ip_address->ai_addr)->sin_addr),
			params->string_ip_address, INET_ADDRSTRLEN) == NULL)
	{
		perror("Error while converting IP address to text ! ");
		exit(1);
	}
}

// convert IP en DNS
void	reverse_dns_lookup(t_parameters *params)
{
	struct sockaddr_in	tmp_addr;
	socklen_t			len;
	int					ret;

	tmp_addr.sin_family = AF_INET;
	tmp_addr.sin_addr.s_addr = inet_addr(params->string_ip_address);
	len = sizeof(struct sockaddr_in);
	ret = getnameinfo((struct sockaddr *)&tmp_addr, len,
			params->dns_name, sizeof(params->dns_name), NULL, 0,
			NI_NAMEREQD);
	if (ret != 0)
		error_exit(1, false,
			"could not resolve reverse dns of %s, with error %s",
			params->string_ip_address, gai_strerror(ret));
}

void	verify_target_address(t_parameters *params)
{
	dns_lookup(params);
	reverse_dns_lookup(params);
}
