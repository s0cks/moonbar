#ifndef HYPR_EVENT_H
#define HYPR_EVENT_H

#include <stdint.h>

#define FOR_EACH_HYPR_EVENT(V)              \
  V(Workspace, workspace)                   \
  V(Workspacev2, workspacev2)               \
  V(FocusedMonitor, focusedmon)             \
  V(FocusedMonitorv2, focusedmonv2)         \
  V(ActiveWindow, activewindow)             \
  V(ActiveWindowv2, activewindowv2)         \
  V(Fullscreen, fullscreen)                 \
  V(MonitorRemoved, monitorremoved)         \
  V(MonitorRemovedv2, monitorremovedv2)     \
  V(MonitorAdded, monitoradded)             \
  V(MonitorAddedv2, monitoraddedv2)         \
  V(CreateWorkspace, createworkspace)       \
  V(CreateWorkspacev2, createworkspacev2)   \
  V(DestroyWorkspace, destroyworkspace)     \
  V(DestroyWorkspacev2, destroyworkspacev2) \
  V(MoveWorkspace, moveworkspace)           \
  V(MoveWorkspacev2, moveworkspacev2)       \
  V(RenameWorkspace, renameworkspace)       \
  V(ActiveSpecial, activespecial)           \
  V(ActiveSpecialv2, activespecialv2)       \
  V(ActiveLayout, activelayout)             \
  V(OpenWindow, openwindow)                 \
  V(CloseWindow, closewindow)               \
  V(MoveWindow, movewindow)                 \
  V(MoveWindowv2, movewindowv2)             \
  V(OpenLayer, openlayer)                   \
  V(CloseLayer, closelayer)                 \
  V(Submap, submap)                         \
  V(ChangeFloatingMode, changefloatingmode) \
  V(Urgent, urgent)                         \
  V(Screencast, screencast)                 \
  V(WindowTitle, windowtitle)               \
  V(WindowTitlev2, windowtitlev2)           \
  V(ToggleGroup, togglegroup)               \
  V(MoveIntoGroup, moveintogroup)           \
  V(MoveOutOfGroup, moveoutofgroup)         \
  V(IgnoreGroupLock, ignoregrouplock)       \
  V(LockGroups, lockgroups)                 \
  V(ConfigReloaded, configreloaded)         \
  V(Pin, pin)                               \
  V(Minimized, minimized)                   \
  V(Bell, bell)

#define DECLARE_EVENT(N, E) typedef struct _##N##Event N##Event;
FOR_EACH_HYPR_EVENT(DECLARE_EVENT)
#undef DECLARE_EVENT

typedef enum : uint8_t {
  kInvalidHyprEvent = 0,
#define DEFINE_EVENT_TYPE(N, E) kHypr##N##Event,
  FOR_EACH_HYPR_EVENT(DEFINE_EVENT_TYPE)
#undef DEFINE_EVENT_TYPE
      kTotalNumberOfHyprEvents,
} HyprEventType;

void hypr_event_workspace_init(WorkspaceEvent* event, const char* workspace);
void hypr_event_workspacev2_init(Workspacev2Event* event, const char* workspace_id, const char* workspace_name);
void hypr_event_focusedmon_init(FocusedMonitorEvent* event, const char* monitor_name, const char* workspace_name);
void hypr_event_focusedmonv2_init(FocusedMonitorv2Event* event, const char* monitor_name, const char* workspace_id);
void hypr_event_activewindow_init(ActiveWindowEvent* event, const char* window_class, const char* window_title);
void hypr_event_activewindowv2_init(ActiveWindowv2Event* event, const char* window_address);
void hypr_event_fullscreen_init(FullscreenEvent* event, const bool state);
void hypr_event_monitorremoved_init(MonitorRemovedEvent* event, const char* monitor_name);
void hypr_event_monitorremovedv2_init(MonitorRemovedv2Event* event, const char* monitor_id, const char* monitor_name,
                                      const char* monitor_desc);
void hypr_event_monitoradded_init(MonitorAddedEvent* event, const char* monitor_name);
void hypr_event_monitoraddedv2_init(MonitorAddedv2Event* event, const char* monitor_id, const char* monitor_name,
                                    const char* monitor_desc);
