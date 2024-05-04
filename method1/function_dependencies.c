#include <stdint.h>
#include <stdio.h>
#include <string.h>

void print_arr8_32bit(const uint8_t *padded_msg) {
  printf("%02x%02x%02x%02x", *padded_msg, *(padded_msg + 1), *(padded_msg + 2),
         *(padded_msg + 3));
}

void printHASH(const uint32_t *var) {
  printf("%08x %08x %08x %08x %08x\n", *var, *(var + 1), *(var + 2), *(var + 3),
         *(var + 4));
}

/*
 * This function will print the 512 msg Block as 16 32 bit words in hex
 * */
void print_block_as_hex(uint8_t *padded_msg) {
  for (int i = 0; i < 64; i += 4) {
    if (i != 0) {
      printf(" ");
    }
    print_arr8_32bit(&padded_msg[i]);
  }
}

void char_uint8_arr(uint8_t *pad, char *msg) {
  uint64_t msgLen = strlen(msg);
  for (int i = 0; i < msgLen; i++) {
    *(pad + i) = *(msg + i);
  }
  *(pad + msgLen) = 0x80;
}

void addSizeToPad(uint8_t *padded_msg, char *message) {
  uint64_t msgSize_bin = strlen(message) * 8;
  for (int i = 0; i < 8; i++) {
    padded_msg[56 + i] = (msgSize_bin >> (56 - i * 8)) & 0xFF;
  }
}

/*
 * populate 80Word sequence
 */
void populateWordSequence(const uint8_t *padded_msg, uint32_t W[]) {

    for (int i = 0; i < 16; i++) {
      W[i] = (padded_msg[i * 4] << 24);
      W[i] |= (padded_msg[(i * 4) + 1] << 16);
      W[i] |= (padded_msg[(i * 4) + 2] << 8);
      W[i] |= padded_msg[(i * 4) + 3];
    }
}

/*
 * function F for compression
 */
uint32_t F(uint8_t t, uint32_t B, uint32_t C, uint32_t D) {
  if (t <= 19) {
    return (B & C) | ((~B) & D);
  } else if (t <= 39) {
    return B ^ C ^ D;
  } else if (t <= 59) {
    return (B & C) | (B & D) | (C & D);
  } else if (t <= 79) {
    return B ^ C ^ D;
  } else {
    return 0x00000000;
  }
}

/*
 * get constants when needed
 */
uint32_t K(uint8_t t) {
  if (t <= 19) {
    return 0x5A827999;
  } else if (t <= 39) {
    return 0x6ED9EBA1;
  } else if (t <= 59) {
    return 0x8F1BBCDC;
  } else if (t <= 79) {
    return 0xCA62C1D6;
  } else {
    return 0x00000000;
  }
}

/*
 * Circular Shift Left
 */
uint32_t S(uint32_t X, uint32_t bits) {
  return (X << bits) | (X >> (32 - bits));
}

/*
 * addition (I1+I2)mod2^32 to ensure 32 bit size
 */
uint32_t plus(uint32_t I1, uint32_t I2) {
  return (uint32_t)(((uint64_t)I1 + (uint64_t)I2) %
                    4294967296); //((uint64_t)pow(2, 32)));
}

int compareHash(const uint32_t hash[5], const uint32_t expHash[5]) {
  for (int i = 0; i < 5; i++) {
    if (hash[i] != expHash[i]) {
      return 0;
    }
  }
  return 1;
};
