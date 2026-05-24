#ifndef GNS_WASM_ACHIEVEMENTS_H
#define GNS_WASM_ACHIEVEMENTS_H

#define GNS_ACH_STARTUP 1
#define GNS_ACH_FIRST_LEVEL 2
#define GNS_ACH_GAME_COMPLETE 3

#define GNS_SCORE_TOTAL_TIME 1

#ifdef GNS_WASM_RAW
__attribute__((import_module("env"), import_name("gns_host_achievement")))
void gns_host_achievement(int event_id, int value);
__attribute__((import_module("env"), import_name("gns_host_score")))
void gns_host_score(int score_id, int value);
static inline void GNS_Achievement(int event_id, int value) {
    gns_host_achievement(event_id, value);
}
static inline void GNS_Score(int score_id, int value) {
    gns_host_score(score_id, value);
}
#else
static inline void GNS_Achievement(int event_id, int value) { (void)event_id; (void)value; }
static inline void GNS_Score(int score_id, int value) { (void)score_id; (void)value; }
#endif

#endif
