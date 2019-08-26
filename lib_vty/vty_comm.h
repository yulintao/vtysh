#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#ifdef HAVE_STROPTS_H
#include <stropts.h>
#endif /* HAVE_STROPTS_H */
#ifdef HAVE_SYS_SELECT_H
#include <sys/select.h>
#endif /* HAVE_SYS_SELECT_H */
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/param.h>
#ifdef HAVE_SYS_SYSCTL_H
#ifdef GNU_LINUX
#include <linux/types.h>
#endif
#include <sys/sysctl.h>
#endif /* HAVE_SYS_SYSCTL_H */
#include <sys/ioctl.h>
#ifdef HAVE_SYS_CONF_H
#include <sys/conf.h>
#endif /* HAVE_SYS_CONF_H */
#ifdef HAVE_SYS_KSYM_H
#include <sys/ksym.h>
#endif /* HAVE_SYS_KSYM_H */
#include <syslog.h>
#ifdef TIME_WITH_SYS_TIME
# include <sys/time.h>
# include <time.h>
#else
# ifdef HAVE_SYS_TIME_H
#  include <sys/time.h>
# else
#  include <time.h>
# endif
#endif /* TIME_WITH_SYS_TIME */
#include <sys/uio.h>
#include <sys/utsname.h>
#ifdef HAVE_RUSAGE
#include <sys/resource.h>
#endif /* HAVE_RUSAGE */
#ifdef HAVE_LIMITS_H
#include <limits.h>
#endif /* HAVE_LIMITS_H */
#ifdef HAVE_INTTYPES_H
#include <inttypes.h>
#endif /* HAVE_INTTYPES_H */
#ifdef HAVE_STDBOOL_H
#include <stdbool.h>
#endif
/* primarily for __STDC_IEC_559__ with clang */
#ifdef HAVE_FEATURES_H
#include <features.h>
#endif

#ifdef HAVE_ASM_TYPES_H
#include <asm/types.h>
#endif /* HAVE_ASM_TYPES_H */

/* misc include group */
#include <stdarg.h>
#if !(defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L)
/* Not C99; do we need to define va_copy? */
#ifndef va_copy
#ifdef __va_copy
#define va_copy(DST,SRC) __va_copy(DST,SRC)
#else
/* Now we are desperate; this should work on many typical platforms. 
   But this is slightly dangerous, because the standard does not require
   va_copy to be a macro. */
#define va_copy(DST,SRC) memcpy(&(DST), &(SRC), sizeof(va_list))
#warning "Not C99 and no va_copy macro available, falling back to memcpy"
#endif /* __va_copy */
#endif /* !va_copy */
#endif /* !C99 */

/* network include group */

#include <sys/socket.h>

#ifdef HAVE_SYS_SOCKIO_H
#include <sys/sockio.h>
#endif /* HAVE_SYS_SOCKIO_H */

#ifdef HAVE_NETINET_IN_H
#include <netinet/in.h>
#endif /* HAVE_NETINET_IN_H */
#include <netinet/in_systm.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>

#ifdef HAVE_NET_NETOPT_H
#include <net/netopt.h>
#endif /* HAVE_NET_NETOPT_H */

#include <net/if.h>

#ifdef HAVE_NET_IF_DL_H
#include <net/if_dl.h>
#endif /* HAVE_NET_IF_DL_H */

#ifdef HAVE_NET_IF_VAR_H
#include <net/if_var.h>
#endif /* HAVE_NET_IF_VAR_H */

#ifdef HAVE_NET_ROUTE_H
#include <net/route.h>
#endif /* HAVE_NET_ROUTE_H */

#ifdef HAVE_NETLINK
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <linux/filter.h>
#else
#define RT_TABLE_MAIN		0
#endif /* HAVE_NETLINK */

#ifdef HAVE_NETDB_H
#include <netdb.h>
#endif /* HAVE_NETDB_H */

#include <arpa/inet.h>

#ifdef HAVE_INET_ND_H
#include <inet/nd.h>
#endif /* HAVE_INET_ND_H */

#ifdef HAVE_NETINET_IN_VAR_H
#include <netinet/in_var.h>
#endif /* HAVE_NETINET_IN_VAR_H */

#ifdef HAVE_NETINET6_IN6_VAR_H
#include <netinet6/in6_var.h>
#endif /* HAVE_NETINET6_IN6_VAR_H */

#ifdef HAVE_NETINET_IN6_VAR_H
#include <netinet/in6_var.h>
#endif /* HAVE_NETINET_IN6_VAR_H */

