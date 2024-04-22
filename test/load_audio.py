import time
import numpy as np
import matplotlib.pyplot as plt
from lib.musik_box.audio.musik_box import WAV


start1 = time.perf_counter()
wav = WAV("sample/c_maj.wav")
end1 = time.perf_counter()

start2 = time.perf_counter()
data = wav.loadAudio()
end2 = time.perf_counter()

start3 = time.perf_counter()
data = wav.convertStereoToMono(data)
end3 = time.perf_counter()

fft_result = np.fft.fft(data)
sampling_rate = wav.getSampleRate()
n = len(data)
freq = np.fft.fftfreq(n, d=1/sampling_rate)
magnitude = np.abs(fft_result)

plt.plot(data)
plt.show()

plt.figure(figsize=(10, 5))
plt.plot(freq, magnitude)  # Plot the frequency vs. magnitude
plt.title('Frequency Spectrum')
plt.xlabel('Frequency (Hz)')
plt.ylabel('Magnitude')
plt.grid(True)
plt.show()

# print(f"Mapping took {end1-start1} s")
# print(f"Loading took {end2-start2} s")
# print(f"Converting to mono took {end3-start3} s")
