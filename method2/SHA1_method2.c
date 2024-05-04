#include "method2.c"
#include "testChar.c"
int main(void) {
  if (!SHA_initTest()) {
    printf("SOMETHING IS WRONG");
    return -1;
  }
  printf("All Tests Passed!\n");

  for(int i = 0 ; i < 200; i ++){
    printf("%6s %3d -> %s\n%10s -> ","String",i+1, challenge_strings[i],"Hash");
    SHA_Print(challenge_strings[i]);
  }
  return 0;
}
