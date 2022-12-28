import cv2
import numpy as np

from pymodbus3.server.sync import StartTcpServer
from pymodbus3.datastore import ModbusSequentialDataBlock
from pymodbus3.datastore import ModbusSlaveContext, ModbusServerContext
from pymodbus3.client.sync import ModbusTcpClient as ModbusClient

import logging



lower_blue = np.array([60,180,0], dtype = "uint8" )
upper_blue = np.array([150,255,90], dtype = "uint8")

lower_red = np.array([130,190,30], dtype = "uint8" )
upper_red = np.array([220,255,120], dtype = "uint8")

#lower_yellow = np.array([15,50,100] , dtype ="uint8")
#upper_yellow = np.array([50,80,200], dtype = "uint8")

lower_yellow = np.array([0,160,60] , dtype ="uint8")
upper_yellow = np.array([50,220,145], dtype = "uint8")

RGB_lower_red = np.array([130,30,30])
RGB_upper_red = np.array([220,129,160])


FORMAT = ('%(asctime)-15s %(threadName)-15s'
          ' %(levelname)-8s %(module)-15s:%(lineno)-8s %(message)s')
logging.basicConfig(format=FORMAT)
log = logging.getLogger()
log.setLevel(logging.DEBUG)

thresh = 40

cap = cv2.VideoCapture(0)


def run_server(signal):
    print("you are in server")
    store = ModbusSlaveContext(
        ir=ModbusSequentialDataBlock(1000,signal), 
        zero_mode=True
    )
    context = ModbusServerContext(slaves=store, single=True)
    StartTcpServer(context, address=("192.168.0.10", 1502))

def run_client():
	client = ModbusClient('192.168.0.201', port=5020)
	connection = client.connect()
	res = client.read_input_registers(0, unit=1)

	return res.registers
	
class colorSegmentation:

	def detectColor(self,img,hsv_l, hsv_u):

		hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
		RGB = np.array([hsv[0] , hsv[1] , hsv[2] ])
		#print(RGB)
		minHSV = np.array([hsv[0] - thresh, hsv[1] - thresh, hsv[2] - thresh])
		maxHSV = np.array([hsv[0] + thresh, hsv[1] + thresh, hsv[2] + thresh])
		#print(minHSV)
		#print("#################################")
		#print(maxHSV)
		mask = cv2.inRange(hsv, hsv_l, hsv_u)
		res = cv2.bitwise_and(img,hsv, mask=mask)
		cv2.imwrite("img/hsv.jpg", hsv)
		cv2.imwrite("img/frame.jpg", img)
		cv2.imwrite("img/mask.jpg", mask)
		cv2.imwrite("img/res.jpg", res)
		return res



	def contains_spesific_Color(self,color,img):	
		success = False
		#print(img.shape)
		for x in range(img.shape[0]):
			for y in range(img.shape[1]):
				r,g,b = img[x,y]
				#print(r,g,b)
				if color == "yellow":
					if r > 0  and r < 50 and g > 30 and g < 80 and b > 50 and b < 100 :
						success = True

				elif color == "red":
					if r > 0  and r < 80 and g > 0 and g < 78 and b > 60 and b < 170 :
						success = True

				elif color == "blue":
					if r > 20  and r < 50 and g > 10 and g < 40 and b > 0 and b < 25 :
						success = True

		print(success)
		return success



	def setColor(self, value):
		self.color = value
	
	def getColor(self):
		print(self.color)
		return self.color


	def getSignal(self):
		_, frame = cap.read()
		#frame = cv2.imread("/home/shaza/RSD2019/Vision/img/img/frame1.jpg")
		RGB = np.array([frame[0] , frame[1] , frame[2] ])
		minRGB = np.array([frame[0] - thresh, frame[1] - thresh, frame[2] - thresh])
		maxRGB = np.array([frame[0] + thresh, frame[1] + thresh, frame[2] + thresh])
		
		"""print(minRGB)
		print("#################################")
		print(maxRGB)
		print("#################################")
		print("#################################")
		print(RGB)"""




		self.color = self.getColor()

		if self.color == 0:
			FinialImg = self.detectColor(frame,lower_yellow, upper_yellow)
			success = self.contains_spesific_Color("yellow",FinialImg)
			if success == True:
				signal = 1
			else:	
				signal = 0	

		elif self.color == 1:
			FinialImg = self.detectColor(frame,lower_red, upper_red)
			success = self.contains_spesific_Color("red",FinialImg)
			if success == True:

				signal = 1
			else:	
				signal = 0	

		elif self.color == 2:
			FinialImg = self.detectColor(frame,lower_blue, upper_blue)
			success = self.contains_spesific_Color("blue",FinialImg)
			if success == True:

				signal = 1	
			else:	

				signal = 0	
		else:	

			signal = 0
		print(signal)

		return signal


colorSeg = colorSegmentation() 


if __name__ == "__main__":
    #color = run_client()
    color = 1
    colorSeg.setColor(color)
    signal = colorSeg.getSignal()
    print(signal)
    #run_server(signal)


