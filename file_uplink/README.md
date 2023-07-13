These codes are from reference

How to installations?
in server
gcc -o server server.c
---------
in client
gcc -o client client.c
---------
How to run?
in server (do first, echo & file & packet_scheduler & path_manager & subflows_info)
./server [port number]
----------
in client (echo)
./client [server address] [port number]
-----------
in client (file & packet_scheduler & path_manager & subflows_info)
./client [server address] [port number] [file path]
--------
