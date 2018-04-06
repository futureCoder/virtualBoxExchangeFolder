#coding=utf8
import itchat
import requests

secretKey = '8b00a2db-d358-4ede-82b9-574c7f907eb1'
token = '$2a$10$2qRBDGlS9WTpR.oEAmYMZOow1cWCau1g/gsuiS.X5tNllXUk8fs0.'

def get_welcome_msg():
	apiUrl = 'http://open-api.zyou.cn/open-api/v1/robot/welcome-msg'
	data = {
		'secretKey' : secretKey,
		'access_token' : token,
	}
	try:
		r = requests.get(apiUrl, data=data).json()
		return r
	except:
		return


def get_response(msg):
	apiUrl = 'http://open-api.zyou.cn/open-api/v1/robot/interlocution'
	data = {
		'secretKey' : secretKey,
		'question' : msg,
		'visitorId' : 'dong123',
		'access_token' : token, 
	}
	try:
		r = requests.post(apiUrl, data=data).json()
		## print(r)
		return r
	except:
		return

def get_token():
	tokenUrl = 'http://open-api.zyou.cn/open-api/v1/uaa/oauth2/token'
	tokenRequest = {
		'grant_type' : 'client_credentials',
		'client_id' : '11061',
		'client_secret' : '$2a$10$YYZ.QAS7VVS9FSxct9vACOplmPxo1prrNNmE2ORhUwePqgEiGDjP2',
	}
	try:
		r = requests.post(tokenUrl, data=tokenRequest).json()
		print(r)
		return r.get('access_token')
	except:
		return


@itchat.msg_register(itchat.content.TEXT)
def zhi_u_reply(msg):
	defaultReply = 'I received: ' + msg['Text']
	reply = get_response(msg['Text'])
	return reply or defaultReply

r = get_response('hello')
#r = get_welcome_msg()
print(r)
##itchat.auto_login(hotReload=True)
##itchat.run()
