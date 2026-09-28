#ifndef FT_MALCOLM_H
# define FT_MALCOLM_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <signal.h>
# include <errno.h>
# include <stdint.h>
# include <sys/socket.h>
# include <sys/ioctl.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <net/if.h>
# include <net/ethernet.h>
# include <netpacket/packet.h>
# include <ifaddrs.h>

/*
** ---------------------------------------------------------------------
** Constants
** ---------------------------------------------------------------------
** ARP hardware type "Ethernet" is 1, protocol type "IPv4" is 0x0800.
** ARP opcodes: 1 = request, 2 = reply. These are defined by RFC 826 /
** the IANA ARP parameters registry, not something we get to choose.
*/
# define ARP_HTYPE_ETHERNET	1
# define ARP_PTYPE_IPV4		0x0800
# define ARP_OP_REQUEST		1
# define ARP_OP_REPLY		2

# define MAC_LEN			6
# define IPV4_LEN			4
# define ETH_FRAME_MAX		1518

/*
** ---------------------------------------------------------------------
** t_eth_header — the 14-byte Ethernet II header that precedes every
** frame on the wire. We build this by hand because we're crafting a
** raw frame ourselves rather than letting the kernel's IP stack do it.
**
** __attribute__((packed)) tells the compiler not to insert any padding
** bytes between fields for alignment purposes. Without it, the struct's
** in-memory layout might not match the exact byte layout the network
** expects, and we'd send garbage.
** ---------------------------------------------------------------------
*/
typedef struct __attribute__((packed)) s_eth_header
{
	uint8_t		dst_mac[MAC_LEN];
	uint8_t		src_mac[MAC_LEN];
	uint16_t	ethertype;
}	t_eth_header;

/*
** ---------------------------------------------------------------------
** t_arp_header — the ARP packet body itself, as defined in RFC 826.
** This is what actually carries the "who has this IP" / "this IP is at
** this MAC" information. All multi-byte integer fields (htype, ptype,
** oper) must be sent in network byte order (big-endian) — see htons()
** usage in arp.c.
** ---------------------------------------------------------------------
*/
typedef struct __attribute__((packed)) s_arp_header
{
	uint16_t	htype;
	uint16_t	ptype;
	uint8_t		hlen;
	uint8_t		plen;
	uint16_t	oper;
	uint8_t		sha[MAC_LEN];
	uint8_t		spa[IPV4_LEN];
	uint8_t		tha[MAC_LEN];
	uint8_t		tpa[IPV4_LEN];
}	t_arp_header;

/*
** t_arp_packet — the full frame we send/receive: Ethernet header
** immediately followed by the ARP header, exactly as it appears on
** the wire. Combining them in one packed struct lets us read/write
** the whole frame with a single cast over a raw byte buffer.
*/
typedef struct __attribute__((packed)) s_arp_packet
{
	t_eth_header	eth;
	t_arp_header	arp;
}	t_arp_packet;

/*
** ---------------------------------------------------------------------
** t_args — the four validated command-line arguments, converted from
** text into raw bytes we can drop directly into a t_arp_packet.
** ---------------------------------------------------------------------
*/
typedef struct s_args
{
	uint8_t	source_ip[IPV4_LEN];
	uint8_t	source_mac[MAC_LEN];
	uint8_t	target_ip[IPV4_LEN];
	uint8_t	target_mac[MAC_LEN];
}	t_args;

/*
** ---------------------------------------------------------------------
** Minimal libft — reimplementations of the handful of libc functions
** we'd otherwise want, since string.h's memcpy/memset/strlen aren't on
** the subject's allowed function list.
** ---------------------------------------------------------------------
*/
void	*ft_memcpy(void *dst, const void *src, size_t n);
void	*ft_memset(void *s, int c, size_t n);
size_t	ft_strlen(const char *s);
int		ft_isxdigit(char c);
int		ft_hexval(char c);

/*
** ---------------------------------------------------------------------
** args.c — argument parsing and validation
** ---------------------------------------------------------------------
*/
int		parse_args(char **argv, t_args *args);

#endif
