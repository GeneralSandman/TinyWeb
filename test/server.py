import socket, sys,time


def sleepSecond(s):
    print "sleep ",s,"second(s)"
    time.sleep(s)

def server(ip, port):
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.bind((ip, int(port)))
    s.listen(8)

    conn,addr=s.accept()
    print addr
    data=conn.recv(4096)
    print data

    conn.close()    
    s.close()

if __name__ == "__main__":
    server(sys.argv[1], sys.argv[2])
