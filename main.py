from lib.audio.musik_box import WAV
import matplotlib.pyplot as plt

import time


def main():
    wav = WAV("sample/sample_audio.wav")
    start = time.perf_counter()
    data = wav.loadAudio()
    print(time.perf_counter() - start)
    left = [d.left for d in data]
    right = [d.right for d in data]

    _, axs = plt.subplots(2, 1, figsize=(10, 4))

    axs[0].plot(left)
    axs[0].set_title('Left')

    axs[1].plot(right)
    axs[1].set_title('Right')

    axs[0].set_ylim(-15_000, 15_000)
    axs[1].set_ylim(-15_000, 15_000)
    plt.tight_layout()
    plt.show()


if __name__ == "__main__":
    main()
