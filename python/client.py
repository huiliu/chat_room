#!/usr/bin/env python
# -*- coding: utf-8 -*-

import time
import socket
import threading
from MessageID_pb2 import *
from ChatRoom_pb2 import *
from ConnectionInfo_pb2 import *
from Login_pb2 import *
from UserData_pb2 import *
from version_pb2 import *

class Session:
    """docstring for Session"""
    def __init__(self, arg):
        self.arg = arg
        

class User:

    """Docstring for User. """

    def __init__(self):
        """TODO: to be defined1. """
        

class Connection:
    def __init__(self, endpoint):
        self.s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        assert type(endpoint) == type(())
        self.s.connect(endpoint)

    def start(self, subject):
        # 是不是应该使用线程
        isFirstPacket = True
        while True:
            data = self.s.recv(1024)
            if isFirstPacket:
                isFirstPacket = False
                print(data)
                continue

            raw = RawMessage()
            raw.ParseFromString(data)
            subject.Notify(raw)

    def SendPackect(self, uid, msg):
        raw = RawMessage()
        raw.id = msg.msg_id
        raw.uid = uid
        raw.clientId = 0
        raw.timestamp = int(time.time())
        raw.strMsg = msg.SerializeToString()

        self.s.send(raw.SerializeToString())


        
class iSubject:
    def __init__(self):
        self.Subscribers = dict()
        
    def Register(self, MsgId, Subscriber):
        if MsgId not in self.Subscribers:
            self.Subscribers[MsgId] = [Subscriber]
        else:
            self.Subscribers[MsgId].append(Subscriber)

    def Unregister(self, MsgId, Subscriber):
        if MsgId in self.Subscribers and Subscriber in self.Subscribers[MsgId]:
            self.Subscribers[MsgId].remove(Subscriber)

    def Notify(self, Msg):
        if Msg.id in self.Subscribers:
            for sub in self.Subscribers[Msg.id]:
                sub.HandleMessage(Msg)

class iObserver:
    def __init__(self):
        pass

    def HandleMessage(self, Msg):
        assert False
        pass

class MessageDispatcher(iSubject):
    def __init__(self):
        iSubject.__init__(self)

class UserManager(iObserver):
    def __init__(self, subject, conn):
        iObserver.__init__(self)
        self.subject = subject
        self.connection = conn


    def __del__(self):
        pass


    def HandleMessage(self, data):
        msg_id = data.id
        pass
        

class ChatroomManager(iObserver):

    def __init__(self, subject, conn):
        iObserver.__init__(self)
        self.subject = subject
        self.connection = conn

        self.subject.Register(MSG_NOTIFY_CHATROOM_LIST, self)
        self.subject.Register(MSG_NOTIFY_CREATE_CHATROOM_RESULT, self)
        self.subject.Register(MSG_NOTIFY_JOIN_CHATROOM_RESULT, self)
        self.subject.Register(MSG_NOTIFY_QUIT_CHATROOM_RESULT, self)
        self.subject.Register(MSG_NOTIFY_CHATROOM_SPEAK, self)
        self.subject.Register(MSG_NOTIFY_SPEAK_FAILED, self)

    def __del__(self):

        self.subject.Unregister(MSG_NOTIFY_CHATROOM_LIST, self)
        self.subject.Unregister(MSG_NOTIFY_CREATE_CHATROOM_RESULT, self)
        self.subject.Unregister(MSG_NOTIFY_JOIN_CHATROOM_RESULT, self)
        self.subject.Unregister(MSG_NOTIFY_QUIT_CHATROOM_RESULT, self)
        self.subject.Unregister(MSG_NOTIFY_CHATROOM_SPEAK, self)
        self.subject.Unregister(MSG_NOTIFY_SPEAK_FAILED, self)

    def HandleMessage(self, data):
        msg_id = data.id

        if MSG_NOTIFY_CHATROOM_LIST == msg_id:
            self._HandleNotifyChatroomList(data)
            pass
        elif MSG_NOTIFY_CREATE_CHATROOM_RESULT == msg_id:
            self._HandleNotifyCreateChatroomResult(data)
            pass
        elif MSG_NOTIFY_JOIN_CHATROOM_RESULT == msg_id:
            self._HandleNotifyJoinChatroomResult(data)
            pass
        elif MSG_NOTIFY_QUIT_CHATROOM_RESULT == msg_id:
            self._HandleNotifyQuitChatroomResult(data)
            pass
        elif MSG_NOTIFY_CHATROOM_SPEAK == msg_id:
            self._HandleNotifyChatroomSpeak(data)
            pass
        elif MSG_NOTIFY_SPEAK_FAILED == msg_id:
            self._HandleNotifyChatroomSpeakFailed(data)
            pass
        else:
            assert False

    def _HandleNotifyChatroomList(self, data):
        pass

    def _HandleNotifyCreateChatroomResult(self, data):
        pass

    def _HandleNotifyJoinChatroomResult(self, data):
        pass

    def _HandleNotifyQuitChatroomResult(self, data):
        pass

    def _HandleNotifyChatroomSpeak(self, data):
        pass

    def _HandleNotifyChatroomSpeakFailed(self, data):
        pass

class SessionManager(iObserver):
    """docstring for SessionManager"""
    def __init__(self, subject, conn):
        iObserver.__init__(self)
        self.subject = subject
        self.connection = conn

        self.subject.Register(MSG_CHECK_VERSION, self)
        self.subject.Register(MSG_NOTIFY_LOGIN_RESULT, self)

    def __del__(self):

        self.subject.Unregister(MSG_CHECK_VERSION, self)
        self.subject.Unregister(MSG_NOTIFY_LOGIN_RESULT, self)

    def HandleMessage(self, data):
        msg_id = data.id

        if MSG_CHECK_VERSION == msg_id:
            self._HandleCheckVersion(data)
            pass
        elif MSG_NOTIFY_LOGIN_RESULT == msg_id:
            self._HandleLoginResult(data)
            pass
        else:
            assert False

    def _HandleCheckVersion(self, data):
        notify = NotifyVersionCheck()
        notify.ParseFromString(data.strMsg)

        if NVCR_FAILED == notify.result:
            print("版本验证失败！")
            return
        elif NVCR_SUCCESS == notify.result:
            print("版本验证成功！")

            reqLogin = ReqLogin()
            reqLogin.username = "liuhui"
            reqLogin.password = '123456'

            self.connection.SendPackect(1, reqLogin)

    def _HandleLoginResult(self, data):
        loginResult = NotifyLoginResult()
        loginResult.ParseFromString(data.strMsg)

        if NLR_FAILED == loginResult.result:
            print("登陆失败！")
            return
        elif NLR_SUCCESS == loginResult.result:
            print("登陆成功!")

        
def main():
    conn = Connection (('127.0.0.1', 9095))

    messageDispatcher = MessageDispatcher();

    userMgr = UserManager(messageDispatcher, conn)
    chatroomMgr = ChatroomManager (messageDispatcher, conn)
    sessionMgr = SessionManager (messageDispatcher, conn)

    conn.start(messageDispatcher)

if __name__ == '__main__':
    main()
