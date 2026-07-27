#include "demo_context.h"

#if defined(PLATFORM_WINDOWS)
int main(int argc, char **argv) {
  (void)argc;
  (void)argv;
#else
int main(void) {
#endif
  run_demo();
  return 0;
}
