#ifndef GNS_WASM_FREESTANDING_TIME_H
#define GNS_WASM_FREESTANDING_TIME_H

typedef long time_t;
typedef long clock_t;

time_t time(time_t *out);
clock_t clock(void);
#define CLOCKS_PER_SEC 1000000L

#endif
