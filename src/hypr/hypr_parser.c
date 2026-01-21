#include "hypr_parser.h"

#include "hypr_event.h"

typedef struct _TrieNode {
  struct _TrieNode* children[62];
  bool terminal;
  HyprEventType value;
} TrieNode;

static inline int get_index(const char c) {
  if (c >= '0' && c <= '9')
    return c - '0';
  else if (c >= 'A' && c <= 'Z')
    return c - 'A' + 10;
  else if (c >= 'a' && c <= 'z')
    return c - 'a' + 30;
  return -1;
}

static inline TrieNode* new_node() {
  TrieNode* node = (TrieNode*)malloc(sizeof(TrieNode));
  if (node) {
    node->terminal = false;
    node->value = kInvalidHyprEvent;
    memset(node->children, 0, sizeof(node->children));
    for (int i = 0; i < 62; i++) {
      node->children[i] = nullptr;
    }
  }
  return node;
}

static inline void insert_node(TrieNode* root, const char* key, const HyprEventType value) {
  TrieNode* curr = root;
  for (int i = 0; key[i] != '\0'; i++) {
    int index = get_index(key[i]);
    if (index == -1) {
      fprintf(stderr, "invalid index: %d\n", index);
      return;
    }

    if (!curr->children[index])
      curr->children[index] = new_node();

    curr = curr->children[index];
  }
  curr->value = value;
  curr->terminal = true;
}

static TrieNode* kRootNode = nullptr;

static inline void init_root_node() {
  if (kRootNode)
    return;
  kRootNode = new_node();
  if (kRootNode) {
#define CREATE_NODE(Type, Name) insert_node(kRootNode, #Name, kHypr##Type##Event);
    FOR_EACH_HYPR_EVENT(CREATE_NODE)
#undef CREATE_NODE
  }
}

static inline char parser_current(HyprParser* parser) {
  return (char)parser->data[parser->rpos];
}

static inline char parser_peek(HyprParser* parser) {
  const uint64_t next = parser->rpos + 1;
  if (next > parser->wpos)
    return '\0';
  return (char)parser->data[next];
}

static inline char parser_next(HyprParser* parser) {
  const char next = parser_current(parser);
  parser->rpos++;
  return next;
}

static inline bool parser_is_eos(HyprParser* parser) {
  return parser->rpos >= parser->wpos || parser_current(parser) == '\0';
}

static inline bool parse_event(HyprParser* parser, HyprEventType* type, uint64_t* start, uint64_t* end,
                               uint64_t* length) {
  (*start) = parser->rpos;
  char curr = '\0';
  TrieNode* cn = kRootNode;
  do {
    if (hypr_parser_has_error(parser)) {
      fprintf(stderr, "parser has error\n");
      return false;
    } else if (!hypr_parser_is(parser, kParsingEvent)) {
      fprintf(stderr, "invalid state: %s\n", hypr_parser_get_state_name(parser->state));
      hypr_parser_set_error(parser, kInvalidStateError);
      return false;
    } else if (parser_is_eos(parser)) {
      fprintf(stderr, "end of stream\n");
      return false;
    }

    curr = parser_current(parser);
    switch (curr) {
      case '>': {
        if (!cn || !cn->terminal) {
          fprintf(stderr, "invalid state\n");
          return false;
        }
        (*end) = parser->rpos;
        parser_next(parser);
        if (parser_current(parser) != '>') {
          fprintf(stderr, "invalid token\n");
          hypr_parser_set_error(parser, kInvalidTokenError);
          return false;
        }
        parser_next(parser);
        (*type) = cn->value;
        (*length) = (*end) - (*start) + 1;
        return true;
      }
      default: {
        const int index = get_index(curr);
        if (index == -1) {
          fprintf(stderr, "[0] invalid token: %c\n", curr);
          return false;
        }
        cn = cn->children[index];
        parser_next(parser);
        continue;
      }
    }
  } while (true);
}