void hypr_event_createworkspace_init(CreateWorkspaceEvent* event, const char* workspace_name);
void hypr_event_createworkspacev2_init(CreateWorkspacev2Event* event, const char* workspace_id,
                                       const char* workspace_name);
void hypr_event_destroyworkspace_init(DestroyWorkspaceEvent* event, const char* workspace_name);
void hypr_event_destroyworkspacev2_init(DestroyWorkspacev2Event* event, const char* workspace_id,
                                        const char* workspace_name);
void hypr_event_moveworkspace_init(MoveWorkspaceEvent* event, const char* workspace_name, const char* monitor_name);
void hypr_event_moveworkspacev2_init(MoveWorkspacev2Event* event, const char* workspace_id, const char* workspace_name,
                                     const char* monitor_name);
void hypr_event_renameworkspace_init(RenameWorkspaceEvent* event, const char* workspace_id, const char* new_name);
void hypr_event_activespecial_init(ActiveSpecialEvent* event, const char* workspace_name, const char* monitor_name);
void hypr_event_activespecialv2_init(ActiveSpecialv2Event* event, const char* workspace_id, const char* workspace_name,
                                     const char* monitor_name);
void hypr_event_activelayout_init(ActiveLayoutEvent* event, const char* keyboard_name, const char* layout_name);
void hypr_event_openwindow_init(OpenWindowEvent* event, const char* window_address, const char* workspace_name,
                                const char* window_class, const char* window_title);
void hypr_event_closewindow_init(CloseWindowEvent* event, const char* window_address);
void hypr_event_movewindow_init(MoveWindowEvent* event, const char* window_address, const char* workspace_name);
void hypr_event_movewindowv2_init(MoveWindowv2Event* event, const char* window_address, const char* workspace_id,
                                  const char* workspace_name);
void hypr_event_openlayer_init(OpenLayerEvent* event, const char* namespace);
void hypr_event_closelayer_init(CloseLayerEvent* event, const char* namespace);
void hypr_event_submap_init(SubmapEvent* event, const char* submapname);
void hypr_event_changefloatingmode_init(ChangeFloatingModeEvent* event, const char* window_address, const bool state);
void hypr_event_urgent_init(UrgentEvent* event, const char* window_address);
void hypr_event_screencast_init(ScreencastEvent* event, const bool state, const char* owner);
void hypr_event_windowtitle_init(WindowTitleEvent* event, const char* window_address);
void hypr_event_windowtitlev2_init(WindowTitlev2Event* event, const char* window_address, const char* window_title);
void hypr_event_togglegroup_init(ToggleGroupEvent* event, const bool state, const char** window_address,
                                 const uint64_t num_window_addresses);
void hypr_event_moveintogroup_init(MoveIntoGroupEvent* event, const char* window_address);
void hypr_event_moveoutofgroup_init(MoveOutOfGroupEvent* event, const char* window_address);
void hypr_event_ignoregrouplock_init(IgnoreGroupLockEvent* event, const bool state);
void hypr_event_lockgroups_init(LockGroupsEvent* event, const bool state);
void hypr_event_configreloaded_init(ConfigReloadedEvent* event);
void hypr_event_pin_init(PinEvent* event, const char* window_address, const bool state);
void hypr_event_minimized_init(MinimizedEvent* event, const char* window_address);
void hypr_event_bell_init(BellEvent* event);

