#include "uv_gsource.h"
#include "mybar.h"

static inline gboolean
prepare(GSource* source, gint* timeout) {
  ASSERT(source);
  uv_gsource* src = (uv_gsource*)source;
  int uv_timeout = uv_backend_timeout(src->loop);
  (*timeout) = (uv_timeout < 0) ? -1 : uv_timeout;
  return false;
}

static inline gboolean
check(GSource* source) {
  ASSERT(source);
  uv_gsource* src = (uv_gsource*)source;
  return src->poll_fd.revents != 0 || uv_backend_timeout(src->loop) == 0;
}

static inline gboolean
dispatch(GSource* source, GSourceFunc callback, gpointer data) {
  ASSERT(source);
  uv_gsource* src = (uv_gsource*)source;
  uv_run(src->loop, UV_RUN_NOWAIT);
  return G_SOURCE_CONTINUE;
}

static GSourceFuncs kSourceFuncs = {
  prepare,
  check,
  dispatch,
};

uv_gsource* uv_gsource_init(uv_loop_t* loop) {
  ASSERT(loop);
  GSource* source = g_source_new(&kSourceFuncs, sizeof(uv_gsource));
  if(!source) {
    fprintf(stderr, "failed to g_source_new\n");
    exit(1);
    return NULL;
  }

  uv_gsource* src = (uv_gsource*)source;
  src->loop = loop;
  src->poll_fd.fd = uv_backend_fd(loop);
  src->poll_fd.events = G_IO_IN|G_IO_OUT|G_IO_ERR;
  g_source_add_poll(source, &src->poll_fd);
  g_source_attach(source, g_main_context_default());
  // g_source_ref(source);
  return src;
}

void uv_gsource_free(uv_gsource* source) {
  if(!source)
    return;
  g_source_destroy((GSource*)source);
  // g_source_unref((GSource*)source);
}
