import serial
import time, wave, pymedia.audio.sound as sound

ser = serial.Serial('COM17', 115200, timeout=0)
format= sound.AFMT_S16_LE

readfromser = 0

while 1 :
  readfromser=ser.readline()
  print readfromser
  if readfromser == '0' :
      print "No colour detected"
  if readfromser == '1' :
      print "This comparison works"
      print "breakpoint1"
      f=wave.open('a.wav', 'rb')
      sampleRate=f.getframerate()
      channels= f.getnchannels()
      s= f.readframes( 300000 )
      snd= sound.Output( sampleRate, channels, format )
      snd.play( s )
      #while snd.isPlaying(): time.sleep( 0.05 )
  if readfromser == '2' :
      print "This comparison works"
      print "breakpoint1"
      f=wave.open('b.wav', 'rb')
      sampleRate=f.getframerate()
      channels= f.getnchannels()
      s= f.readframes( 300000 )
      snd= sound.Output( sampleRate, channels, format )
      snd.play( s )
      #while snd.isPlaying(): time.sleep( 0.05 )
  if readfromser == '3' :
      print "This comparison works"
      print "breakpoint1"
      f=wave.open('c.wav', 'rb')
      sampleRate=f.getframerate()
      channels= f.getnchannels()
      s= f.readframes( 300000 )
      snd= sound.Output( sampleRate, channels, format )
      snd.play( s )
      #while snd.isPlaying(): time.sleep( 0.05 )
  if readfromser == '4' :
      print "This comparison works"
      print "breakpoint1"
      f=wave.open('csharp.wav', 'rb')
      sampleRate=f.getframerate()
      channels= f.getnchannels()
      s= f.readframes( 300000 )
      snd= sound.Output( sampleRate, channels, format )
      snd.play( s )
      #while snd.isPlaying(): time.sleep( 0.05 )
  time.sleep(1)
