#include <stdio.h>
#include "traceroute.h"

char				*g_hostname;
int					g_sendfd;
uint8_t 			g_inital_ttl;
uint8_t 			g_max_ttl;
uint8_t 			g_probes;
uint8_t 			g_waittime;
uint16_t			g_port;
char				g_rhostname[NI_MAXHOST];
struct addrinfo		*g_addrinfo;
int                 g_recvfd;

void			readopt(int ac, char **av);

void 	creat_sock(void)
{
	uid_t				uid;
	int					sendfd;
	int 				recvfd;
	struct sockaddr_in	serbind; 

	if ((uid = getuid()) != 0)
		FATAL("Need run premission to create raw socket.");

	recvfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	ERR_CHECK(recvfd == -1, "socket");
	sendfd = socket(AF_INET, SOCK_DGRAM, 0);
	ERR_CHECK(sendfd == -1, "socket");

	setuid(uid);
	
	serbind.sin_family = AF_INET;
	serbind.sin_addr.s_addr = INADDR_ANY;
	serbind.sin_port = htons(getpid() & 0xffff);
	if (bind(sendfd, (struct sockaddr *)(&serbind), sizeof(serbind)) == -1)
		INFO("Bind failed.");
	g_sendfd = sendfd;
	g_recvfd = recvfd;
}

void 	init(void)
{
	g_addrinfo = host_to_addrinfo(g_hostname, AF_INET, SOCK_DGRAM);
	ERR_CHECK(g_addrinfo == NULL, "host_to_addrinfo");
	if (getnameinfo(g_addrinfo->ai_addr, g_addrinfo->ai_addrlen, g_rhostname,\
													NI_MAXHOST, NULL, 0, 0) != 0)
		perror_("getnameinfo");
	g_inital_ttl = INITIAL_TTL;
	g_max_ttl = MAX_TTL;
	g_probes = DEFAULT_PROBE;
	g_waittime = DEFAULT_WAIT;
	g_port = DEFAULT_PORT_NUM;
}

void 	clean_up(void)
{
	close(g_recvfd);
	close(g_sendfd);
	freeaddrinfo(g_addrinfo);
}

void 	readloop(void)
{
	struct sockaddr_in 		serlast;
	//char					sendbuf[1024];

	for (u_int8_t ttl = g_inital_ttl; ttl < g_max_ttl; ttl++)
	{
		ERR_CHECK(setsockopt(g_sendfd, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl)) == -1,\
			"setsockopt");
		bzero_(&serlast, sizeof(serlast));
		printf("%2d ", ttl);
		fflush(stdout);
		for (uint8_t probe = 0; probe < g_probes; probe++)
		{
			//sendto(g_sendfd, )
		}
	}
}

int		main(int ac, char **av)
{
	readopt(ac, av);
	init();
	creat_sock();
	clean_up();
	return (0);
}