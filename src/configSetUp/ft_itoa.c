/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aseite <marvin@42lausanne.ch>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/10 19:27:02 by aseite            #+#    #+#             */
/*   Updated: 2024/10/18 15:54:50 by aseite           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	gl(int n)
{
	int	count;

	count = 0;
	if (n <= 0)
		count = 1;
	while (n != 0)
	{
		n /= 10;
		count++;
	}
	return (count + 1);
}

void	helper(int nb, char *str, int *index)
{
	if (nb >= 10)
		helper(nb / 10, str, index);
	str[(*index)++] = (nb % 10) + '0';
	str[*index] = '\0';
}

char	*ft_itoa(int n)
{
	char	*c;
	int		index;

	index = 0;
	c = (char *)malloc(gl(n) * sizeof(char));
	if (!c)
		return (NULL);
	if (n == -2147483648)
	{
		c[0] = '-';
		c[1] = '2';
		n += 2000000000;
		n = -n;
		index += 2;
	}
	else if (n < 0)
	{
		c[index++] = '-';
		n = -n;
	}
	helper(n, c, &index);
	return (c);
}
/*
#include <stdio.h>

int	main()
{
	int	n = 0;

	printf("%s \n", ft_itoa(n));
	return (0);
}*/
