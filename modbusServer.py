
#from pymodbus.client.sync import ModbusTcpClient as ModbusClient

#cli = ModbusClient('127.0.0.1', port=5020)
#assert cli.connect()
#res = cli.read_input_registers(40003, unit=1)
#assert not res.isError()
#print(res.registers)


#g++ -std=c++11 -I /usr/include/modbus myprogram.c -o myprogram -lmodbus 

from pymodbus.server.sync import StartTcpServer
from pymodbus.datastore import ModbusSequentialDataBlock
from pymodbus.datastore import ModbusSlaveContext, ModbusServerContext

import logging
FORMAT = ('%(asctime)-15s %(threadName)-15s'
          ' %(levelname)-8s %(module)-15s:%(lineno)-8s %(message)s')
logging.basicConfig(format=FORMAT)
log = logging.getLogger()
log.setLevel(logging.DEBUG)

signal = 1

def run_server(signal):
    print("you are in server")
    store = ModbusSlaveContext(
        ir=ModbusSequentialDataBlock(1000,signal), 
        zero_mode=True
    )
    context = ModbusServerContext(slaves=store, single=True)
    StartTcpServer(context, address=("localhost", 1502))


if __name__ == "__main__":
    run_server(1)
