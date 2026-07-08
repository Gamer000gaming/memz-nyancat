import midi
import sys

pattern = midi.read_midifile(sys.argv[1])

def pitchconv(pitch):
    return int(round(1193180.0 / (2 ** ((pitch - 69) / 12.0) * 440), 0))

with open(sys.argv[2], "wb") as out:
    d = 0
    bytes_written = 0  # track total bytes written
    
    for event in pattern[0]:
        if isinstance(event, midi.NoteOnEvent) and event.velocity != 0:
            d = int(round(event.tick / 48.0, 0))
        elif isinstance(event, midi.NoteOffEvent) or (
            isinstance(event, midi.NoteOnEvent) and event.velocity == 0
        ):
            d += int(round(event.tick / 48.0, 0))
            p = pitchconv(event.pitch)
            
            # split into bytes
            low = p & 0xff
            high = ((d & 0x07) << 5) | ((p >> 8) & 0x1f)
            
            # check if adding these 2 bytes will exceed the 476-byte limit
            if bytes_written + 2 > 476:
                break
                
            out.write(chr(low) + chr(high))
            bytes_written += 2  # increment by the 2 bytes just written
            
            d = 0
