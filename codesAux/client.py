import socket
HOST = 'localhost'     # Endereco IP do Servidor
PORT = 8888            # Porta que o Servidor esta
tcp = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
dest = (HOST, PORT)
tcp.connect(dest)
print 'Para sair use CTRL+X\n'

tcp.send ("oi")
print(tcp.recv(1024).decode("ascii"))
tcp.close()

