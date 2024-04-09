from lib.audio.build.musik_box import WAV
import matplotlib.pyplot as plt


def main():
    wav = WAV("sample/sample_audio.wav")
    data = wav.loadAudio()
    left = [d.left for d in data]
    right = [d.right for d in data]

    plt.plot(left)
    plt.show()


if __name__ == "__main__":
    main()
