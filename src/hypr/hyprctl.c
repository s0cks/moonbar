#include "hyprctl.h"

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#ifdef _NDEBUG

#include <assert.h>
#define ASSERT(x) assert((x))

#else

#define ASSERT(x)

#endif

void hyprctl_init(uv_loop_t* loop, Hyprctl* ctx) {
  memset(ctx, 0, sizeof(Hyprctl));
  ctx->path = nullptr;  // TODO(@s0cks): get hypr socket path
  ctx->socket = socket(AF_UNIX, SOCK_STREAM, 0);
  if (ctx->socket == -1) {
    fprintf(stderr, "failed to open hypr socket\n");
    exit(EXIT_FAILURE);
  }
}

static inline bool get_hypr_instance_path(char* result, const int result_len) {
  const char* his = getenv("HYPRLAND_INSTANCE_SIGNATURE");
  const char* xdg_runtime_dir = getenv("XDG_RUNTIME_DIR");
  if (!his || !xdg_runtime_dir)
    return false;
  memset(result, 0, result_len);
  snprintf(result, result_len, "%s/hypr/%s", xdg_runtime_dir, his);
  return true;
}

void hyprctl_send(Hyprctl* ctx, const uint8_t* bytes, const uint64_t num_bytes) {
  if (!ctx) {
    fprintf(stderr, "context is null\n");
    return;
  }
  ASSERT(bytes);
  ASSERT(num_bytes > 0);

  char sock_dir[PATH_MAX];
  if (!get_hypr_instance_path(sock_dir, PATH_MAX)) {
    fprintf(stderr, "failed to get hypr socket dir\n");
    exit(EXIT_FAILURE);
    return;
  }

  char sock_path[PATH_MAX];
  memset(sock_path, 0, sizeof(sock_path));
  snprintf(sock_path, sizeof(sock_path), "%s/%s", sock_dir, ".socket.sock");

  struct sockaddr_un addr;
  memset(&addr, 0, sizeof(addr));
  addr.sun_family = AF_UNIX;
  strncpy(addr.sun_path, sock_path, sizeof(addr.sun_path) - 1);

  fprintf(stdout, "connecting to %s ...\n", addr.sun_path);
  {
    const int status = connect(ctx->socket, (struct sockaddr*)&addr, sizeof(addr));
    if (status == -1) {
      fprintf(stderr, "connect failed\n");
      close(ctx->socket);
      exit(EXIT_FAILURE);
      return;
    }
    fprintf(stdout, "connected!: %d\n", status);
  }

  fprintf(stdout, "writing %s ...\n", (const char*)bytes);
  const ssize_t num_written = send(ctx->socket, bytes, num_bytes, 0);
  fprintf(stdout, "num written: %lu\n", num_written);
  if (num_written == -1) {
    fprintf(stderr, "failed to write bytes to socket\n");
    close(ctx->socket);
    exit(EXIT_FAILURE);
    return;
  }

  char buff[8192];
  size_t num_read = 0;
  do {
    num_read = read(ctx->socket, buff, 8192);
    if (num_read < 0) {
      fprintf(stderr, "failed to read from hypr ipc socket\n");
      close(ctx->socket);
      exit(EXIT_FAILURE);
      return;
    }

    fprintf(stdout, "read: %s\n", buff);
  } while (num_read > 0);

  fprintf(stdout, "finished\n");
  close(ctx->socket);
}

void hyprctl_sendf(Hyprctl* ctx, const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  char command[8192];
  snprintf(command, 8192, fmt, args);
  va_end(args);
  return hyprctl_sends(ctx, command);
}

void hyprctl_free(Hyprctl* ctx) {
  if (!ctx)
    return;
}

void hyprctl_killactive(uv_loop_t* loop) {
  Hyprctl hypr;
  hyprctl_init(loop, &hypr);
  hyprctl_sends(&hypr, "killactive");
}

void hyprctl_forcekillactive(uv_loop_t* loop) {
  Hyprctl hypr;
  hyprctl_init(loop, &hypr);
  hyprctl_sends(&hypr, "forcekillactive");
}

void hyprctl_closewindow(uv_loop_t* loop, const char* window) {
  Hyprctl hypr;
  hyprctl_init(loop, &hypr);
  hyprctl_sendf(&hypr, "closewindow %s", window);
}

void hyprctl_killwindow(uv_loop_t* loop, const char* window);
void hyprctl_signal(uv_loop_t* loop, const char* signal);
void hyprctl_signalwindow(uv_loop_t* loop, const char* window, const char* signal);
