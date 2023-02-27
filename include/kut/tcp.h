// Copyright 26-Jan-2023 ºDeme
// GNU General Public License - V3 <http://www.gnu.org/licenses/>

/// TCP-IP utilities.
///
/// Example:
///
///|  #include "kut/thread.h"
///|  #include "main.h"
///|  #include "kut/DEFS.h"
///|  #include "kut/sys.h"
///|  #include "kut/tcp.h"
///|
///|  static char *help () {
///|    return
///|    "Use:\n"
///|    "tcpTest server\n"
///|    "or\n"
///|    "tcpTest client"
///|    ;
///|  }
///|
///|  static char *handler (char *rq) {
///|    printf("Recibido %s\n", rq);
///|    return "World";
///|  }
///|
///|  static void runServer(void) {
///|    TcpServer *sv = tcp_server(2525);
///|
///|    int end = FALSE;
///|    for(;;) {
///|      Rs *conn_rs = tcp_accept(sv);
///|      if (end) break;
///|      TcpConn *conn = rs_get(conn_rs);
///|      if (!conn) EXC_IO(rs_error(conn_rs));
///|
///|        //--
///|        void fn (void *conn) {
///|          for (;;) {
///|            Rs *rq_rs = tcp_read(conn);
///|            char *rq = rs_get(rq_rs);
///|            if (!rq) {
///|              puts(rs_error(rq_rs));
///|              return;
///|            }
///|
///|            if (!str_eq(rq, "Hello")) {
///|              end = TRUE;
///|              tcp_dial("localhost", 2525);
///|              break;
///|            }
///|
///|            char *rp = handler(rq);
///|            char *err = tcp_write(conn, rp);
///|            if (*err) {
///|              puts(err);
///|              break;
///|            }
///|          }
///|          tcp_close_conn(conn);
///|        }
///|      thread_run2(fn, conn);
///|    }
///|
///|    tcp_close_server(sv);
///|  }
///|
///|  static void runClient(void) {
///|    Rs *conn_rs = tcp_dial("localhost", 2525);
///|    TcpConn *conn = rs_get(conn_rs);
///|    if (!conn) {
///|      puts(rs_error(conn_rs));
///|      return;
///|    }
///|
///|    REPEAT(2) {
///|      tcp_write(conn, "Hello");
///|
///|      Rs *rp_rs = tcp_read(conn);
///|      char *rp = rs_get(rp_rs);
///|      if (!rp) {
///|        puts(rs_error(conn_rs));
///|        return;
///|      }
///|      puts(str_f("Recibido: %s", rp));
///|    }_REPEAT
///|
///|    tcp_write(conn, "End");
///|    tcp_close_conn(conn);
///|  }
///|
///|  int main(int argc, char *argv[]) {
///|    sys_init();
///|
///|    if (argc != 2) {
///|      puts(help());
///|      return 0;
///|    }
///|
///|    if (str_eq(argv[1], "server")) runServer();
///|    else if (str_eq(argv[1], "client")) runClient();
///|    else {
///|      puts(help());
///|      return 0;
///|    }
///|    puts("Program end.");
///|  }

#ifndef LKUT_TCP_H
  #define LKUT_TCP_H

#include "rs.h"
#include "bytes.h"

/// Server structure.
typedef struct tcp_TcpServer TcpServer;

/// Connection structure.
typedef struct tcp_TcpConn TcpConn;

/// Returns a server connected to 'port'.
/// Throw EXC_IO.
TcpServer *tcp_server (int port);

/// Returns a Rs<TcpConn>. A client connection.
Rs *tcp_accept (TcpServer *sv);

/// Read a client connection in a Rs<char>.
Rs *tcp_read (TcpConn *conn);

/// Read a client connection in a Rs<Bytes>.
Rs *tcp_read_bin (TcpConn *conn);

/// Write a text in 'conn', returning "" if there no was any error, or an
/// error message oterwise.
char *tcp_write (TcpConn *conn, char *s);

/// Write 'bs' in 'conn', returning "" if there no was any error, or an
/// error message oterwise.
char *tcp_write_bin (TcpConn *conn, Bytes *bs);

/// Returns a Rs<TcpConn> with a client connection to a server 'sv' in the
/// port 'port'.
Rs *tcp_dial (char *sv, int port);

/// Close the server 'sv'
void tcp_close_server (TcpServer *sv);

/// Close the connection "conn".
void tcp_close_conn (TcpConn *conn);

#endif
