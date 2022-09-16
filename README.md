# remote_filesystem_service(RFS)
This repo contains code for server-side and client-side application including common header files.

1. "send_receive.h" and "encrypt_decrypt.h" header files are included by both server and client application.
2. For running server application:

  Compile "server_interface.c" inside server_folder on server machine using following command
    `gcc server_interface.c -o server`
    
  Run server by executing `server` with available port with following command.
    `./server <port-number>`
    
3. For running client application:

  Compile "client_interface.c" inside client_folder on client machine using following command
    `gcc client_interface.c -o client`
    
  Run client by executing `client` with server-ip and socket-port with following command.
    `./client <server-ip> <port-number>`
    
4. List of commands:

  `HELP` -- get help.
  `EXIT` -- exit client application.
  `CWD` -- get current working directory.
  `LS` -- get list of files and directories.
  `CD <path>` -- change cwd to path.
  `DWD <filename>` -- download filename file from server.
  `UPD <filename>` -- upload filename file to server.
