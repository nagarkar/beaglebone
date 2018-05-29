# Usage: python streaming_telemetry_client.py
"""The Python client implementation of the beaglebone/cpp/qpcpp/examples/posix/imu attitude api

Before using this client, set the address correctly in the code.

To kill this client, close the browser window that it opens.
"""

from __future__ import print_function
from time import sleep

# conda install -c mwcraig vpython (requires python 2.7)
from vpython import *
import math

# https://goo.gl/SCQ7V6,  pip install pyquaternion
from pyquaternion import Quaternion

# python -m pip install --upgrade pip (need pip version 9.0.1 or higher)
# python -m pip install grpcio
# python -m pip install grpcio-tools
import grpc

# Generated files (see build.sh)
import telemetry_service_pb2
import telemetry_service_pb2_grpc

# Setup vpython scene & cube
scene.title = "VPython: Draw a rotating cube"
scene.range = 2
scene.autocenter = True
cube = box()    # using defaults, see http://www.vpython.org/contents/docs/defaults.html 

def run():
    #runpolling()
    runstream()

def runpolling():
    channel = grpc.insecure_channel('192.168.8.1:50051')
    stub = telemetry_service_pb2_grpc.TelemetryServiceStub(channel)
    while(1==1):
      response = stub.GetAttitudeQ(telemetry_service_pb2.AttitudeQRequest())
      if response is not None:
         print('orientation,%.3f,%.3f,%.3f,%.3f,%s' % (response.Qc, response.Qx, response.Qy, response.Qz, response.timestamp))
      sleep(.01);

# VPython rotate cube
def rotatecube(q_prev, q_new):
	  if (q_prev is "undefined"):
		  print("q_prev undef")
		  qeff = q_new
	  else:
		  qeff = q_new * q_prev.inverse
		  qeff = qeff.unit

	  angle = qeff.radians
	  axis = qeff.axis
	  cube.rotate( angle=angle, axis=vector(axis[0], axis[1], axis[2]) )
	  print("Axis %s, %s, %s %s" %(axis[0], axis[1], axis[2], angle))


def runstream():
    channel = grpc.insecure_channel('192.168.8.1:50051')
    stub = telemetry_service_pb2_grpc.TelemetryServiceStub(channel)
    q_prev = "undefined"
    while(1==1):
      responses = stub.GetAttitudeStream(telemetry_service_pb2.AttitudeQRequest())
      if responses is None:
           continue;
      for response in responses:
          c = response.Qc
          x = response.Qx
          y = response.Qy
          z = response.Qz
          q_new = Quaternion(c, x, y, z)
          q_new = q_new.unit
          rotatecube(q_prev , q_new)          
          q_prev = q_new          

if __name__ == '__main__':
    run()
