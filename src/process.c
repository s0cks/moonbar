#include "moonbar.h"
#include "app.h"
#include "util.h"

static inline void
on_read(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf) {
  Process* proc = (Process*)stream->data;
  if (nread > 0) {
    // Identify which stream sent the data
    if(stream == (uv_stream_t*)&proc->out) {
      mbar_buff_append(proc->out_buffer, (const uint8_t*)buf->base, nread);
    } else if(stream == (uv_stream_t*)&proc->err) {
      mbar_buff_append(proc->err_buffer, (const uint8_t*)buf->base, nread);
    }
  } else if (nread < 0) {
    if (nread != UV_EOF)
      fprintf(stderr, "Read error: %s\n", uv_err_name(nread));
    uv_close((uv_handle_t*)stream, NULL);
  }
  free(buf->base);
}

static inline void
on_alloc(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf) {
  buf->base = malloc(suggested_size);
  buf->len = suggested_size;
}

static inline void
on_process_exit(uv_process_t* handle, int64_t status, int signal) {
  Process* proc = (Process*)handle->data;
  if(status == 0) {
    proc->out_buffer->data[proc->out_buffer->length + 1] = '\0';
    char* result = trim((char*)proc->out_buffer->data);
    lua_pushstring(proc->owner->L, result);
    mbar_proc_pcall_on_success(proc);
  } else {
    proc->err_buffer->data[proc->err_buffer->length + 1] = '\0';
    char* result = trim((char*)proc->err_buffer->data);
    lua_pushstring(proc->owner->L, result);
    mbar_proc_pcall_on_failure(proc);
  }
  mbar_proc_pcall_on_finished(proc);
  // TODO(@s0cks):
  // uv_close((uv_handle_t*)handle, NULL);
}

void mbar_proc_init(BarApp* app, Process* proc, char** argv, const int argc) {
  ASSERT(app);
  ASSERT(app->loop);
  ASSERT(proc);
  ASSERT(argv);
  ASSERT(argc >= 1);
  uv_loop_t* loop = app->loop;
  uv_pipe_init(loop, &proc->out, 0);
  proc->out.data = proc;
  uv_pipe_init(loop, &proc->err, 0);
  proc->handle.data = proc;
  proc->err.data = proc;
  proc->owner = app;
  proc->argc = argc;
  proc->argv = argv;
  proc->out_buffer = (Buffer*)malloc(sizeof(Buffer));
  mbar_buff_init(proc->out_buffer, 4096);
  proc->err_buffer = (Buffer*)malloc(sizeof(Buffer));
  mbar_buff_init(proc->err_buffer, 4096);
  proc->on_success = proc->on_failure = proc->on_finished = LUA_NOREF;
  proc->options.file = argv[0];
  proc->options.args = argv;
  proc->options.exit_cb = on_process_exit;
  proc->options.stdio_count = 3;
  proc->options.stdio = proc->io;
  proc->io[0].flags = UV_IGNORE;
  proc->io[1].flags = UV_CREATE_PIPE|UV_WRITABLE_PIPE;
  proc->io[1].data.stream = (uv_stream_t*)&proc->out;
  proc->io[2].flags = UV_CREATE_PIPE|UV_WRITABLE_PIPE;
  proc->io[2].data.stream = (uv_stream_t*)&proc->err;
}

void mbar_proc_spawn(Process* proc) {
  ASSERT(proc);
  uv_loop_t* loop = proc->owner->loop;
  const int status = uv_spawn(loop, &proc->handle, &proc->options);
  if(status != 0) {
    fprintf(stderr, "failed to spawn process: %s\n", uv_strerror(status));
    exit(1);
    return;
  }
  uv_read_start((uv_stream_t*)&proc->out, on_alloc, on_read);
  uv_read_start((uv_stream_t*)&proc->err, on_alloc, on_read);
}

void mbar_proc_close(Process* proc) {
  ASSERT(proc);
}
