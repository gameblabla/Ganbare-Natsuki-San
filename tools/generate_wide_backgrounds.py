#!/usr/bin/env python3
"""Generate 400x240 widescreen background BMPs from the 4:3 source set.

The original backgrounds are mostly 320x240.  For the widescreen asset set we
fill the 400x240 framebuffer by preserving aspect ratio, scaling up, and
center-cropping.  For 320x240 this means 1.25x to 400x300 followed by a 30 px
crop from top and bottom.  A mild unsharp pass keeps the result from looking
soft after the resample.
"""
from __future__ import annotations

import argparse
from pathlib import Path
from PIL import Image, ImageFilter

TARGET_W = 400
TARGET_H = 240


def convert_one(src: Path, dst: Path) -> None:
    im = Image.open(src).convert("RGB")
    src_w, src_h = im.size
    scale = max(TARGET_W / src_w, TARGET_H / src_h)
    scaled_w = max(TARGET_W, int(round(src_w * scale)))
    scaled_h = max(TARGET_H, int(round(src_h * scale)))

    im = im.resize((scaled_w, scaled_h), Image.Resampling.LANCZOS)
    left = max(0, (scaled_w - TARGET_W) // 2)
    top = max(0, (scaled_h - TARGET_H) // 2)
    im = im.crop((left, top, left + TARGET_W, top + TARGET_H))
    im = im.filter(ImageFilter.UnsharpMask(radius=0.65, percent=55, threshold=3))

    dst.parent.mkdir(parents=True, exist_ok=True)
    im.save(dst, format="BMP")


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--source", default="data/4by3/image/bak", help="source background directory")
    parser.add_argument("--output", default="data/wide/image/bak", help="widescreen background directory")
    args = parser.parse_args()

    src_root = Path(args.source)
    out_root = Path(args.output)
    if not src_root.is_dir():
        raise SystemExit(f"source directory not found: {src_root}")

    count = 0
    for src in sorted(src_root.glob("*.bmp")):
        # Runtime loads numeric backgrounds as N.bmp.  Keep 19_256.bmp if it is
        # present, but it is not used by the normal BAK_IMAGE_PATH loader.
        convert_one(src, out_root / src.name)
        count += 1
    print(f"generated {count} widescreen backgrounds in {out_root}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
