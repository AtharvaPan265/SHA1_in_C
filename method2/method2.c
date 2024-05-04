#include "function_dependencies.c"

void paddingFunction(uint8_t *padded_msg, char *message, uint64_t blkCnt) {
  memset(padded_msg, 0, blkCnt * 64);
  char_uint8_arr(padded_msg, message);
  addSizeToPad((padded_msg + ((blkCnt - 1) * 64)), message);
  // printf("\n");
}

void compressionFunction(const uint8_t *padded_msg, uint32_t hash[5]) {
  uint32_t W[16];
  uint32_t MASK = 0x0000000f;

  populateWordSequence(&*padded_msg, W);

  uint32_t A = hash[0];
  uint32_t B = hash[1];
  uint32_t C = hash[2];
  uint32_t D = hash[3];
  uint32_t E = hash[4];

  uint32_t s;
  for (uint8_t t = 0; t < 80; t++) {
    s = t & MASK;
    if (t >= 16) {
      W[s] =
          S((W[(s + 13) & MASK] ^ W[(s + 8) & MASK] ^ W[(s + 2) & MASK] ^ W[s]),
            1);
    }
    uint32_t TEMP =
        (plus(S(A, 5), plus(F(t, B, C, D), plus(E, plus(W[s], K(t))))));
    E = D;
    D = C;
    C = S(B, 30);
    B = A;
    A = TEMP;

    // printf("t = %2d: %08x\t%08x\t%08x\t%08x\t%08x\n", t, A, B, C, D, E);
  }
  hash[0] = plus(hash[0], A);
  hash[1] = plus(hash[1], B);
  hash[2] = plus(hash[2], C);
  hash[3] = plus(hash[3], D);
  hash[4] = plus(hash[4], E);

  // printHASH(hash);
}

void SHA1(uint32_t *hash, char *message) {
  uint32_t H0 = 0x67452301;
  uint32_t H1 = 0xEFCDAB89;
  uint32_t H2 = 0x98BADCFE;
  uint32_t H3 = 0x10325476;
  uint32_t H4 = 0xC3D2E1F0;
  hash[0] = H0;
  hash[1] = H1;
  hash[2] = H2;
  hash[3] = H3;
  hash[4] = H4;
  uint64_t blkCnt =
      (strlen(message) < 56) ? 1 : ((strlen(message) + 63) / 64) + 1;
  uint8_t blockArr[blkCnt][64];
  paddingFunction(*blockArr, message, blkCnt);
  for (int i = 0; i < blkCnt; i++) {
    compressionFunction(blockArr[i], hash);
  }
}

int SHA_run_compare(char *message, uint32_t expHash[5]) {
  uint32_t hash[5];
  SHA1(hash, message);
  if (compareHash(hash, expHash)) {
    // printf("SUCCESS!!\nSHA1(%s) -> ", getName(message));
    // printHASH(hash);
    // printf("\n");
    return 1;
  }
  return 0;
}
int SHA_initTest() {
  int res = 1;
  char msg_abc[] = "abc";
  uint32_t res_abc[5] = {0xa9993e36, 0x4706816a, 0xba3e2571, 0x7850c26c,
                         0x9cd0d89d};

  char msg_abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq[] =
      "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq";
  uint32_t res_abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq[5] = {
      0X84983e44, 0X1c3bd26e, 0Xbaae4aa1, 0Xf95129e5, 0Xe54670f1};

  char msg_a[1000000];
  uint32_t res_a[5] = {0x34aa973c, 0xd4c4daa4, 0xf61eeb2b, 0xdbad2731,
                       0x6534016f};

  res *= SHA_run_compare(msg_abc, res_abc);
  res *= SHA_run_compare(
      msg_abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq,
      res_abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq);
  memset(msg_a, 0x61, 1000000);
  res *= SHA_run_compare(msg_a, res_a);
  return res;
}

void SHA_Print(char *message) {
  uint32_t hash[5];
  SHA1(hash, message);
  printHASH(hash);
}
