cd Image
python3 png2bin.py frames/*.png image.bin
cd ../Song
python2 midi2bin.py nyan.mid song.bin

cd ..

cat Image/image.bin Song/song.bin Other/message.txt > data.bin
Compressor/compressor.exe data.bin compressed.bin
