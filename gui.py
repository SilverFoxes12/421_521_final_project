from Tkinter import *
import tkFont
import RPi.GPIO as GPIO

GPIO.setmode(GPIO.BOARD)
GPIO.setup(40, GPIO.OUT)
GPIO.output(40, GPIO.LOW)

win = Tk()

myFont = tkFont.Font(family = 'Helvetica', size = 36, weight = 'bold')

def ledON():
	print("LED button pressed")
	if GPIO.input(40) :
 		GPIO.output(40,GPIO.LOW)
		ledButton["text"] = "LED ON"
	else:
		GPIO.output(40,GPIO.HIGH)
                ledButton["text"] = "LED OFF"

def exitProgram():
	print("Exit Button pressed")
        GPIO.cleanup()
	win.quit()	


win.title("First GUI")
win.geometry('800x480')

#exitButton  = Button(win, text = "Exit", font = myFont, command = exitProgram, height =2 , width = 6) 
#exitButton.pack(side = BOTTOM)

#ledButton = Button(win, text = "PRESS ME!!!", font = myFont, command = ledON, height = 2, width =8 )
#ledButton.pack()

#newButton = Button(win, text = "JOHNNN!!", font = myFont, height = 1, width = 9)
#newButton.pack()

Label(win, text="Soil Humidity").grid(row=0)
Label(win, text="Sunlight").grid(row=1)

e = Entry(win)
e.grid(row=0, column=1)
e.delete(0, END)
e.insert(0, "a default value")


f = Entry(win)
f.grid(row=1, column=1)
f.delete(0, END)
f.insert(0, "a default value")

saveButton = Button(win, text = "SAVE", font = myFont, height = 2, width = 8)
saveButton.grid(row = 3, column=0)

mainloop()
