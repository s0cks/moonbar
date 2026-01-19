#ifndef HYPR_PARSER_H
#define HYPR_PARSER_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hypr_event.h"

#define FOR_EACH_HYPR_PARSER_STATE(V) \
  V(ParsingStart)                     \
  V(ParsingEvent)                     \
  V(ParsingData)                      \
  V(ParsingFinished)

typedef enum _HyprParserState {
#define DEFINE_STATE(Name) k##Name,
  FOR_EACH_HYPR_PARSER_STATE(DEFINE_STATE)
#undef DEFINE_STATE
} HyprParserState;

static inline const char* hypr_parser_get_state_name(HyprParserState s) {
  switch (s) {
#define DEFINE_GET_NAME(Name) \
  case k##Name:               \
    return #Name;
    FOR_EACH_HYPR_PARSER_STATE(DEFINE_GET_NAME)
#undef DEFINE_GET_NAME
    default:
      return "invalid state.";
  }
}

#define FOR_EACH_HYPR_PARSER_ERROR(V) \
  V(InvalidState)                     \
  V(UnexpectedEndOfInput)             \
  V(InvalidToken)

typedef enum _HyprParserError {
  kNoError = 0,
#define DEFINE_ERROR(Name) k##Name##Error,
  FOR_EACH_HYPR_PARSER_ERROR(DEFINE_ERROR)
#undef DEFINE_ERROR
      kTotalNumberOfErrors,
} HyprParserError;

static inline const char* hypr_parser_get_error_name(const HyprParserError e) {
  switch (e) {
    case kNoError:
      return "no error.";
#define DEFINE_GET_NAME(Name) \
  case k##Name##Error:        \
    return #Name;
      FOR_EACH_HYPR_PARSER_ERROR(DEFINE_GET_NAME)
#undef DEFINE_GET_NAME
    default:
      return "invalid error.";
  }
}

typedef struct _HyprParser HyprParser;
typedef bool (*HyprParserStartCallback)(HyprParser* parser);
typedef bool (*HyprParserParseEventCallback)(HyprParser* parser, HyprEvent* event);
typedef bool (*HyprParserFinishedCallback)(HyprParser* parser);

struct _HyprParser {
  HyprParserState state;
  HyprParserError err;
  uint8_t* data;
  uint64_t rpos;
  uint64_t wpos;
  HyprParserStartCallback on_start;
  HyprParserParseEventCallback on_event;
  HyprParserFinishedCallback on_finished;
};

static inline void hypr_parser_set_state(HyprParser* parser, const HyprParserState state) {
  parser->state = state;
}

static inline bool hypr_parser_get_state(HyprParser* parser) {
  return parser->state;
}

static inline bool hypr_parser_is(HyprParser* parser, const HyprParserState state) {
  return hypr_parser_get_state(parser) == state;
}

static inline void hypr_parser_set_error(HyprParser* parser, const HyprParserError err) {
  parser->err = err;
}

static inline bool hypr_parser_has_error(HyprParser* parser) {
  return parser->err != kNoError;
}

static inline void hypr_parser_init(HyprParser* parser, const uint8_t* bytes, const uint64_t num_bytes,
                                    HyprParserStartCallback on_start, HyprParserParseEventCallback on_event,
                                    HyprParserFinishedCallback on_finished) {
  fprintf(stdout, "creating hypr parser\n");
  parser->state = kParsingFinished;
  parser->err = kNoError;
  parser->on_start = on_start;
  parser->on_event = on_event;
  parser->on_finished = on_finished;
  if (bytes && num_bytes > 0) {
    const uint64_t total_bytes = sizeof(uint8_t) * num_bytes;
    uint8_t* new_data = (uint8_t*)malloc(total_bytes);
    if (!new_data) {
      fprintf(stderr, "failed to allocate memory for parser data\n");
      return;
    }

    memset(new_data, 0, total_bytes);
    memcpy(new_data, bytes, num_bytes);
    parser->data = new_data;
    parser->rpos = 0;
    parser->wpos = num_bytes;
    parser->state = kParsingStart;
  }
  fprintf(stdout, "finished creating parser\n");
}

bool hypr_parser_parse(HyprParser* parser);

static inline void hypr_parser_free(HyprParser* parser) {
  if (parser->data)
    free(parser->data);
}

#endif  // HYPR_PARSER_H
