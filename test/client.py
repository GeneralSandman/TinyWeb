import socket, sys,time


def client(localport,ip, port):
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.bind(("192.168.1.6", int(localport)))
    s.connect((ip, int(port)))
    print 'server address:', s.getpeername()
    print "sleep 7 seconds" 
    time.sleep(7)
    print "send message"
    s.send("I'm client")
    # buf=s.recv(30)
    # print buf
    print "sleep 7 seconds" 
    time.sleep(7)
    print "close connection"
    s.close()


if __name__ == "__main__":
    client(sys.argv[1], sys.argv[2], sys.argv[3])
