#moss's HelloKhala Client
#add time type
import socket
import struct
import json

def login():
	send = {'type': 'login'}
	return send
def logout():
	send = {'type': 'logout'}
	return send
def devType():
	send = {'type': 'dev'}
	return send
def isLogin():
	send = {'type': 'isLogin'}
	return send	
def nodeId():
	send = {'type': 'nodeId'}
	return send
def time():
	send = {'type':'time'}
	return send
def default():
	return -1

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect(('127.0.0.1', 2007))
operator = {'login':login,'logout':logout,'devType':devType,'isLogin':isLogin,'nodeId':nodeId,'time':time}  
while True:
	input = raw_input('[input cmd]:')
	sendStr = operator.get(input,default)()
	if sendStr == -1:
		print 'err type:',input,'please input again!'
		continue
	strjson = json.dumps(sendStr)
	print '[send msg]:',strjson
	#get msg len,and hostToNetWork32
	inputLen = len(strjson)
	pstr = '>I'+ str(inputLen)+'s'
	bytes = struct.pack(pstr, inputLen,strjson)
	s.send(bytes)
	d = s.recv(1024)
	if len(d) == 0:
		print 'exit'
		break
	#parse msg,and networkToHost
        parseStr = '>I' + str(len(d)-4) + 's'
        resLen,resStr = struct.unpack(parseStr,d)
        print '[rev msg]:',resStr
	print ''
	if d == 'logout success!':
		print 'exit'
		break
s.close()
