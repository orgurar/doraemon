from src.threaded_server import ThreadedServer


# DEBUG: decide whether server should use ssl over http or not
USE_SSL = False

## TODO: read from .env file
HOST = "192.168.1.25"
PORT = 443 if USE_SSL else 8080



def run() -> None:
    # creating and starting a multithreaded secure socket server
    secure_server = ThreadedServer(HOST, PORT, use_ssl=USE_SSL)
    secure_server.listen()



if __name__ == "__main__":
    run()