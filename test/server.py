import socket, sys,time


def sleepSecond(s):
    print "sleep ",s,"second(s)"
    time.sleep(s)

def server(ip, port):
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.bind((ip, int(port)))
    s.listen(8)

    conn,addr=s.accept()
    print "get a connection:",addr
    sleepSecond(10)

    conn.close()    
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


if __name__ == "__main__":
    server(sys.argv[1], sys.argv[2])
