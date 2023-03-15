#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <sys/mman.h>
#include <string>
#include <jnivm/vm.h>
#include <jnivm/env.h>
#include <fstream>
#include <sstream>
#include <sys/socket.h>

#ifdef NDEBUG
#define printf(...)
#endif

void (*mcpelauncher_preinithook)(const char*sym, void*val, void **orig);

cmsghdr* ___cmsg_nxthdr(msghdr* msg, cmsghdr* cmsg) {
  cmsghdr* ptr;
  ptr = reinterpret_cast<cmsghdr*>(reinterpret_cast<char*>(cmsg) + CMSG_ALIGN(cmsg->cmsg_len));
  size_t len = reinterpret_cast<char*>(ptr+1) - reinterpret_cast<char*>(msg->msg_control);
  if (len > msg->msg_controllen) {
    return nullptr;
  }
  return ptr;
}

int (*__socket_org)(int affamily, int type, int protocol);
int __socket(int affamily, int type, int protocol) {
    switch(affamily) {
        case AF_INET:
        case AF_INET6:
            return socket(affamily, type, protocol);
        default:
            errno = EAFNOSUPPORT;
            return 0;
    }
}

struct macOS_msghdr {
	void            *msg_name;      /* [XSI] optional address */
	socklen_t       msg_namelen;    /* [XSI] size of address */
	struct          iovec *msg_iov; /* [XSI] scatter/gather array */
	int             msg_iovlen;     /* [XSI] # elements in msg_iov */
	void            *msg_control;   /* [XSI] ancillary data, see below */
	socklen_t       msg_controllen; /* [XSI] ancillary data buffer len */
	int             msg_flags;      /* [XSI] flags on received message */
};
struct macOS_cmsghdr {
	socklen_t       cmsg_len;       /* [XSI] data byte count, including hdr */
	int             cmsg_level;     /* [XSI] originating protocol */
	int             cmsg_type;      /* [XSI] protocol-specific type */
/* followed by	unsigned char  cmsg_data[]; */
};

ssize_t __sendmsg(int socket, const struct msghdr *message, int flags) {
    macOS_msghdr mmessage;
    if(message->msg_namelen > 0) {
        printf("sendmsg message->msg_controllen > 0: %lld\n", (long long)message->msg_controllen);
    }
    mmessage.msg_name = nullptr; //message->msg_name;
    mmessage.msg_namelen = 0; //message->msg_namelen
    mmessage.msg_iov = message->msg_iov;
    mmessage.msg_iovlen = (decltype(mmessage.msg_iovlen))message->msg_iovlen;
    if(message->msg_controllen > 0) {
        printf("sendmsg message->msg_controllen > 0: %lld\n", (long long)message->msg_controllen);
    }
    mmessage.msg_control = nullptr;
    mmessage.msg_controllen = 0;
    mmessage.msg_flags = message->msg_flags;
    auto ret = sendmsg(socket, (struct msghdr *)&mmessage, flags);
    return ret;
}

ssize_t __recvmsg(int socket, struct msghdr *message, int flags) {
    macOS_msghdr mmessage;
    if(message->msg_namelen > 0) {
        printf("recvmsg message->msg_controllen > 0: %lld\n", (long long)message->msg_controllen);
    }
    mmessage.msg_name = nullptr; //message->msg_name;
    mmessage.msg_namelen = 0; //message->msg_namelen
    mmessage.msg_iov = message->msg_iov;
    mmessage.msg_iovlen = (decltype(mmessage.msg_iovlen))message->msg_iovlen;
    if(message->msg_controllen > 0) {
        printf("recvmsg message->msg_controllen > 0: %lld\n", (long long)message->msg_controllen);
    }
    mmessage.msg_control = nullptr;
    mmessage.msg_controllen = 0;
    mmessage.msg_flags = message->msg_flags;
    auto ret = recvmsg(socket, (struct msghdr *)&mmessage, flags);
    if(mmessage.msg_namelen > 0) {
        printf("recvmsg mmessage.msg_controllen > 0: %lld\n", (long long)mmessage.msg_controllen);
    }
    message->msg_name = nullptr; //mmessage.msg_name;
    message->msg_namelen = 0; //mmessage.msg_namelen
    message->msg_iov = mmessage.msg_iov;
    message->msg_iovlen = (decltype(message->msg_iovlen))mmessage.msg_iovlen;
    if(mmessage.msg_controllen > 0) {
        printf("recvmsg mmessage.msg_controllen > 0: %lld\n", (long long)mmessage.msg_controllen);
    }
    message->msg_control = nullptr;
    message->msg_controllen = 0;
    message->msg_flags = mmessage.msg_flags;
    return ret;
}

extern "C" void __attribute__ ((visibility ("default"))) mod_preinit() {
    auto h = dlopen("libmcpelauncher_mod.so", 0);
    if(!h) {
        return;
    }
    mcpelauncher_preinithook = (decltype(mcpelauncher_preinithook)) dlsym(h, "mcpelauncher_preinithook");
    dlclose(h);
    mcpelauncher_preinithook("__cmsg_nxthdr", (void*)&___cmsg_nxthdr, nullptr);
    mcpelauncher_preinithook("socket", (void*)&__socket, nullptr);
    mcpelauncher_preinithook("sendmsg", (void*)&__sendmsg, nullptr);
    mcpelauncher_preinithook("recvmsg", (void*)&__recvmsg, nullptr);
}
