#if  !defined( __NETDB__ )
#define __NETDB__

#if !defined( __UTYPES__ )
#define __UTYPES__
typedef	unsigned char uchar;
typedef	uchar					byte;
typedef unsigned short ushort;
typedef	unsigned int	uint;
typedef	unsigned long ulong;
#endif

typedef struct
{
	char	*h_name;
	char	**h_aliases;
	int		h_addrtype;
	int		h_length;
	char	**h_addr_list;
}hostent;
#define h_addr h_addr_list[0]

typedef struct
{
	char	*n_name;
	char	**n_aliases;
	int		n_addrtype;
	ulong	n_net;
}netent;

typedef struct
{
  char  *s_name;  /* official service name */
  char  **s_aliases;  /* alias list */
  int		s_port;   /* port # */
  char  *s_proto; /* protocol to use */
}servent;

typedef struct
{
	char	*p_name;
	char	**p_aliases;
	int		p_proto;
}protoent;

typedef struct
{
	char	*r_name;
	char	**r_aliases;
	long	r_number;
}rpcent;

extern hostent	*gethostbyname(const char *name);
extern hostent	*gethostbyaddr(const char *addr, int len, int type);
extern hostent	*gethostent(void);
extern int			sethostent(int stayopen);
extern int			endhostent(void);

extern netent		*getnetbyname(const char *name);
extern netent		*getnetbyaddr(int net, int type);
extern netent		*getnetent(void);
extern int			setnetent(int stayopen);
extern int			endnetent(void);

extern servent	*getservbyname(const char *name, const char *proto);
extern servent	*getservbyport(int port, const char *proto);
extern servent	*getservent(void);
extern int			setservent(int stayopen);
extern int			endservent(void);

extern protoent	*getprotobyname(const char *name);
extern protoent	*getprotobynumber(int proto);
extern protoent	*getprotoent(void);
extern int			setprotoent(int stayopen);
extern int			endprotoent(void);

extern rpcent		*getrpcbyname(const char *name);
extern rpcent		*getrpcbynumber(long number);
extern rpcent		*getrpcent(void);
extern int			setrpcent(int stayopen);
extern int			endrpcent(void);



/* This doesn't belong here */
extern ulong gethostid(void);
extern int		gethostname(char *name, int namelen);

#endif