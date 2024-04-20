import time
from lib.audio.musik_box import WAV

start1 = time.perf_counter()
wav = WAV("sample/sample_audio.wav")
end1 = time.perf_counter()

start2 = time.perf_counter()
data = wav.loadAudio()
end2 = time.perf_counter()

print(f"Mapping took {end1 - start1} s | Loading took {end2-start2} s")
