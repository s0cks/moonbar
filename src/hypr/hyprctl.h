#ifndef HYPRCTL_H
#define HYPRCTL_H

#include <string.h>
#include <uv.h>

typedef struct _Hyprctl {
  int socket;
  const char* path;
} Hyprctl;

void hyprctl_init(uv_loop_t* loop, Hyprctl* ctx);
void hyprctl_send(Hyprctl* ctx, const uint8_t* bytes, const uint64_t num_bytes);
void hyprctl_sendf(Hyprctl* ctx, const char* fmt, ...);
void hyprctl_free(Hyprctl* ctx);

static inline void hyprctl_sends(Hyprctl* ctx, const char* data) {
  return hyprctl_send(ctx, (const uint8_t*)data, strlen(data));
}

void hyprctl_exec(uv_loop_t* loop);
void hyprctl_execr(uv_loop_t* loop);
void hyprctl_pass(uv_loop_t* loop, const char* window);
void hyprctl_sendshortcut(uv_loop_t* loop);
void hyprctl_sendkeystate(uv_loop_t* loop);
void hyprctl_killactive(uv_loop_t* loop);
void hyprctl_forcekillactive(uv_loop_t* loop);
void hyprctl_closewindow(uv_loop_t* loop, const char* window);
void hyprctl_killwindow(uv_loop_t* loop, const char* window);
void hyprctl_signal(uv_loop_t* loop, const char* signal);
void hyprctl_signalwindow(uv_loop_t* loop, const char* window, const char* signal);
void hyprctl_workspace(uv_loop_t* loop, const char* workspace);
void hyprctl_movetoworkspace(uv_loop_t* loop);
void hyprctl_movetoworkspacesilent(uv_loop_t* loop);
void hyprctl_togglefloating(uv_loop_t* loop);
void hyprctl_setfloating(uv_loop_t* loop);
void hyprctl_settiled(uv_loop_t* loop);
void hyprctl_fullscreen(uv_loop_t* loop);
void hyprctl_fullscreenstate(uv_loop_t* loop);
void hyprctl_dpms(uv_loop_t* loop);
void hyprctl_forceidle(uv_loop_t* loop);
void hyprctl_pin(uv_loop_t* loop);
void hyprctl_movefocus(uv_loop_t* loop);
void hyprctl_movewindow(uv_loop_t* loop);
void hyprctl_swapwindow(uv_loop_t* loop);
void hyprctl_centerwindow(uv_loop_t* loop);
void hyprctl_resizeactive(uv_loop_t* loop);
void hyprctl_moveactive(uv_loop_t* loop);
void hyprctl_resizewindowpixel(uv_loop_t* loop);

#endif  // HYPRCTL_H
