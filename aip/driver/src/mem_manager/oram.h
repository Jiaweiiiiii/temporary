#ifndef __ORAM_H__
#define __ORAM_H__
#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

struct MemoryInfo {
    void *phy_addr;
    void *vir_addr;
    unsigned int total_size;
    unsigned int used_size;
};

int oram_memory_init();

void oram_memory_deinit(void);

MemoryInfo get_oram_memory_info();

void *oram_malloc(unsigned int size);

void *oram_memalign(unsigned int align, unsigned int size);

void oram_free(void *addr);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif
