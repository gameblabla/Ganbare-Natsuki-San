# Newgrounds medals for the clang/WASM build

The WASM host has Newgrounds.io medal configuration for this project baked into
`newgrounds_config.js`:

- App ID: `62141:suIp6eIZ`
- Encryption key: `Q75eyadC5JXmGjf5LjQaKA==`

Configured medals:

| Local event | Newgrounds medal | ID | Points | Trigger |
| --- | --- | ---: | ---: | --- |
| `startup` | Booted up ! | 90232 | 5 | The game reaches `gns_start()` |
| `first_level` | First level | 90233 | 5 | Stage 1 is completed in normal play |
| `game_complete` | Game complete | 90234 | 50 | Stage 50 is completed in normal play |

Configured scoreboard:

| Local event | Newgrounds board | ID | Format | Posted value |
| --- | --- | ---: | --- | --- |
| `total_time` | Total time | 15895 | Time | Total clear time in milliseconds |

The score is posted through `ScoreBoard.postScore` only for a legitimate full
Time Attack run. The run must start from stage 1, must not use the title-screen
stage selector, must not be replay/demo playback, and must clear stages
sequentially through stage 50. Normal play still unlocks medals, but it does not
post to the `Total time` scoreboard. Newgrounds stores time scores as integer
milliseconds.

To enable online Newgrounds medal unlocks, put the official Newgrounds.io
JavaScript library next to `index.html` as `newgroundsio.min.js`. The build
already includes these script tags before `gns_wasm.js`:

```html
<script src="newgroundsio.min.js"></script>
<script src="newgrounds_config.js"></script>
<script src="gns_wasm.js"></script>
```

If `newgroundsio.min.js` is absent, the Makefile emits an empty placeholder so
the page still loads. The game will still run and local achievement toasts still work. When
Newgrounds is configured but the user is not signed in, medal unlocks and score
posts are kept pending in `localStorage` and retried after sign-in.

The local toast thumbnails in `achievements/` were generated from bundled game
art. Newgrounds medal icons still need to be assigned on the Newgrounds project
page; the API unlock call only unlocks existing medal IDs.


## Achievement icons and first-level detection

The WASM build uses `achievements/startup.png`, `achievements/first_level.png`, and `achievements/game_complete.png` for local toast thumbnails. These are copied into `build/wasm-clang/achievements/` during `make -f Makefile.wasm`.

The first-level medal is emitted in two places for robustness: the normal stage-clear path and a WASM save-file progress fallback. If `work.sav` is written with the next-stage field at 2 or higher, the browser host also attempts the `first_level` Newgrounds medal. This avoids missing the medal if the original scene transition bypasses the direct clear hook.


## Library compatibility

This build supports the static `NGIO` JavaScript helper used by the bundled `newgroundsio.min.js` file. It initializes with `preloadMedals: true` and `preloadScoreBoards: true`, then calls `NGIO.unlockMedal(id)` and `NGIO.postScore(boardId, value, tag)` for online medals and scores. Local achievement toasts are separate from Newgrounds unlock state.
