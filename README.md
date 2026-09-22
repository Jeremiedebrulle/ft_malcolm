# This project has been created as part of the 42 curriculum by <jdebrull>

## ft_malcolm
 
An introduction to Man-in-the-Middle attacks — a minimal ARP spoofing tool written in C, built as part of the 42 cybersecurity curriculum.
 
`ft_malcolm` waits for a target to broadcast an ARP request for a given source IP, then sends a single spoofed ARP reply mapping that source IP to an attacker-controlled MAC address — poisoning the target's ARP table.
 
> ⚠️ **Educational use only.** Only run this against IP addresses and networks you own or have explicit authorization to test. Spoofing ARP tables on networks you don't control is illegal in most jurisdictions.
 
## Requirements
 
- Linux (raw sockets, `AF_PACKET`)
- A C compiler (`gcc`/`clang`) and `make`
- Root privileges (raw socket creation requires `CAP_NET_RAW`)
- Run inside a VM on a network you control — see [Setup](#setup) below
## Build
 
```bash
make
```
 
Produces the `ft_malcolm` executable. Usual rules included: `make`, `make clean`, `make fclean`, `make re`.
 
## Usage
 
```bash
sudo ./ft_malcolm <source_ip> <source_mac> <target_ip> <target_mac>
```
 
| Argument      | Description                                                        |
|---------------|---------------------------------------------------------------------|
| `source_ip`   | The IP address whose identity you're spoofing                       |
| `source_mac`  | The (attacker-controlled) MAC address to associate with `source_ip` |
| `target_ip`   | The victim machine whose ARP table will be poisoned                 |
| `target_mac`  | The victim's real MAC address                                       |
 
### Example
 
```
$ sudo ./ft_malcolm 10.12.255.255 ff:bb:ff:ff:ee:ff 10.12.10.22 10:dd:b1:aa:bb:cc
Found available interface: eth0
An ARP request has been broadcast.
mac address of request: 10:dd:b1:aa:bb:cc
IP address of request: 10.12.255.255
Now sending an ARP reply to the target address with spoofed source, please wait...
Sent an ARP reply packet, you may now check the arp table on the target.
Exiting program...
```
 
The program exits automatically after sending the single reply, or cleanly on `Ctrl+C`.
 
## How it works
 
1. Validates all four arguments (IP format, MAC format) and exits with a clear error message on malformed input.
2. Locates an available network interface via `getifaddrs`.
3. Opens a raw `AF_PACKET` socket bound to that interface.
4. Listens for an ARP request (opcode `1`) from the target, asking for `source_ip`.
5. Constructs and sends a single spoofed ARP reply (opcode `2`), telling the target that `source_ip` lives at `source_mac`.
6. Exits.
See [RFC 826](https://www.rfc-editor.org/rfc/rfc826) for the ARP packet format this implementation is built against.
 
## Setup
 
This project is meant to be built and run inside a VM (Debian recommended) on a network you control, with the VM's network adapter set to **Bridged** mode so it can see real ARP traffic on the LAN.
 
## Constraints
 
Per the subject:
- Only one global variable used across the project
- No unexpected crashes (segfault, bus error, double free) — every syscall's return value is checked
- Only the allowed function set is used in the mandatory part (see subject for the full list)