#ifdef HAVE_NETINET6_IN_H
#include <netinet6/in.h>
#endif /* HAVE_NETINET6_IN_H */


#ifdef HAVE_NETINET6_IP6_H
#include <netinet6/ip6.h>
#endif /* HAVE_NETINET6_IP6_H */

#ifdef HAVE_NETINET_ICMP6_H
#include <netinet/icmp6.h>
#endif /* HAVE_NETINET_ICMP6_H */

#ifdef HAVE_NETINET6_ND6_H
#include <netinet6/nd6.h>
#endif /* HAVE_NETINET6_ND6_H */

/* Privileges / capabilities */
#ifdef HAVE_LCAPS
#include <sys/capability.h>
#include <sys/prctl.h>
#endif /* HAVE_LCAPS */

#ifdef HAVE_SOLARIS_CAPABILITIES
#include <priv.h>
#endif /* HAVE_SOLARIS_CAPABILITIES */

/* Some systems do not define UINT32_MAX, etc.. from inttypes.h
 * e.g. this makes life easier for FBSD 4.11 users.
 */
#ifndef INT8_MAX
#define INT8_MAX	(127)
#endif
#ifndef INT16_MAX
#define INT16_MAX	(32767)
#endif
#ifndef INT32_MAX
#define INT32_MAX	(2147483647)
#endif
#ifndef UINT8_MAX
#define UINT8_MAX	(255U)
#endif
#ifndef UINT16_MAX
#define UINT16_MAX	(65535U)
#endif
#ifndef UINT32_MAX
#define UINT32_MAX	(4294967295U)
#endif

#ifdef HAVE_GLIBC_BACKTRACE
#include <execinfo.h>
#endif /* HAVE_GLIBC_BACKTRACE */

/* MAX / MIN are not commonly defined, but useful */
#ifndef MAX
#define MAX(a, b) \
	({ typeof (a) _a = (a); \
	   typeof (b) _b = (b); \
	   _a > _b ? _a : _b; })
#endif
#ifndef MIN
#define MIN(a, b) \
	({ typeof (a) _a = (a); \
	   typeof (b) _b = (b); \
	   _a < _b ? _a : _b; })
#endif

#define ZEBRA_NUM_OF(x) (sizeof (x) / sizeof (x[0]))

/* Zebra message types. */
#define ZEBRA_INTERFACE_ADD                1
#define ZEBRA_INTERFACE_DELETE             2
#define ZEBRA_INTERFACE_ADDRESS_ADD        3
#define ZEBRA_INTERFACE_ADDRESS_DELETE     4
#define ZEBRA_INTERFACE_UP                 5
#define ZEBRA_INTERFACE_DOWN               6
#define ZEBRA_IPV4_ROUTE_ADD               7
#define ZEBRA_IPV4_ROUTE_DELETE            8
#define ZEBRA_IPV6_ROUTE_ADD               9
#define ZEBRA_IPV6_ROUTE_DELETE           10
#define ZEBRA_REDISTRIBUTE_ADD            11
#define ZEBRA_REDISTRIBUTE_DELETE         12
#define ZEBRA_REDISTRIBUTE_DEFAULT_ADD    13
#define ZEBRA_REDISTRIBUTE_DEFAULT_DELETE 14
#define ZEBRA_IPV4_NEXTHOP_LOOKUP         15
#define ZEBRA_IPV6_NEXTHOP_LOOKUP         16
#define ZEBRA_IPV4_IMPORT_LOOKUP          17
#define ZEBRA_IPV6_IMPORT_LOOKUP          18
#define ZEBRA_INTERFACE_RENAME            19
#define ZEBRA_ROUTER_ID_ADD               20
#define ZEBRA_ROUTER_ID_DELETE            21
#define ZEBRA_ROUTER_ID_UPDATE            22
#define ZEBRA_HELLO                       23
#define ZEBRA_IPV4_NEXTHOP_LOOKUP_MRIB    24
#define ZEBRA_VRF_UNREGISTER              25
#define ZEBRA_INTERFACE_LINK_PARAMS       26
#define ZEBRA_NEXTHOP_REGISTER            27
#define ZEBRA_NEXTHOP_UNREGISTER          28
#define ZEBRA_NEXTHOP_UPDATE              29
#define ZEBRA_MESSAGE_MAX                 30


/* Address family numbers from RFC1700. */
typedef enum {
  AFI_IP  = 1,
  AFI_IP6 = 2,
  AFI_ETHER = 3,                /* RFC 1700 has "6" for 802.* */
#define AFI_MAX 4
} afi_t;



