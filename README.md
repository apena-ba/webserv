# WebServ
## Description
Welcome to our HTTP 1.1 Web Server!

This server was made for high availability taking Nginx as reference. It has some features like:
- Displaying static and dynamic web pages using cgi
- Hosting multiple servers in one system port
- GET, DELETE and POST request handling

This server was developed for MacOs Ventura

## Usage
### Set-Up
1. Open a terminal
2. Clone the repository

`git clone github.com/apena-ba/webserv/webserv.git <path-to-clone>`

3. Set up the configuration

To set the server up, its necessary to specify the path for the basic pages and cgi into server.conf

### Building
To build the server, run 'make' command into your terminal

### Running
After the project is compiled, run the server with:

`./webserv <path-to-server.conf>`
