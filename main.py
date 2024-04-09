import time
import numpy as np
import matplotlib.pyplot as plt
from lib.audio.musik_box import WAV



def main():
    wav = WAV("sample/sample_audio.wav")
    start = time.perf_counter()
    data = wav.loadAudio()
    print(time.perf_counter() - start)
    left = [d.left for d in data][:100001]
    _ = [d.right for d in data]

    fft_result = np.fft.fft(left)
    fft_freq = np.fft.fftfreq(len(left), 1/wav.getSampleRate())

    plt.figure(figsize=(12, 6))

    plt.subplot(1, 2, 1)
    plt.plot(left)
    plt.title('Original Signal')
    plt.xlabel('Time [s]')
    plt.ylabel('Amplitude')

    plt.subplot(1, 2, 2)
    plt.stem(fft_freq, np.abs(fft_result))
    plt.title('FFT of the Signal')
    plt.xlabel('Frequency [Hz]')
    plt.ylabel('Magnitude')
    plt.xlim(0, wav.getSampleRate() / 2)

    plt.tight_layout()
    plt.show()


if __name__ == "__main__":
    main()