static inline bool parse_string_data(HyprParser* parser, char** result) {
  char current = '\0';
  uint64_t start = parser->rpos;
  while (true) {
    current = parser_current(parser);
    switch (current) {
      case '\0':
      case '\n':
      case ',': {
        const uint64_t end = parser->rpos - 1;
        const uint64_t length = (end - start) + 1;
        char* data = (char*)malloc(sizeof(char) * length);
        if (data) {
          memset(data, 0, length);
          strncpy(data, (const char*)&parser->data[start], length);
          data[length] = '\0';
          (*result) = data;
        }
        return data && length > 0;
      }
      default:
        parser_next(parser);
        continue;
    }
  }
  return false;
}

static inline bool parse_event_data(HyprParser* parser, HyprEvent* event) {
  switch (event->type) {
    case kHyprWorkspaceEvent: {
      if (!parse_string_data(parser, &event->workspace.workspace))
        return false;
      return true;
    }
    case kHyprWorkspacev2Event: {
      if (!parse_string_data(parser, &event->workspacev2.workspace_id))
        return false;
      if (!parse_string_data(parser, &event->workspacev2.workspace_name))
        return false;
      return true;
    }

    case kHyprFocusedMonitorEvent: {
      if (!parse_string_data(parser, &event->focusedmon.monitor_name))
        return false;
      if (!parse_string_data(parser, &event->focusedmon.workspace_name))
        return false;
      return true;
    }
    case kHyprFocusedMonitorv2Event: {
      if (!parse_string_data(parser, &event->focusedmonv2.monitor_name))
        return false;
      if (!parse_string_data(parser, &event->focusedmonv2.workspace_id))
        return false;
      return true;
    }

    case kHyprActiveWindowEvent: {
      if (!parse_string_data(parser, &event->activewindow.window_class))
        return false;
      fprintf(stdout, "parsed window class: %s\n", event->activewindow.window_class);
      parser_next(parser);  // TODO(@s0cks): check if comma
      if (!parse_string_data(parser, &event->activewindow.window_title))
        return false;
      fprintf(stdout, "parsed window title: %s\n", event->activewindow.window_title);
      return true;
    }
    case kHyprActiveWindowv2Event: {
      if (!parse_string_data(parser, &event->activewindowv2.window_address))
        return false;
      fprintf(stdout, "parsed window address: %s\n", event->activewindowv2.window_address);
      return true;
    }

    case kHyprFullscreenEvent: {
      // TODO(@s0cks): implement
      return false;
    }
    case kHyprMonitorRemovedEvent: {
      if (!parse_string_data(parser, &event->monitorremoved.monitor_name))
        return false;
      return true;
    }
    case kHyprMonitorRemovedv2Event: {
      if (!parse_string_data(parser, &event->monitorremovedv2.monitor_id))
        return false;
      if (!parse_string_data(parser, &event->monitorremovedv2.monitor_name))
        return false;
      if (!parse_string_data(parser, &event->monitorremovedv2.monitor_desc))
        return false;
      return true;
    }

    case kHyprMonitorAddedEvent: {
      if (!parse_string_data(parser, &event->monitoradded.monitor_name))
        return false;
      return true;
    }
    case kHyprMonitorAddedv2Event: {
      if (!parse_string_data(parser, &event->monitoraddedv2.monitor_id))
        return false;
      if (!parse_string_data(parser, &event->monitoraddedv2.monitor_name))
        return false;
      if (!parse_string_data(parser, &event->monitoraddedv2.monitor_desc))
        return false;
      return true;
    }

    case kHyprCreateWorkspaceEvent: {
      if (!parse_string_data(parser, &event->createworkspace.workspace_name))
        return false;
      return true;
    }
    case kHyprCreateWorkspacev2Event: {
      if (!parse_string_data(parser, &event->createworkspacev2.workspace_id))
        return false;
      if (!parse_string_data(parser, &event->createworkspacev2.workspace_name))
        return false;
      return true;
    }

    case kHyprDestroyWorkspaceEvent: {
      if (!parse_string_data(parser, &event->destroyworkspace.workspace_name))
        return false;
      return true;
    }
    case kHyprDestroyWorkspacev2Event: {
      if (!parse_string_data(parser, &event->destroyworkspacev2.workspace_id))
        return false;
      if (!parse_string_data(parser, &event->destroyworkspacev2.workspace_name))
        return false;
      return true;
    }

    case kHyprMoveWorkspaceEvent: {
      if (!parse_string_data(parser, &event->moveworkspace.workspace_name))
        return false;
      if (!parse_string_data(parser, &event->moveworkspace.monitor_name))
        return false;
      return true;
    }
    case kHyprMoveWorkspacev2Event: {
      if (!parse_string_data(parser, &event->moveworkspacev2.workspace_id))
        return false;
      if (!parse_string_data(parser, &event->moveworkspacev2.workspace_name))
        return false;
      if (!parse_string_data(parser, &event->moveworkspacev2.monitor_name))
        return false;
      return true;
    }

    case kHyprRenameWorkspaceEvent: {
      if (!parse_string_data(parser, &event->renameworkspace.workspace_id))
        return false;
      if (!parse_string_data(parser, &event->renameworkspace.new_name))
        return false;
      return true;
    }

    case kHyprActiveSpecialEvent: {
      if (!parse_string_data(parser, &event->activespecial.workspace_name))
        return false;
      if (!parse_string_data(parser, &event->activespecial.monitor_name))
        return false;
      return true;
    }
    case kHyprActiveSpecialv2Event: {
      if (!parse_string_data(parser, &event->activespecialv2.workspace_id))
        return false;
      if (!parse_string_data(parser, &event->activespecialv2.workspace_name))
        return false;
      if (!parse_string_data(parser, &event->activespecialv2.monitor_name))
        return false;
      return true;
    }

    case kHyprActiveLayoutEvent: {
      if (!parse_string_data(parser, &event->activelayout.keyboard_name))
        return false;
      if (!parse_string_data(parser, &event->activelayout.layout_name))
        return false;
      return true;
    }

    case kHyprOpenWindowEvent: {
      if (!parse_string_data(parser, &event->openwindow.window_address))
        return false;
      if (!parse_string_data(parser, &event->openwindow.workspace_name))
        return false;
      if (!parse_string_data(parser, &event->openwindow.window_class))
        return false;
      if (!parse_string_data(parser, &event->openwindow.window_title))
        return false;
      return true;
    }
    case kHyprCloseWindowEvent: {
      if (!parse_string_data(parser, &event->closewindow.window_address))
        return false;
      return true;
    }

    case kHyprMoveWindowEvent: {
      if (!parse_string_data(parser, &event->movewindow.window_address))
        return false;
      if (!parse_string_data(parser, &event->movewindow.workspace_name))
        return false;
      return true;
    }
    case kHyprMoveWindowv2Event: {
      if (!parse_string_data(parser, &event->movewindowv2.window_address))
        return false;
      if (!parse_string_data(parser, &event->movewindowv2.workspace_id))
        return false;
      if (!parse_string_data(parser, &event->movewindowv2.workspace_name))
        return false;
      return true;
    }

    case kHyprOpenLayerEvent: {
      if (!parse_string_data(parser, &event->openlayer.namespace))
        return false;
      return true;
    }
    case kHyprCloseLayerEvent: {
      if (!parse_string_data(parser, &event->closelayer.namespace))
        return false;
      return true;
    }
    case kHyprSubmapEvent: {
      if (!parse_string_data(parser, &event->submap.submapname))
        return false;
      return true;
    }
    case kHyprChangeFloatingModeEvent: {
      if (!parse_string_data(parser, &event->changefloatingmode.window_address))
        return false;
      // TODO(@s0cks): parse state
      return true;
    }
    case kHyprUrgentEvent: {
      if (!parse_string_data(parser, &event->urgent.window_address))
        return false;
      return true;
    }
    case kHyprScreencastEvent: {
      // TODO(@s0cks): parse state
      if (!parse_string_data(parser, &event->screencast.owner))
        return false;
      return true;
    }
    case kHyprWindowTitleEvent: {
      if (!parse_string_data(parser, &event->windowtitle.window_address))
        return false;
      return true;
    }
    case kHyprWindowTitlev2Event: {
      if (!parse_string_data(parser, &event->windowtitlev2.window_address))
        return false;
      if (!parse_string_data(parser, &event->windowtitlev2.window_title))
        return false;
      return true;
    }
    case kHyprToggleGroupEvent: {
      // TODO(@s0cks): parse state
      // TODO(@s0cks): parse window addresses
      return false;
    }
    case kHyprMoveIntoGroupEvent: {
      if (!parse_string_data(parser, &event->moveintogroup.window_address))
        return false;
      return true;
    }
    case kHyprMoveOutOfGroupEvent: {
      if (!parse_string_data(parser, &event->moveoutofgroup.window_address))
        return false;
      return true;
    }
    case kHyprIgnoreGroupLockEvent: {
      // TODO(@s0cks): parse state
      return true;
    }
    case kHyprLockGroupsEvent: {
      // TODO(@s0cks): parse state
      return true;
    }

    case kHyprConfigReloadedEvent:
      return true;

    case kHyprPinEvent: {
      if (!parse_string_data(parser, &event->pin.window_address))
        return false;
      // TODO(@s0cks): parse state
      return true;
    }

    case kHyprMinimizedEvent: {
      if (!parse_string_data(parser, &event->minimized.window_address))
        return false;
      // TODO(@s0cks): parse state
      return true;
    }
    case kHyprBellEvent: {
      if (!parse_string_data(parser, &event->bell.window_address))
        return false;
      return true;
    }

    case kInvalidHyprEvent:
    default:
      fprintf(stderr, "invalid hypr event\n");
      return false;
  }
}

