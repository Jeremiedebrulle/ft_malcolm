#include "ft_malcolm.h"

/*
** ft_memcpy — copies n bytes from src to dst.
**
** Why we need our own: the standard memcpy() isn't on the subject's
** allowed function list, so we can't just #include <string.h> and call
** it. This is a straightforward byte-by-byte copy; nothing clever is
** needed since we're only ever copying tiny, fixed-size chunks (MAC
** addresses, IP addresses, whole packed structs).
*/
void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	unsigned char		*d;
	const unsigned char	*s;
	size_t				i;

	d = (unsigned char *)dst;
	s = (const unsigned char *)src;
	i = 0;
	while (i < n)
	{
		d[i] = s[i];
		i++;
	}
	return (dst);
}

/*
** ft_memset — fills n bytes at s with the value c.
**
** We use this to zero out our packet buffer before filling in fields
** (ft_memset(&packet, 0, sizeof(packet))). Zeroing first matters:
** if we only set the fields we care about and leave the rest as
** whatever garbage was on the stack, we'd leak stack memory contents
** onto the network — a real (if minor) info-leak bug, not just messy
** code.
*/
void	*ft_memset(void *s, int c, size_t n)
{
	unsigned char	*p;
	size_t			i;

	p = (unsigned char *)s;
	i = 0;
	while (i < n)
	{
		p[i] = (unsigned char)c;
		i++;
	}
	return (s);
}

/*
** ft_strlen — returns the length of a null-terminated string.
** Needed for basic string handling in argument parsing (e.g. checking
** a MAC address string is exactly 17 characters long).
*/
size_t	ft_strlen(const char *s)
{
	size_t	len;

	len = 0;
	while (s[len])
		len++;
	return (len);
}

/*
** ft_isxdigit — returns 1 if c is a valid hexadecimal digit
** (0-9, a-f, A-F), 0 otherwise. Used while parsing MAC address bytes,
** which are written in hex (e.g. "ff", "b1", "0a").
*/
int	ft_isxdigit(char c)
{
	if (c >= '0' && c <= '9')
		return (1);
	if (c >= 'a' && c <= 'f')
		return (1);
	if (c >= 'A' && c <= 'F')
		return (1);
	return (0);
}

/*
** ft_hexval — converts a single hex character to its numeric value
** (e.g. 'a' -> 10, 'F' -> 15, '3' -> 3). Returns -1 if c isn't a valid
** hex digit, so callers can detect malformed input without a separate
** ft_isxdigit() check every time.
*/
int	ft_hexval(char c)
{
	if (c >= '0' && c <= '9')
		return (c - '0');
	if (c >= 'a' && c <= 'f')
		return (c - 'a' + 10);
	if (c >= 'A' && c <= 'F')
		return (c - 'A' + 10);
	return (-1);
}
