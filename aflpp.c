#include "afl-fuzz.h"
#include "afl-mutations.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h> 
#include <string.h>
#include <stdbool.h>


typedef struct my_mutator {

  afl_state_t *afl;
  u8          *buf;
  u32          buf_size;

} my_mutator_t;

my_mutator_t *afl_custom_init(afl_state_t *afl, unsigned int seed) {

  (void)seed;

  my_mutator_t *data = calloc(1, sizeof(my_mutator_t));
  if (!data) {

    perror("afl_custom_init alloc");
    return NULL;

  }

  if ((data->buf = malloc(MAX_FILE)) == NULL) {

    perror("afl_custom_init alloc");
    return NULL;

  } else {

    data->buf_size = MAX_FILE;

  }

  data->afl = afl;

  return data;

}
const size_t start_offset = 0x10030;


size_t afl_custom_fuzz(my_mutator_t *data, uint8_t *buf, size_t buf_size,
                       u8 **out_buf, uint8_t *add_buf, size_t add_buf_size,
                       size_t max_size) {
                  
  // Check if max_size exceeds data->buf_size and reallocate if necessary
  if (max_size > data->buf_size) {
    u8 *ptr = realloc(data->buf, max_size);
    if (!ptr) {
      return 0;
    } else {
      data->buf = ptr;
      data->buf_size = max_size;
    }
  }
  u32 havoc_steps = 1 + rand_below(data->afl, 16);
     
  memcpy(data->buf, buf, buf_size);
  size_t end_offset = start_offset+0x4;;
  size_t i = start_offset;
  while(i+0x3 < buf_size){
     if (data->buf[i+2] == 0x5f && data->buf[i+3] == 0xd6 &&
        data->buf[i] == 0xc0 && data->buf[i+1] == 0x03) {
        end_offset = i;
        break;
      }
      i=i+0x4;
  }
  size_t segment_length= end_offset - start_offset;
  u32 mutated_segment_len = afl_mutate(data->afl, data->buf + start_offset, segment_length, 
                                       havoc_steps, false, true, add_buf, add_buf_size, max_size);

  *out_buf = data->buf;
  return buf_size;
}


/**
 * Deinitialize everything
 *
 * @param data The data ptr from afl_custom_init
 */
void afl_custom_deinit(my_mutator_t *data) {

  free(data->buf);
  free(data);

}

