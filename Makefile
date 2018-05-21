KMOD=	icmp_hook
SRCS=	icmp_hook.c
CFLAGS+= -g

.include <bsd.kmod.mk>
