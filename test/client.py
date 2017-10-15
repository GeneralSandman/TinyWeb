import socket, sys,time


def sleepSecond(s):
    print "sleep ",s,"second(s)"
    time.sleep(s)

def client(localport,ip, port):
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.bind(("127.0.0.1", int(localport)))
    s.connect((ip, int(port)))

    print 'server address:', s.getpeername()

    sleepSecond(3)
    print "send message:I'm client"
    s.send("I'm client")

    sleepSecond(1)    
    buf=s.recv(100)
    print buf

    sleepSecond(3)
    print "close connection"
    s.close()


if __name__ == "__main__":
    client(sys.argv[3], sys.argv[1], sys.argv[2])
