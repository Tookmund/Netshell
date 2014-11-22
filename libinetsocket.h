#ifndef LIBINET_SOCKET_H_
#define LIBINET_SOCKET_H_
int create_inet_stream_socket(const char* host, const char* service, char proto_osi3, int flags);
int create_inet_dgram_socket(char proto_osi3, int flags);
ssize_t sendto_inet_dgram_socket(int sfd, const void* buf, size_t size, const char* host, const char* service, int sendto_flags);
ssize_t recvfrom_inet_dgram_socket(int sfd, void* buffer, size_t size, char* src_host, size_t src_host_len, char* src_service, size_t src_service_len, int recvfrom_flags, int numeric);
int connect_inet_dgram_socket(int sfd, const char* host, const char* service);
int destroy_inet_socket(int sfd);
int shutdown_inet_stream_socket(int sfd, int method);
int create_inet_server_socket(const char* bind_addr, const char* bind_port, char proto_osi4, char proto_osi3, int flags);
int accept_inet_stream_socket(int sfd, char* src_host, size_t src_host_len, char* src_service, size_t src_service_len, int flags, int accept_flags);
int create_multicast_socket(const char* group, const char* port, const char* if_name);

#endif
