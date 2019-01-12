import socket, sys,time,os


def sleepSecond(s):
    print "sleep ",s,"second(s)"
    time.sleep(s)

def server(ip, port):
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.bind((ip, int(port)))
    s.listen(8)
    conn,addr=s.accept()

    print "get a connection:",addr
    print "close this connection after 15 seconds\n "
    sleepSecond(15)

    conn.close()    
    s.close()

def forkServer(ip,port):
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.setsockopt(socket.SOL_SOCKET,socket.SO_REUSEADDR,1)
    s.bind((ip, int(port)))

    pid=os.fork()
    if 0==pid:
        print "child pid:",os.getpid()
        s.listen(8)
        while(1):
            conn,addr=s.accept()
            print "child1 get a connection:",addr
            conn.close()
        s.close()

    else:
        pid=os.fork()
        if 0==pid:
            print "child2 pid:",os.getpid()
            s.listen(8)
            while(1):
                conn,addr=s.accept()
                print "child2 get a connection:",addr
                conn.close()
            s.close()
            
        else:
            print "parent pid:",os.getpid()
            s.listen(8)
            while(1):
                conn,addr=s.accept()
                print "parent get a connection:",addr
                conn.close()
            s.close()

def forkServer2(ip,port):
    pid=os.fork()
    if 0==pid:
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.setsockopt(socket.SOL_SOCKET,socket.SO_REUSEADDR,1)
        s.bind((ip, int(port)))
        s.listen(8)
        print "child pid:",os.getpid()
        while(1):
            conn,addr=s.accept()
            print "child1 get a connection:",addr
            conn.close()
        s.close()
    else:
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.setsockopt(socket.SOL_SOCKET,socket.SO_REUSEADDR,1)
        s.bind((ip, int(port)))
        s.listen(8)
        print "child pid:",os.getpid()
        while(1):
            conn,addr=s.accept()
            print "child1 get a connection:",addr
            conn.close()
        s.close()



def server2(ip,port):
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.bind((ip, int(port)))
    s.listen(8)
    while(1):
        conn,addr=s.accept()
        print "get a connection:",addr

        print "send message to client"
        conn.send("I get you");

        # data=conn.recv(4096)
        # print data

        print "close this connection after 3 seconds "
        sleepSecond(3)
        conn.close()
        print "we have close this connection:",addr
        print "\n"
    s.close()

def webServer(ip,port):
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.bind((ip, int(port)))
    s.listen(8)

    conn,addr=s.accept()
    print addr
    data=conn.recv(4096)
    print data


    h = "HTTP/1.0 200 OK\r\n"
    c = "%sContent-Type: text/html\r\n\r\n"
    html = "hello world"
    res = h + c + html
    conn.send(res)

    sleepSecond(3)
    conn.close()    
    s.close()

def fcgiServer(ip,port):
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.bind((ip, int(port)))
    s.listen(8)

    conn,addr=s.accept()
    print 'client address:',addr


    data=conn.recv(4096)
    print 'recv client data:',data

    h = "HTTP/1.0 200 OK\r\n"
    c = "%sContent-Type: text/html\r\n\r\n"
    html = "hello world"
    res = h + c + html
    conn.send(res)

    sleepSecond(3)
    conn.close()    
    s.close()

if __name__ == "__main__":
    fcgiServer(sys.argv[1], sys.argv[2])
