//#ifndef RX2_UTILS_H
//#define RX2_UTILS_H
//#pragma once
/** RandomX utils */

void rx_slow_hash_allocate_state(void);
void rx_slow_hash_free_state(void);
uint64_t rx_seedheight(const uint64_t height);
void rx_seedheights(const uint64_t height, uint64_t *seed_height, uint64_t *next_height);
bool rx_needhash(const uint64_t height, uint64_t *seedheight, const int miners);
void rx_seedhash(const uint64_t seedheight, const char *hash, const int miners);
void rx_slow_hash(const void *data, size_t length, char *hash, const int miners);
void rx_alt_slowhash(const uint64_t mainheight, const uint64_t seedheight, const char *seedhash, const void *data, size_t length, char *hash);
void rx_reorg(const uint64_t split_height);

//#endif