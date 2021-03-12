#ifdef __cplusplus
extern "C" {
#else
#include <stdbool.h>
#endif

void override_hook_all();
void override_unhook_all();

void hook_all();
void unhook_all();
bool hook_status();

#ifdef __cplusplus
}
#endif