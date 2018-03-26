#coding=utf8
import requests
import itchat

KEY = 'fe062b57b0964a5fb673d283b04272d3'

def get_response(msg):
    apiUrl = 'http://www.tuling123.com/openapi/api'
    data = {
        'key'    : KEY,
        'info'   : msg,
        'userid' : 'daxiong-wechat',
    }
    try:
        r = requests.post(apiUrl, data=data).json()
        return r.get('text')
    except:
        return


@itchat.msg_register(itchat.content.TEXT)
def tuling_reply(msg):
    defaultReply = '不好意思耳朵不好使，你是说' + msg['Text'] + '???'
    reply = get_response(msg['Text'])
    return reply or defaultReply


itchat.auto_login(hotReload=True)
itchat.run()
