#include "ft_malcolm.h"

/*
** validate_ip — converts a dotted-decimal IPv4 string (e.g. "10.12.1.1")
** into 4 raw bytes, using inet_pton().
**
** Why inet_pton and not our own parser: it's explicitly on the allowed
** function list, and hand-rolling IPv4 parsing correctly (rejecting
** leading zeros ambiguity, out-of-range octets, wrong number of dots,
** trailing garbage, etc.) is exactly the kind of "reinventing a security
** boundary" work you don't want to do yourself when a correct, audited
** implementation is right there and permitted.
**
** inet_pton returns 1 on success, 0 if the string isn't a valid address
** in the given format, and -1 on a genuine error (e.g. bad address
** family). We only care about the success/failure distinction here.
*/
static int	validate_ip(const char *str, uint8_t out[IPV4_LEN])
{
	struct in_addr	addr;

	if (inet_pton(AF_INET, str, &addr) != 1)
		return (-1);
	ft_memcpy(out, &addr.s_addr, IPV4_LEN);
	return (0);
}

/*
** validate_mac — parses a MAC address string in the strict form
** "xx:xx:xx:xx:xx:xx" (exactly 6 groups of 2 hex digits, separated by
** colons, 17 characters total) into 6 raw bytes.
**
** We can't use inet_pton for this — it only understands IP address
** families, not link-layer addresses — and there's no MAC-parsing
** function on the allowed list, so this is hand-rolled. Being strict
** about the format (rejecting "aaa:bb:..." with 3 hex digits in a
** group, like the subject's error example) matters: a MAC parser that
** silently accepts malformed input could let us build a corrupted
** Ethernet frame without any indication why the attack didn't work.
*/
static int	validate_mac(const char *str, uint8_t out[MAC_LEN])
{
	int	i;
	int	hi;
	int	lo;

	if (ft_strlen(str) != 17)
		return (-1);
	i = 0;
	while (i < MAC_LEN)
	{
		hi = ft_hexval(str[i * 3]);
		lo = ft_hexval(str[i * 3 + 1]);
		if (hi == -1 || lo == -1)
			return (-1);
		if (i < MAC_LEN - 1 && str[i * 3 + 2] != ':')
			return (-1);
		out[i] = (uint8_t)((hi << 4) | lo);
		i++;
	}
	return (0);
}

/*
** parse_args — validates and converts all four positional arguments in
** order, stopping at the first failure (matching the subject's example
** behaviour: only one error is ever shown per run, for the first bad
** argument encountered).
**
** Returns 0 on success, -1 on failure (an error message has already
** been printed to stderr by this point, matching the "ft_malcolm: ..."
** format shown in the subject).
*/
int	parse_args(char **argv, t_args *args)
{
	if (validate_ip(argv[1], args->source_ip) == -1)
	{
		fprintf(stderr, "ft_malcolm: unknown host or invalid IP address: (%s).\n",
			argv[1]);
		return (-1);
	}
	if (validate_mac(argv[2], args->source_mac) == -1)
	{
		fprintf(stderr, "ft_malcolm: invalid mac address: (%s)\n", argv[2]);
		return (-1);
	}
	if (validate_ip(argv[3], args->target_ip) == -1)
	{
		fprintf(stderr, "ft_malcolm: unknown host or invalid IP address: (%s).\n",
			argv[3]);
		return (-1);
	}
	if (validate_mac(argv[4], args->target_mac) == -1)
	{
		fprintf(stderr, "ft_malcolm: invalid mac address: (%s)\n", argv[4]);
		return (-1);
	}
	return (0);
}
