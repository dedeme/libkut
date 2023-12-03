// Copyright 23-Jan-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

#include <errno.h>
#include <signal.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include "kut/tcp.h"
#include "kut/DEFS.h"

struct tcp_TcpServer {
  int id;
};

struct tcp_TcpConn {
  int id;
};

TcpServer *tcp_server (int port, int conns) {
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

  listen(id, conns);

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

// <Bytes>
Rs *tcp_read (TcpConn *conn, int len, int seconds) {
  fd_set set;
  struct timeval timeout;
  FD_ZERO (&set);
  FD_SET (conn->id, &set);
  timeout.tv_sec = seconds;
  timeout.tv_usec = 0;

  int rsel;
  for (;;) {
    rsel = select (FD_SETSIZE, &set, NULL, NULL, &timeout);
    if (rsel < 0 && errno == EINTR) continue;
    break;
  }
  if (rsel < 0)
    return rs_fail(str_f(
      "Fail reading on connection (select): %s", strerror(errno)
    ));
  if (rsel == 0)
    return rs_fail("Time out");

  int len1 = len + 1;
  unsigned char bs[len1];
  int rlen;
  for (;;) {
    rlen = (int)read(conn->id, bs, len1);
    if (rlen < 0 && errno == EINTR) continue;
    break;
  }
  if (rlen < 0) {
    close(conn->id);
    return rs_fail(str_f(
      "Fail reading on connection (recv): %s", strerror(errno)
    ));
  }
  if (rlen > len)
    return rs_fail("Connection overflow");

  int tt = rlen;
  if (!memcmp("POST ", bs, 5)) {
    unsigned char *pbs = bs;
    int ix = -1;
    int ibody = -1;
    for (int i = 0; i < 5000; ++i, ++pbs) {
      if (!memcmp("Content-Length: ", pbs, 16)) {
        ix = i + 16;
      } else if (!memcmp("\r\n\r\n", pbs, 4)) {
        ibody = i + 4;
      }
    }
    if (ix != -1 && ibody != -1) {
      char sn[9];
      memcpy(sn, bs + ix, 8);
      sn[8] = 0;
      int post_len = atoi(sn) + ibody;

      while (tt < post_len) {
        for (;;) {
          rlen = (int)read(conn->id, bs + tt, len1 - tt);
          if (rlen < 0 && errno == EINTR) continue;
          break;
        }
        if (rlen < 0) {
          close(conn->id);
          return rs_fail(str_f(
            "Fail reading on connection (recv): %s", strerror(errno)
          ));
        }
        if (rlen > len)
          return rs_fail("Connection overflow");
        if (rlen == 0)
          return rs_fail("Incomplete POST request");
        tt += rlen;
      }
    }
  }

  Bytes *r = bytes_new();
  bytes_add_bytes(r, bs, tt);
  return rs_ok(r);
}

char *tcp_write (TcpConn *conn, Bytes *bs) {
  int rs;
  for (;;) {
    rs = send(conn->id, bytes_bs(bs), bytes_len(bs), 0);
    if (rs == -1 && errno == EINTR) continue;
    break;
  }
  if (rs == -1) {
    close(conn->id);
    return str_f("Fail sending on connection: %s", strerror(errno));
  }
  fsync(conn->id);
  return "";
}

// <TcpConn>
Rs *tcp_dial (char *sv, int port) {
  struct sockaddr_in server;
  int id = socket(AF_INET , SOCK_STREAM , 0);
  if (id == -1) return rs_fail("Could not create socket");
  int oldflags = fcntl (id, F_GETFL, 0);
  if (oldflags == -1) return rs_fail("Could not create socket");
  if (fcntl (id, F_SETFL, oldflags) == -1)
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
