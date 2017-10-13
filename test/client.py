import socket, sys,time


def client(localport,ip, port):
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.bind(("127.0.0.1", int(localport)))
    s.connect((ip, int(port)))
    print 'server address:', s.getpeername()
    print "sleep 3 seconds" 
    time.sleep(3)
    print "send message:I'm client"
    s.send("I'm client")
    time.sleep(1)
    buf=s.recv(30)
    print buf
    print "sleep 3 seconds" 
    time.sleep(3)
    print "close connection"
    s.close()


if __name__ == "__main__":
    client(sys.argv[1], sys.argv[2], sys.argv[3])
