     ------------------------------------------------------------------------------
      How to run:
      simply run make from both server's and client's directory
      ./server
      ./client 127.0.0.1
      -----------------------------------------------------------------------------
      FTP_server.cpp
      creates the socket on defined port and starts listening on it.
      after receiving connection it creates a child process by using fork, 
      copies the file descriptor of command channel to stdin for simplicity 
      and runs the executable serve which handles further requests from the client.
      -----------------------------------------------------------------------------
      serve.cpp
      it sets up the data channel and handles the further requests from the client
      -----------------------------------------------------------------------------
      FTP_client.cpp
      establishes the command channel and data channel to server.
      reads the commands from stdin and handles them accordingly.
      -----------------------------------------------------------------------------
