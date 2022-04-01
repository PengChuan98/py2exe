import os
import sys

def MessageBox(msg, info = 'Message'):
    import ctypes
    ctypes.windll.user32.MessageBoxW(None, str(msg), str(info), 0)
    return 0
os.MessageBox = MessageBox

MessageBox("this is entry main file")

print("Test String.....")


