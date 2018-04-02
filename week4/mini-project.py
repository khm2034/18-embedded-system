import RPi.GPIO as GPIO
import time
import pygame
import threading
GPIO.setwarnings(False)
GPIO.setmode(GPIO.BCM)
GPIO.setup(20, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
GPIO.setup(24, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
GPIO.setup(23, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
GPIO.setup(21, GPIO.OUT)

sound = 24
trig = 19
echo = 16
GPIO.setup(trig, GPIO.OUT)
GPIO.setup(echo, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
GPIO.setup(17, GPIO.OUT)
motor = GPIO.PWM(17, 50)
motor.start(2.5)
push = False
step = 1
on = False
music = False
pygame.mixer.init(48000, -16, 1, 1024)
pygame.mixer.music.load("music.mp3")

def init():
	global on
    global push 
    global music
    global step 
	on = False
	push = False
	GPIO.output(21, GPIO.LOW)
	music = False
	step = 1
	GPIO.remove_event_detect(sound)

def callback(sound):
	global step
	if(GPIO.input(sound)):
		step = 3

def chk_dis():
	global trig
	global echo
	global motor
	while(1):
		time.sleep(1)
		GPIO.output(trig, True)
		time.sleep(0.0001)
		GPIO.output(trig, False)
		while GPIO.input(echo) == 0:
			pulse_start = time.time()
		while GPIO.input(echo) == 1:
			pulse_end = time.time()
		distance = (pulse_end - pulse_start) * 17150
		distance = round(distance, 2)
		print("dis : " + str(distance) + "cm")
		if(distance <= 50):
			print("servo")
			motor.ChangeDutyCycle(7.5)
			time.sleep(0.2)
			motor.ChangeDutyCycle(2.5)
			time.sleep(0.2)

t = threading.Thread(target = chk_dis)
t.daemon = True
t.start()
while True:
	try:
		if(step == 1):
			push = GPIO.input(20)
			if(push == True):
				step = 2
				GPIO.add_event_detect(sound, GPIO.BOTH, callback=callback)
		elif(step == 2):
			push = GPIO.input(20)
			if(push == False):
				init()
				continue
			if(on == False):
				on = True
				GPIO.output(21, GPIO.HIGH)
			elif(on == True):
				on = False
				GPIO.output(21, GPIO.LOW)
			time.sleep(0.1)
		elif(step == 3):
			if(music == False):
				music = True
				pygame.mixer.music.play(-1)
			if(GPIO.input(23) == True):
				pygame.mixer.music.stop()
				init()
	except:
		GPIO.cleanup()
