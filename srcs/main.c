#include "ft_malcolm.h"

/*
** print_usage — shown when the wrong number of arguments is given.
** Not explicitly required by the subject's examples, but silently
** doing nothing (or crashing on argv[5] that doesn't exist) would
** violate the "no unexpected crashes" rule the moment someone runs
** the program with too few arguments.
*/
static void	print_usage(void)
{
	fprintf(stderr,
		"Usage: ft_malcolm <source_ip> <source_mac> <target_ip> <target_mac>\n");
}

int	main(int argc, char **argv)
{
	t_args	args;

	if (argc != 5)
	{
		print_usage();
		return (1);
	}
	if (getuid() != 0)
	{
		fprintf(stderr,
			"ft_malcolm: this program requires root privileges (raw sockets).\n");
		return (1);
	}
	if (parse_args(argv, &args) == -1)
		return (1);
	/*
	** ---------------------------------------------------------------
	** From here, still to build:
	**   1. Find an available network interface (iface.c, getifaddrs)
	**   2. Open a raw AF_PACKET socket bound to it (socket.c)
	**   3. Install a SIGINT handler so Ctrl+C exits cleanly (signal.c)
	**   4. Loop on recvfrom() until we see an ARP request asking for
	**      args.source_ip, sent by args.target_mac (arp.c)
	**   5. Build and sendto() a single spoofed ARP reply
	**   6. Clean up (close the socket) and exit
	** ---------------------------------------------------------------
	*/
	printf("Arguments validated successfully. Next: interface + socket.\n");
	return (0);
}
