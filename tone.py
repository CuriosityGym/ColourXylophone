import math
import numpy
import pyaudio

duration=0.3
freq=300
def sine(frequency, length, rate):
    length = int(length * rate)
    factor = float(frequency) * (math.pi * 2) / rate
    return numpy.sin(numpy.arange(length) * factor)


def play_tone(stream, frequency=330, length=1, rate=44100):
    chunks = []
    chunks.append(sine(frequency, length, rate))

    chunk = numpy.concatenate(chunks) * 0.25

    stream.write(chunk.astype(numpy.float32).tostring())


if __name__ == '__main__':
    p = pyaudio.PyAudio()
    stream = p.open(format=pyaudio.paFloat32,
                    channels=1, rate=44100, output=1)

    play_tone(stream, freq, duration)
    play_tone(stream, freq*2, duration)
    play_tone(stream, freq*3, duration)
    play_tone(stream, freq*4, duration)
    play_tone(stream, freq*5, duration)
    play_tone(stream, freq*6, duration)
    play_tone(stream, freq*7, duration)
    play_tone(stream, freq*8, duration)

    stream.close()
    p.terminate()
