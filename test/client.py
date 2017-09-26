import socket, sys


def client(ip, port):
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.bind(("127.0.0.1", 1234))
    print 'localaddress:', s.getsockname()
    s.connect((ip, int(port)))
    print 'peeraddress:', s.getpeername()
    buf=s.recv(15)
    print buf
    s.close()


if __name__ == "__main__":
    client(sys.argv[1], sys.argv[2])
