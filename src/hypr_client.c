#include "hypr_client.h"

#include "app.h"
#include "hypr/hypr_parser.h"

static inline bool get_hypr_instance_path(char* result, const int result_len) {
  const char* his = getenv("HYPRLAND_INSTANCE_SIGNATURE");
  const char* xdg_runtime_dir = getenv("XDG_RUNTIME_DIR");
  if (!his || !xdg_runtime_dir)
    return false;
  memset(result, 0, result_len);
  snprintf(result, result_len, "%s/hypr/%s", xdg_runtime_dir, his);
  return true;
}

static inline void alloc_buffer(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf) {
  buf->base = malloc(suggested_size);
  buf->len = suggested_size;
}

static inline bool on_parse_start(HyprParser* parser) {
  ASSERT(parser);
  return true;
}

static inline bool on_parse_finished(HyprParser* parser) {
  ASSERT(parser);
  return true;
}

static inline bool on_event(HyprParser* parser, HyprEvent* event) {
  ASSERT(parser);
  return true;
}

static inline bool on_data(HyprParser* parser, const char* data, const uint64_t data_len) {
  ASSERT(parser);
  return true;
}

static inline void on_read(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf) {
  if (nread > 0) {
    HyprParser parser;
    hypr_parser_init(&parser, (const uint8_t*)buf->base, nread - 1, on_parse_start, on_event, on_parse_finished);
    hypr_parser_parse(&parser);
    hypr_parser_free(&parser);
  } else if (nread < 0) {
    if (nread != UV_EOF)
      fprintf(stderr, "failed to read from hyprland ipc socket\n");
    uv_close((uv_handle_t*)stream, NULL);
  }
  free(buf->base);
}

static inline void on_connect(uv_connect_t* req, int status) {
  ASSERT(req);
  HyprClient* client = (HyprClient*)req->data;
  ASSERT(client);
  if (status < 0) {
    mbar_error(client->owner, "failed to connect to hypr ipc socket");
    return;
  }

  DLOG("connected!");
  uv_read_start(req->handle, alloc_buffer, on_read);
}

static inline void connect_to_hypr_socket(HyprClient* client, uv_pipe_t* pipe, const char* socket) {
  ASSERT(client);
  ASSERT(socket);
  char sock_path[PATH_MAX];
  memset(sock_path, 0, sizeof(sock_path));
  snprintf(sock_path, sizeof(sock_path), "%s/%s", client->instance_path, socket);
  DLOG_F("connecting to %s\n", sock_path);
  uv_pipe_init(client->owner->loop, pipe, 0);
  uv_connect_t* conn = (uv_connect_t*)malloc(sizeof(uv_connect_t));
  conn->data = client;
  uv_pipe_connect(conn, pipe, sock_path, on_connect);
}

void mbar_hypr_init(HyprClient* client, BarApp* app) {
  ASSERT(client);
  ASSERT(app);
  if (!get_hypr_instance_path(client->instance_path, PATH_MAX))
    mbar_error(app, "failed to get hypr instance path");
  client->owner = app;
  connect_to_hypr_socket(client, &client->event_pipe, ".socket2.sock");
}

void mbar_hypr_close(HyprClient* client) {
  if (!client)
    return;
  // TODO(@s0cks): implement
}
