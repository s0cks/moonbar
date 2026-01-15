typedef struct _Buffer Buffer;
struct _Buffer {
  uint8_t* data;
  uint64_t length;
  uint64_t capacity;
};

void mbar_buff_init(Buffer* buff, const uint64_t init_cap);
void mbar_buff_copy(Buffer* buff, const uint8_t* bytes, const uint64_t num_bytes);
void mbar_buff_append(Buffer* buff, const uint8_t* bytes, const uint64_t num_bytes);
void mbar_buff_free(Buffer* buff);

static inline uint8_t*
mbar_buff_start(Buffer* buff) {
  return &buff->data[0];
}

static inline uint8_t*
mbar_buff_current(Buffer* buff) {
  return &buff->data[buff->length];
}

static inline uint64_t
mbar_buff_remaining(Buffer* buff) {
  return buff->capacity - buff->length;
}

static inline uint8_t*
mbar_buff_end(Buffer* buff) {
  return &buff->data[buff->capacity];
}

static inline void
mbar_buff_copyb(Buffer* buff, Buffer* src) {
  ASSERT(buff);
  ASSERT(src);
  mbar_buff_copy(buff, src->data, src->length);
}

static inline void
mbar_buff_appendb(Buffer* buff, Buffer* src) {
  ASSERT(buff);
  ASSERT(src);
  mbar_buff_append(buff, src->data, src->length);
}
