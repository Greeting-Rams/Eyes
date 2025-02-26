# Eyes
Eye movement, closing, stepper motors
Traceback (most recent call last):
  File "/usr/lib/python3/dist-packages/serial/serialposix.py", line 322, in open
    self.fd = os.open(self.portstr, os.O_RDWR | os.O_NOCTTY | os.O_NONBLOCK)
OSError: [Errno 16] Device or resource busy: '/dev/ttyACM0'

During handling of the above exception, another exception occurred:

Traceback (most recent call last):
  File "/home/greetingrams/Python/practice/./instruction.py", line 5, in <module>
    ser=serial.Serial('/dev/ttyACM0', 9600, timeout=1)
  File "/usr/lib/python3/dist-packages/serial/serialutil.py", line 244, in __init__
    self.open()
  File "/usr/lib/python3/dist-packages/serial/serialposix.py", line 325, in open
    raise SerialException(msg.errno, "could not open port {}: {}".format(self._port, msg))
serial.serialutil.SerialException: [Errno 16] could not open port /dev/ttyACM0: [Errno 16] Device or resource busy: '/dev/ttyACM0
