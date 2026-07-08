#!/bin/bash
set -e

cd "$(dirname "$0")"

echo "[+] Building image..."
cd Image
python3 png2bin.py frames/*.png image.bin
cd ..

echo "[+] Building song..."
cd Song
python2 midi2bin.py nyan.mid song.bin
cd ..

echo "[+] Checking sizes..."

IMG_SIZE=$(stat -c%s Image/image.bin)
SONG_SIZE=$(stat -c%s Song/song.bin)
MSG_SIZE=$(stat -c%s Other/message.txt)

if [ "$IMG_SIZE" -ne 24000 ]; then
    echo "ERROR: image.bin is $IMG_SIZE bytes (expected 24000)"
    exit 1
fi

if [ "$SONG_SIZE" -ne 476 ]; then
    echo "ERROR: song.bin is $SONG_SIZE bytes (expected 52)"
    exit 1
fi

if [ "$MSG_SIZE" -ne 76 ]; then
    echo "ERROR: message.txt is $MSG_SIZE bytes (expected 76)"
    exit 1
fi

echo "[+] Creating data.bin..."
cat Image/image.bin Song/song.bin Other/message.txt > data.bin

echo "[+] Compressing..."
Compressor/compressor.exe data.bin compressed.bin

echo "[+] Done!"
ls -lh data.bin compressed.bin
