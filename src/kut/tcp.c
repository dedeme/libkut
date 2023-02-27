// Copyright 23-Jan-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include <errno.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include "kut/tcp.h"
#include "kut/DEFS.h"

struct tcp_TcpServer {
  int id;
};

struct tcp_TcpConn {
  int id;
};

TcpServer *tcp_server (int port) {
  TcpServer *this = MALLOC(TcpServer);

  struct sockaddr_in server;
  int id = socket(AF_INET , SOCK_STREAM , 0);
	if (id == -1)
    EXC_IO("Could not create socket");

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(port);

	if(bind(id,(struct sockaddr *)&server , sizeof(server)) < 0)
		EXC_IO("bind failed. Error");

	listen(id, 3);

  this->id = id;
  return this;
}

// <TcpConn>
Rs *tcp_accept (TcpServer *sv) {
  TcpConn *r = MALLOC(TcpConn);

  struct sockaddr_in client;
	int c = sizeof(struct sockaddr_in);
	int id = accept(sv->id, (struct sockaddr *)&client, (socklen_t*)&c);
	if (id < 0)
    return rs_fail("accept failed");

  r->id = id;
  return rs_ok(r);
}

// <char>
Rs *tcp_read (TcpConn *conn) {
  // <Bytes>
  Rs *bs_rs = tcp_read_bin(conn);
  Bytes *bs = rs_get(bs_rs);
  if (!bs) return bs_rs;
  return rs_ok(bytes_to_str(bs));
}

// <Bytes>
Rs *tcp_read_bin (TcpConn *conn) {
  Bytes *r = bytes_new();
  int buffer = 8192;
  unsigned char bs[buffer];
  int len;
  if ((len = (int)recv(conn->id, bs, buffer, 0)) > 0)
    bytes_add_bytes(r, bs, len);

  if (len == -1) {
    close(conn->id);
    return rs_fail(str_f("Fail reading on connection: %s", strerror(errno)));
  }

  return rs_ok(r);
}

char *tcp_write (TcpConn *conn, char *s) {
  return tcp_write_bin(conn, bytes_from_str(s));
}

char *tcp_write_bin (TcpConn *conn, Bytes *bs) {
  if (send(conn->id, bytes_bs(bs), bytes_len(bs), 0) == -1)
     return str_f("Fail sending on connection: %s", strerror(errno));
  fsync(conn->id);
  return "";
}

// <TcpConn>
Rs *tcp_dial (char *sv, int port) {
  struct sockaddr_in server;
  int id = socket(AF_INET , SOCK_STREAM , 0);
	if (id == -1)
    return rs_fail("Could not create socket");

	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	if (!inet_aton(sv, &server.sin_addr)) {
    struct hostent *he;
    if ((he = gethostbyname(sv)) == NULL)
      return rs_fail(str_f("Host '%s' not found", sv));
    server.sin_addr = *(struct in_addr *) he->h_addr;
  }

	if (connect(id , (struct sockaddr *)&server , sizeof(server)) < 0)
    return rs_fail(str_f("Fail connection with '%s'", sv));

  TcpConn *r = MALLOC(TcpConn);
  r->id = id;
  return rs_ok(r);
}

void tcp_close_server (TcpServer *sv) {
  close(sv->id);
}

void tcp_close_conn (TcpConn *conn) {
  close(conn->id);
}
