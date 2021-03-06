import signal
import serial
import time, wave
import sys
import glob
import math
import numpy
import pyaudio
from random import randint
import atexit

baudrate=115200
timeout=0
testResponse='A'
waitIterations=5

duration=0.06
freq=450
samplingRate=44100
def serial_ports():
    foundPort=False
    """ Lists serial port names

        :raises EnvironmentError:
            On unsupported or unknown platforms
        :returns:
            A list of the serial ports available on the system
    """
    if sys.platform.startswith('win'):
        ports = ['COM%s' % (i + 1) for i in range(256)]
    elif sys.platform.startswith('linux') or sys.platform.startswith('cygwin'):
        # this excludes your current terminal "/dev/tty"
        ports = glob.glob('/dev/tty[A-Za-z]*')
    elif sys.platform.startswith('darwin'):
        ports = glob.glob('/dev/tty.*')
    else:
        raise EnvironmentError('Unsupported platform')

    result=""
    for port in ports:
        try:
	    print port
	    counter=0
            s = serial.Serial(port, baudrate, timeout=timeout)
	    #portDictionary=s.getSettingsDict()
	    time.sleep(5)
	    #print("/".join(portDictionary))
	    #for key in portDictionary:
  		#print "the key name is " + key + " and its value is " + str(portDictionary[key])
	    #s.write(enumResponse)
	    while(counter<waitIterations):
		if(s.inWaiting()> 0):	
			#print "Waiting"
			#print s.inWaiting()
			#s.write(enumResponse)
			time.sleep(0.5)	
			response=s.readline()
			counter=counter+1    
			print response 	
			result=port
			s.close()
			foundPort=True
			break
		if(s.inWaiting()==0):
                        time.sleep(1)	
			counter=counter+1   							
	            	continue
	    if foundPort:
		break	
            s.close()
        except (OSError, serial.SerialException):
            pass
    return result


def sine(frequency, length, rate):
    length = int(length * rate)
    factor = float(frequency) * (math.pi * 2) / rate
    return numpy.sin(numpy.arange(length) * factor)


def play_tone(stream, frequency=freq, length=duration, rate=samplingRate):
    chunks = []
    chunks.append(sine(frequency, length, rate))

    chunk = numpy.concatenate(chunks) * 0.25

    stream.write(chunk.astype(numpy.float32).tostring())

def sigint_handler(signum, frame):
    print "Goodbye my dear fellow"
    SerialPortConn.close()
    quit()



SerialPort=serial_ports()
if SerialPort=="":
	print "Cannot find Device. Quitting"
        quit()


print "Found, Serial Port is "+ SerialPort
SerialPortConn = serial.Serial(SerialPort, baudrate, timeout=timeout)

readfromser = 0
p = pyaudio.PyAudio()
stream = p.open(format=pyaudio.paFloat32,channels=1, rate=samplingRate, output=1)




while 1 :
	if(SerialPortConn.inWaiting()>0):
		readfromser = SerialPortConn.readline()
		print readfromser
		if readfromser == '0' :
			print "No colour detected"
		if readfromser == '1' :
			#print "This comparison works"
			#print "breakpoint1"
			play_tone(stream, freq, 0.2)

		if readfromser == '2' :
			#print "This comparison works"
			#print "breakpoint1"
			play_tone(stream, freq*2, 0.2)


		if readfromser == '3' :
			#print "This comparison works"
			#print "breakpoint1"
			play_tone(stream, freq*3, 0.2)


		if readfromser == '4' :
			#print "This comparison works"
			#print "breakpoint1"
			play_tone(stream, freq*4, 0.2)
	#time.sleep(0.1)
        signal.signal(signal.SIGINT, sigint_handler)
