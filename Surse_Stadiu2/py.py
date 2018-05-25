import pyautogui
from time import sleep
import serial

ser = serial.Serial()
ser.baudrate = 9600
ser.port = 'COM5'
ser.parity = 'E'
ser.open()
ser.timeout = 0.3

while True:
    s=ser.read(1000)
    ascii = s.decode("ascii")
    vector = []
    charlist = set()
    special = ['.', ',', '*', '<', '>', '+', '-', '=', '_', '~', '(', ')', '#', '@', '&', '%']
    normal = ['a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p']
    if(len(ascii) > 0):
        for i in (0, len(ascii)-1):
            charlist.add(ascii[i])
        while len(charlist) != 0:
            ch = charlist.pop()
            if normal.count(ch) != 0:
                pyautogui.keyDown(ch)
            else:
                indexinlist = special.index(ch) if ch in special else -1
                if indexinlist != -1 :
                    pyautogui.keyUp(normal[indexinlist])
