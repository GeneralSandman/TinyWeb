import socket, sys,time


def sleepSecond(s):
    print "sleep ",s,"second(s)"
    time.sleep(s)

def client1(localport,ip, port):
    lport=int(localport)
    while(1):
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.setsockopt(socket.SOL_SOCKET,socket.SO_REUSEADDR,1)
        s.bind(("127.0.0.1", lport))
        lport+=1
        s.connect((ip, int(port)))
        sleepSecond(1)
        s.close()

def client(localport,ip, port):
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.bind(("127.0.0.1", int(localport)))
    s.connect((ip, int(port)))

    print 'server address:', s.getpeername()

    sleepSecond(3)
    print "send message:I'm client"
    s.send("I'm client")

    buf=s.recv(100)
    print buf

    sleepSecond(3)
    print "close connection"
    s.close()

def webClient(localport,ip,port=9999):
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.bind(("127.0.0.1", int(localport)))
    s.connect((ip, int(port)))
    print 'server address:', s.getpeername()

    r1 = "GET /index.html HTTP/1.1\r\n"
    r2 = "host: localhost:9999\r\n"
    r3="connection: Close\r\n\r\n"
    r=r1+r2+r3

    # for num in range(1,1000):
    s.send(r)
    buf =s.recv(5000)
    print len(buf)

    sleepSecond(12)
        # print num
    s.close()



if __name__ == "__main__":
    
    client1(sys.argv[3], sys.argv[1], sys.argv[2])
