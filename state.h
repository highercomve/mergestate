#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

uint8_t *MergeStates(const uint8_t *src, int src_len, const uint8_t *patch, int patch_len, int *out_len);
uint8_t *RemoveFromState(const uint8_t *src, int src_len, const uint8_t *key, int *out_len);
void FreeMem(uint8_t *ptr, int len);
void Deinit(void);

#ifdef __cplusplus
}
#endif
