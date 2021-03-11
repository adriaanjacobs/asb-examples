#ifdef __cplusplus
extern "C" {
#else
#include <stdbool.h>
#endif

void hook_all();
void unhook_all();
bool hook_status();

#ifdef __cplusplus
}
#endif