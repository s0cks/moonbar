#include "hypr_parser.h"

#include "hypr_event.h"

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
  return parser->rpos >= parser->wpos || parser_peek(parser) == '\0';
}

static inline bool parse_event(HyprParser* parser, uint64_t* start, uint64_t* end, uint64_t* length) {
  (*start) = parser->rpos;
  char curr = '\0';
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

    curr = parser_peek(parser);
    // fprintf(stdout, "[%s] current: %c (%d)\n", hypr_parser_get_state_name(parser->state), curr, (int)curr);
    switch (curr) {
      case '>': {
        (*end) = parser->rpos;
        parser_next(parser);
        if (parser_peek(parser) != '>') {
          fprintf(stderr, "invalid token\n");
          hypr_parser_set_error(parser, kInvalidTokenError);
          return false;
        }
        parser_next(parser);
        (*length) = (*end) - (*start) + 1;
        return true;
      }
      default:
        parser_next(parser);
        continue;
    }
  } while (true);
}

static inline bool parse_event_data(HyprParser* parser, uint64_t* start, uint64_t* end, uint64_t* length) {
  (*start) = parser->rpos + 1;
  char curr = '\0';
  do {
    if (hypr_parser_has_error(parser)) {
      fprintf(stderr, "parser has error\n");
      return false;
    }

    curr = parser_peek(parser);
    // fprintf(stdout, "[%s] current: %c (%d)\n", hypr_parser_get_state_name(parser->state), curr, (int)curr);
    switch (curr) {
      case '\n':
      case '\0':
      case ',': {
        (*end) = parser->rpos;
        (*length) = ((*end) - (*start)) + 1;
        return true;
      }
      default:
        parser_next(parser);
        continue;
    }
  } while (true);
}

bool hypr_parser_parse(HyprParser* parser) {
  if (parser->on_start) {
    if (!parser->on_start(parser))
      return false;
  }

  uint64_t start = 0;
  uint64_t end = 0;
  uint64_t length = 0;
  HyprEvent ev;
  ev.type = kInvalidHyprEvent;
  hypr_parser_set_state(parser, kParsingEvent);
  while (!parser_is_eos(parser) && hypr_parser_is(parser, kParsingEvent)) {
    if (!parse_event(parser, &start, &end, &length))
      break;

    char event[length];
    memcpy(&event[0], &parser->data[start], length);
    event[length] = '\0';

#define DEFINE_CHECK_TYPE(Type, Name)            \
  if (strncmp(event, #Name, strlen(#Name)) == 0) \
    ev.type = kHypr##Type##Event;
    FOR_EACH_HYPR_EVENT(DEFINE_CHECK_TYPE)
#undef DEFINE_CHECK_TYPE

    fprintf(stdout, "parsed hypr event (%d): %s\n", ev.type, hypr_event_get_name(ev.type));

    hypr_parser_set_state(parser, kParsingData);
  parse_data:
    if (!parse_event_data(parser, &start, &end, &length)) {
      fprintf(stderr, "failed to parse hypr event data\n");
      return false;
    }

    char data[length];
    memcpy(&data[0], &parser->data[start], length);
    data[length] = '\0';
    if (parser_is_eos(parser)) {
      hypr_parser_set_state(parser, kParsingFinished);
      goto finished;
    }

    switch (parser_peek(parser)) {
      case '\n':
        parser_next(parser);
        continue;
      case ',': {
        parser_next(parser);
        if (parser->on_event) {
          if (!parser->on_event(parser, &event))
            return false;
        }
        goto parse_data;
      }
      case '\0': {
        hypr_parser_set_state(parser, kParsingFinished);
        goto finished;
      }
      default:
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
  }

  // TODO: check for linebreak
  if (parser->on_finished) {
    if (!parser->on_finished(parser))
      return false;
  }
  return true;
}
