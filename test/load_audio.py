import time
import numpy as np
import matplotlib.pyplot as plt
from lib.musik_box.audio.musik_box import WAV, AudioType

start1 = time.perf_counter()
wav = WAV("sample/sample_audio.wav")
end1 = time.perf_counter()

start2 = time.perf_counter()
data = wav.loadAudio()
end2 = time.perf_counter()

start3 = time.perf_counter()
if wav.getNumChannels() == AudioType.STEREO:
    data = wav.convertStereoToMono(data)
else:
    data = [d.left for d in data]
end3 = time.perf_counter()

plt.plot(data)
plt.show()

fft = np.fft.fft(data, len(data))

plt.plot(fft)
plt.show()

print(f"Mapping took {end1-start1} s")
print(f"Loading took {end2-start2} s")
print(f"Converting to mono took {end3-start3} s")
