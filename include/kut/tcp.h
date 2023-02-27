// Copyright 26-Jan-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// TCP-IP utilities.

#ifndef LKUT_TCP_H
  #define LKUT_TCP_H

#include "rs.h"
#include "bytes.h"

typedef struct tcp_TcpServer TcpServer;

typedef struct tcp_TcpConn TcpConn;

TcpServer *tcp_server (int port);

// <TcpConn>
Rs *tcp_accept (TcpServer *sv);

// <char>
Rs *tcp_read (TcpConn *conn);

// <Bytes>
Rs *tcp_read_bin (TcpConn *conn);

char *tcp_write (TcpConn *conn, char *s);

char *tcp_write_bin (TcpConn *conn, Bytes *bs);

// <TcpConn>
Rs *tcp_dial (char *sv, int port);

void tcp_close_server (TcpServer *conn);

void tcp_close_conn (TcpConn *conn);

#endif