void hypr_event_workspace_free(WorkspaceEvent* event);
void hypr_event_workspacev2_free(Workspacev2Event* event);
void hypr_event_focusedmon_free(FocusedMonitorEvent* event);
void hypr_event_focusedmonv2_free(FocusedMonitorv2Event* event);
void hypr_event_activewindow_free(ActiveWindowEvent* event);
void hypr_event_activewindowv2_free(ActiveWindowv2Event* event);
void hypr_event_fullscreen_free(FullscreenEvent* event);
void hypr_event_monitorremoved_free(MonitorRemovedEvent* event);
void hypr_event_monitorremovedv2_free(MonitorRemovedv2Event* event);
void hypr_event_monitoradded_free(MonitorAddedEvent* event);
void hypr_event_monitoraddedv2_free(MonitorAddedv2Event* event);
void hypr_event_createworkspace_free(CreateWorkspaceEvent* event);
void hypr_event_createworkspacev2_free(CreateWorkspacev2Event* event);
void hypr_event_destroyworkspace_free(DestroyWorkspaceEvent* event);
void hypr_event_destroyworkspacev2_free(DestroyWorkspacev2Event* event);
void hypr_event_moveworkspace_free(MoveWorkspaceEvent* event);
void hypr_event_moveworkspacev2_free(MoveWorkspacev2Event* event);
void hypr_event_renameworkspace_free(RenameWorkspaceEvent* event);
void hypr_event_activespecial_free(ActiveSpecialEvent* event);
void hypr_event_activespecialv2_free(ActiveSpecialv2Event* event);
void hypr_event_activelayout_free(ActiveLayoutEvent* event);
void hypr_event_openwindow_free(OpenWindowEvent* event);
void hypr_event_closewindow_free(CloseWindowEvent* event);
void hypr_event_movewindow_free(MoveWindowEvent* event);
void hypr_event_movewindowv2_free(MoveWindowv2Event* event);
void hypr_event_openlayer_free(OpenLayerEvent* event);
void hypr_event_closelayer_free(CloseLayerEvent* event);
void hypr_event_submap_free(SubmapEvent* event);
void hypr_event_changefloatingmode_free(ChangeFloatingModeEvent* event);
void hypr_event_urgent_free(UrgentEvent* event);
void hypr_event_screencast_free(ScreencastEvent* event);
void hypr_event_windowtitle_free(WindowTitleEvent* event);
void hypr_event_windowtitlev2_free(WindowTitlev2Event* event);
void hypr_event_togglegroup_free(ToggleGroupEvent* event);
void hypr_event_moveintogroup_free(MoveIntoGroupEvent* event);
void hypr_event_moveoutofgroup_free(MoveOutOfGroupEvent* event);
void hypr_event_ignoregrouplock_free(IgnoreGroupLockEvent* event);
void hypr_event_lockgroups_free(LockGroupsEvent* event);
void hypr_event_configreloaded_free(ConfigReloadedEvent* event);
void hypr_event_pin_free(PinEvent* event);
void hypr_event_minimized_free(MinimizedEvent* event);
void hypr_event_bell_free(BellEvent* event);

#define DEFINE_EVENT(N, E) struct _##N##Event

DEFINE_EVENT(Workspace, workspace) {
  char* workspace;
};

DEFINE_EVENT(Workspacev2, workspacev2) {
  char* workspace_id;
  char* workspace_name;
};

DEFINE_EVENT(FocusedMonitor, focusedmonitor) {
  char* monitor_name;
  char* workspace_name;
};

DEFINE_EVENT(FocusedMonitorv2, focusedmonitorv2) {
  char* monitor_name;
  char* workspace_id;
};

DEFINE_EVENT(ActiveWindow, activewindow) {
  char* window_class;
  char* window_title;
};

DEFINE_EVENT(ActiveWindowv2, activewindowv2) {
  char* window_address;
};

DEFINE_EVENT(Fullscreen, fullscreen) {
  bool state;
};

DEFINE_EVENT(MonitorRemoved, monitorremoved) {
  char* monitor_name;
};

DEFINE_EVENT(MonitorRemovedv2, monitorremovedv2) {
  char* monitor_id;
  char* monitor_name;
  char* monitor_desc;
};

DEFINE_EVENT(MonitorAdded, monitoradded) {
  char* monitor_name;
};

DEFINE_EVENT(MonitorAddedv2, monitoraddedv2) {
  char* monitor_id;
  char* monitor_name;
  char* monitor_desc;
};

DEFINE_EVENT(CreateWorkspace, createworkspace) {
  char* workspace_name;
};

DEFINE_EVENT(CreateWorkspacev2, createworkspacev2) {
  char* workspace_id;
  char* workspace_name;
};

