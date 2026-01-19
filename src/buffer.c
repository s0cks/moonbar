#include "moonbar.h"

static inline uint64_t round_up_pow2(uint64_t value) {
  return value;
}

void mbar_buff_init(Buffer* buff, const uint64_t init_cap) {
  buff->data = nullptr;
  buff->length = 0;
  buff->capacity = 0;
  if (init_cap > 0) {
    uint64_t new_cap = round_up_pow2(init_cap);
    uint8_t* new_data = (uint8_t*)malloc(sizeof(uint8_t) * new_cap);
    if (!new_data) {
      fprintf(stderr, "failed to allocate buffer of size %lu\n", new_cap);
      return;
    }

    memset(new_data, 0, sizeof(uint8_t) * new_cap);
    buff->data = new_data;
    buff->capacity = new_cap;
  }
}

void mbar_buff_copy(Buffer* buff, const uint8_t* bytes, const uint64_t num_bytes) {
  ASSERT(buff);
  if (bytes == NULL || num_bytes == 0)
    return;
  uint8_t* new_data = (uint8_t*)malloc(sizeof(uint8_t) * num_bytes);
  if (!new_data) {
    fprintf(stderr, "failed to allocate buffer of size %lu\n", num_bytes);
    return;
  }
  memset(new_data, 0, num_bytes);
  memcpy(new_data, bytes, num_bytes);
  buff->data = new_data;
  buff->length = buff->capacity = num_bytes;
}

void mbar_buff_append(Buffer* buff, const uint8_t* bytes, const uint64_t num_bytes) {
  ASSERT(buff);
  if (buff->length + num_bytes >= buff->capacity) {
    uint64_t new_cap = round_up_pow2(buff->capacity * 2);
    if (new_cap < buff->length + num_bytes)
      new_cap = buff->length + num_bytes;
    uint8_t* new_data = (uint8_t*)realloc(buff->data, new_cap);
    if (!new_data) {
      fprintf(stderr, "failed to grow buffer to new size of: %lu\n", new_cap);
      return;
    }
    buff->data = new_data;
    buff->capacity = new_cap;
  }
  memcpy(&buff->data[buff->length], bytes, num_bytes);
  buff->length += num_bytes;
}

void mbar_buff_free(Buffer* buff) {
  if (!buff || !buff->data)
    return;
  free(buff->data);
}
