from pynput.keyboard import Key, Controller

keyboard = Controller()

# Press and release ESC
keyboard.press(Key.esc)
keyboard.release(Key.esc)

