# kwd_server
This project create a server running on Linux.

The basci model of this server is a Reactor model, which includes one Reactor
and multiple work threads.

Working threads will process messages come from clients and Reactor will return 
possible results back to clients.
