#include <stdio.h>
#include <string.h>
#include "mongoose.h"

// This is an example on how to use mongoose to implement a proxy server.

static int begin_request_handler(struct mg_connection *conn) {
  const struct mg_request_info *ri2, *ri = mg_get_request_info(conn);
  struct mg_connection *conn2;
  const char *hh, *error_message = NULL;
  char proto[10], host[200], ebuf[100], buf[500];
  int port, n;

  if ((hh = mg_get_header(conn, "Host")) == NULL) {
    error_message = "No Host: header";
  } else if (sscanf(hh, "%9[htps]://%199[^:]:%d", proto, host, &port) != 3 &&
             sscanf(hh, "%9[htps]://%199s", proto, host) != 2) {
    error_message = "Invalid Host: header";
    // TODO(lsm): pass all headers to server properly
  } else if ((conn2 = mg_download(host, port, !strcmp(proto, "https"),
                                  ebuf, sizeof(ebuf), "%s %s HTTP/1.0\r\n\r\n",
                                  ri->request_method, ri->uri)) == NULL) {
    error_message = "Cannot connect to host";
  } else {
    // TODO(lsm): pass all headers to client properly
    ri2 = mg_get_request_info(conn2);
    mg_printf(conn, "%s %s %s\r\n\r\n",
              ri2->request_method, ri2->uri, ri2->http_version);
    while ((n = mg_read(conn2, buf, sizeof(buf))) > 0) {
      mg_write(conn, buf, n);
    }
    mg_close_connection(conn2);
  }

  if (error_message != NULL) {
    mg_printf(conn, "HTTP/1.0 500 Server error\r\n\r\n%s", error_message);
  }

  return 1;
}

int main(void) {
  struct mg_context *ctx;
  struct mg_callbacks callbacks;
  const char *options[] = {"listening_ports", "8080", NULL};

  memset(&callbacks, 0, sizeof(callbacks));
  callbacks.begin_request = begin_request_handler;
  ctx = mg_start(&callbacks, NULL, options);
  getchar();
  mg_stop(ctx);

  return 0;
}
