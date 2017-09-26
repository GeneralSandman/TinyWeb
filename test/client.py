import socket, sys,time


def client(ip, port):
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.bind(("127.0.0.1", 1234))
    s.connect((ip, int(port)))
    time.sleep(5)
    print 'peeraddress:', s.getpeername()
    info="I'm client"
    s.send(info)
    buf=s.recv(30)
    print buf
    s.close()


if __name__ == "__main__":
    client(sys.argv[1], sys.argv[2])