DEFINE_EVENT(DestroyWorkspace, destroyworkspace) {
  char* workspace_name;
};

DEFINE_EVENT(ConfigReloaded, configreloaded){
    // no fields
};

DEFINE_EVENT(DestroyWorkspacev2, destroyworkspacev2) {
  char* workspace_id;
  char* workspace_name;
};

DEFINE_EVENT(MoveWorkspace, moveworkspace) {
  char* workspace_name;
  char* monitor_name;
};

DEFINE_EVENT(MoveWorkspacev2, moveworkspacev2) {
  char* workspace_id;
  char* workspace_name;
  char* monitor_name;
};

DEFINE_EVENT(RenameWorkspace, renameworkspace) {
  char* workspace_id;
  char* new_name;
};

DEFINE_EVENT(ActiveSpecial, activespecial) {
  char* workspace_name;
  char* monitor_name;
};

DEFINE_EVENT(ActiveSpecialv2, activespecialv2) {
  char* workspace_id;
  char* workspace_name;
  char* monitor_name;
};

DEFINE_EVENT(ActiveLayout, activelayout) {
  char* keyboard_name;
  char* layout_name;
};

DEFINE_EVENT(OpenWindow, openwindow) {
  char* window_address;
  char* workspace_name;
  char* window_class;
  char* window_title;
};

DEFINE_EVENT(CloseWindow, closewindow) {
  char* window_address;
};

DEFINE_EVENT(MoveWindow, movewindow) {
  char* window_address;
  char* workspace_name;
};

DEFINE_EVENT(MoveWindowv2, movewindowv2) {
  char* window_address;
  char* workspace_id;
  char* workspace_name;
};

DEFINE_EVENT(OpenLayer, openlayer) {
  char* namespace;
};

DEFINE_EVENT(CloseLayer, closelayer) {
  char* namespace;
};

DEFINE_EVENT(Submap, submap) {
  char* submapname;
};

DEFINE_EVENT(ChangeFloatingMode, changefloatingmode) {
  char* window_address;
  bool state;
};

DEFINE_EVENT(Urgent, urgent) {
  char* window_address;
};

DEFINE_EVENT(Screencast, screencast) {
  bool state;
  char* owner;
};

DEFINE_EVENT(WindowTitle, windowtitle) {
  char* window_address;
};

DEFINE_EVENT(WindowTitlev2, windowtitlev2) {
  char* window_address;
  char* window_title;
};

DEFINE_EVENT(ToggleGroup, togglegroup) {
  bool state;
  char* window_addresses;
  int num_window_addresses;
};

DEFINE_EVENT(MoveIntoGroup, moveintogroup) {
  char* window_address;
};

DEFINE_EVENT(MoveOutOfGroup, moveoutofgroup) {
  char* window_address;
};

DEFINE_EVENT(IgnoreGroupLock, ignoregrouplock) {
  bool state;
};

DEFINE_EVENT(LockGroups, lockgroups) {
  bool state;
};

DEFINE_EVENT(ConfigreLoaded, configreloaded){
    // no fields
};

DEFINE_EVENT(Pin, Pin) {
  char* window_address;
  bool state;
};

DEFINE_EVENT(Minimized, minimized) {
  char* window_address;
  bool state;
};

DEFINE_EVENT(Bell, bell) {
  char* window_address;
};
#undef DEFINE_EVENT

static inline const char* hypr_event_get_name(const HyprEventType rhs) {
  switch (rhs) {
#define DEFINE_TO_STRING(Type, Name) \
  case kHypr##Type##Event:           \
    return #Type;
    FOR_EACH_HYPR_EVENT(DEFINE_TO_STRING)
#undef DEFINE_TO_STRING

    case kInvalidHyprEvent:
    default:
      return "InvalidHyprEvent";
  }
}

typedef struct _HyprEvent {
  HyprEventType type;
  union {
#define EVENT_TYPE(N, E) N##Event E;
    FOR_EACH_HYPR_EVENT(EVENT_TYPE)
#undef EVENT_TYPE
  };
} HyprEvent;

void hypr_event_print(HyprEvent* event);

#endif  // HYPR_EVENT_H
