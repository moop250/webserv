#include <string>

template <typename integer>
static void	helper(integer nb, char str[100], int *index)
{
	if (nb >= 10)
		helper(nb / 10, str, index);
	str[(*index)++] = (nb % 10) + '0';
	str[*index] = '\0';
}

template <typename integer>
std::string	tostring(integer n)
{
    char        c[100];
	int		    index;
    std::string s;

    for (int i = 0; i < 100; i++)
	{
        c[i] = '\0';
	}
	index = 0;
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
    s = c;
	return (s);
}
