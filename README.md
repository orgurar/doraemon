# Doraemon

![](logo.png)

## Name

Doraemon (Japanese: ドラえもん [doɾaemoɴ]) is a Japanese manga series written and illustrated by Fujiko F. Fujio.

## Description

Doraemon is a Command & Control system for Windows written in Python (server) and C++ (agent).

## Getting Started

### Prerequisites

Doraemon Server is written in the latest version on python (currently v3.9.6) and it is necessary to start the server.

- [Python](https://www.python.org/) - a programming language that lets you work quickly
  and integrate systems more effectively.

### Installation

In order to get Doraemon system up and running, first use `git clone --recurse-submodules https://gitlab.com/orgurar/doraemon.git` to clone your local repo.

#### Server

To reach the server subdirectory, just use `cd server` command in your cmd.

Start the server by using the following commands

```
# install python requirements
pip install -r requirements.txt

# start server with the provided run.py script
python run.py
```

#### Agent

All you have to do is run the `agent.exe` file located in the `server/bin` folder.

If you would like to run the agent on your local machine, you can download the file and start the agent.

In addition, in order to run the agent on another computer (must be in you subnetwork), you will have to open a python http server on your computer and download the agent manualy, you are welcome to be helped from the following code.

```
# in cmd, run the following command to create an http server locally
python -m http.server 8000 # or chose a different port
```

then, from your browser, perform an `HTTP GET` request to
`http://<server_local_ip>/server/bin/agent.exe` and the agent file will be automatcally downloaded

## Usage

Doraemon meant for educational purposes only! you can take this guide to the limits but please, don't you dare to do anything to hurt users in your local enviroment, because I know you already thought about it (Well... most of you)

## Roadmap

Doraemon system is far from being finished, in the future I will add some features to improve agent's security and behaviour, as will as an SSL layer over the communication sockets on both agent and server sides.

## Authors and acknowledgment

- **Or Gur Arie** - Creator and Maintainer - [orgurar](https://gitlab.com/orgurar)

## License

Doraemon is licensed under the MIT license, see the [LICENSE](LICENSE) file for details.
