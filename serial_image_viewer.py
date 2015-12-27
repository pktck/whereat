#!/usr/bin/python
"""Serial image viewer module."""
import serial
import cv2
import numpy as np
import serial.tools.list_ports_posix
import operator

# pylint: disable=no-member

BEGIN_FRAME = 'BEGIN_FRAME'
END_FRAME = 'END_FRAME'

class SerialImageViewer(object):
    """Used to view images from the serial port."""

    def __init__(self, *args, **kwargs):
        """Creates a new SerialImageViewer. Arguments are passed to the
        __init__ method of a new Serial object."""
        if not (args or kwargs):
            ports = [port[0] for port in serial.tools.list_ports_posix.comports()
                    if port[2] == 'USB VID:PID=2341:003e']
            kwargs = {'port': ports[0], 'baudrate': 115200, 'timeout': 1.0}
        self._serial_port = serial.Serial(*args, **kwargs)

    def get_frame(self):
        """Fetches a frame from the serial port and returns an OpenCV image."""
        self._serial_port.close()
        self._serial_port.open()

        self._request_frame()

        serial_data = self._serial_port.readall()

        frame_start_idx = serial_data.find(BEGIN_FRAME) + len(BEGIN_FRAME)
        frame_end_idx   = serial_data.find(END_FRAME)

        print serial_data[0:frame_start_idx]
        print serial_data[frame_end_idx:]

        raw_frame = serial_data[frame_start_idx:frame_end_idx]

        np_frame = np.fromstring(raw_frame, dtype=np.uint8)
        # np_frame = np_frame.reshape((30, 30))

        # image = cv2.fromarray(np_frame)

        # return image
        return np_frame

    def _request_frame(self):
        """Request a frame from the device."""
        self._send_command('GET_FRAME')

    def _send_command(self, command):
        """Send a command to the device."""
        self._serial_port.write(command + '\n')
        self._serial_port.flush()

    def display_frame(self, frame=None):
        """Displays a frame in an X11 window."""
        if frame is None:
            frame = self.get_frame()
        cv2.namedWindow('frame')
        cv2.imshow('frame', frame)
        cv2.waitKey(0)




def main():
    """To be run in __main__."""
    pass

if __name__ == '__main__':
    main()