bool hypr_parser_parse(HyprParser* parser) {
  if (!kRootNode) {
    init_root_node();
    if (!kRootNode)
      return false;
  }

  if (parser->on_start) {
    if (!parser->on_start(parser))
      return false;
  }

  fprintf(stdout, "parsing: %s...\n", (const char*)parser->data);

  uint64_t start = 0;
  uint64_t end = 0;
  uint64_t length = 0;
  HyprEvent ev;
  ev.type = kInvalidHyprEvent;
  hypr_parser_set_state(parser, kParsingEvent);
  while (!parser_is_eos(parser) && hypr_parser_is(parser, kParsingEvent)) {
    if (!parse_event(parser, &ev.type, &start, &end, &length))
      break;

    fprintf(stdout, "parsed hypr event (%d): %s\n", ev.type, hypr_event_get_name(ev.type));

    hypr_parser_set_state(parser, kParsingData);
  parse_data:
    if (!parse_event_data(parser, &ev)) {
      fprintf(stderr, "failed to parse hypr event data\n");
      return false;
    }

    if (parser_is_eos(parser)) {
      hypr_parser_set_state(parser, kParsingFinished);
      goto finished;
    }

    fprintf(stdout, "[3] current: %c\n", parser_current(parser));
    switch (parser_current(parser)) {
      case '\n':
        fprintf(stdout, "newline\n");
        parser_next(parser);
        continue;
      case ',': {
        parser_next(parser);
        fprintf(stdout, "parsed data\n");
        if (parser->on_event) {
          if (!parser->on_event(parser, &ev))
            return false;
        }
        goto parse_data;
      }
      case '\0': {
        hypr_parser_set_state(parser, kParsingFinished);
        goto finished;
      }
      default:
        fprintf(stdout, "unexpected token\n");
        hypr_parser_set_error(parser, kInvalidTokenError);
        hypr_parser_set_state(parser, kParsingFinished);
        return false;
    }
  }

finished:
  fprintf(stdout, "state: %s\n", hypr_parser_get_state_name(parser->state));
  if (parser->state != kParsingFinished) {
    fprintf(stderr, "parsing isnt finished\n");
    return false;
  } else if (parser->err != kNoError) {
    fprintf(stderr, "parser error\n");
    return false;
  }

  // TODO: check for linebreak
  if (parser->on_finished) {
    if (!parser->on_finished(parser))
      return false;
  }
  return true;
}
