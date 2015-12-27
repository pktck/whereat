#!/usr/bin/python
"""Prints the file descriptor of an attached Arduino Due programming port."""

import serial.tools.list_ports_posix

def main():
    ports = [port[1] for port in serial.tools.list_ports_posix.comports()
            if port[2] == 'USB VID:PID=2341:003d SNR=853323434323511121E0']

    return ports[0]


if __name__ == '__main__':
    port = main()
    assert port
    print port

