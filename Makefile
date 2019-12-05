client:
	gcc scan_clnt.c scan_client.c scan_xdr.c -o scan_client

server:
	gcc scan_proc.c scan_svc.c scan_xdr.c -o scan_server

