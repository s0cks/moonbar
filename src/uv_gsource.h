#ifndef BAR_GUVSRC_H
#define BAR_GUVSRC_H

#include <uv.h>
#include <gtk/gtk.h>

typedef struct _uv_gsource{
  GSource source;
  GPollFD poll_fd;
  uv_loop_t* loop;
} uv_gsource;

uv_gsource* uv_gsource_init(uv_loop_t* loop);
void uv_gsource_free(uv_gsource* source);

#endif // BAR_GUVSRC_H
