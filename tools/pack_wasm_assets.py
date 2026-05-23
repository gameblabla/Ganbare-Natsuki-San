#!/usr/bin/env python3
import argparse
import json
from pathlib import Path


def add_tree(entries, root, virtual_prefix):
    root = Path(root)
    if not root.exists():
        return
    for path in sorted(p for p in root.rglob('*') if p.is_file()):
        rel = path.relative_to(root).as_posix()
        vpath = f'{virtual_prefix}/{rel}' if rel else virtual_prefix
        entries.append((vpath, path))


def main():
    ap = argparse.ArgumentParser(description='Pack Ganbare Natsuki-san browser assets for the clang/Canvas WASM build.')
    ap.add_argument('--image-root', required=True)
    ap.add_argument('--data-root', required=True)
    ap.add_argument('--sound-root', default=None)
    ap.add_argument('--out-bin', required=True)
    ap.add_argument('--out-json', required=True)
    args = ap.parse_args()

    entries = []
    add_tree(entries, args.image_root, 'image')
    add_tree(entries, args.data_root, 'data')
    if args.sound_root:
        add_tree(entries, args.sound_root, 'sound')

    manifest = {}
    offset = 0
    out_bin = Path(args.out_bin)
    out_bin.parent.mkdir(parents=True, exist_ok=True)

    with out_bin.open('wb') as bf:
        for virtual_path, real_path in entries:
            data = real_path.read_bytes()
            manifest[virtual_path] = {'offset': offset, 'size': len(data)}
            bf.write(data)
            offset += len(data)

    out_json = Path(args.out_json)
    out_json.parent.mkdir(parents=True, exist_ok=True)
    out_json.write_text(json.dumps({'files': manifest}, separators=(',', ':')), encoding='utf-8')
    print(f'packed {len(entries)} files, {offset} bytes')


if __name__ == '__main__':
    main()
