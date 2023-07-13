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

uplink에서는 client에서 file전송에 걸린 총 시간을 logging하도록 했는데, 현재 os권한으로 폴더만드는 코드는 못찾았다. 그래서 logging folder를 미리 만들어주지 않으면 로깅 파일이 저장이 안됨...
