Usage: ft_ping [OPTION...] HOST ...

Available options :

-?, --?			give this help list
-v, --v		    verbose mode


[BONUS]

-V, --V			print program version
-c, --c			stop after sending NUMBER packets
-i, --i			wait NUMBER seconds between sending each packet
-w, --w			stop after N seconds
-s, --s			send NUMBER data octets
-ttl, --ttl		specify N as time-to-live
-rdns, --rdns	convert ip address to string address

Pour tester l'option : w

time ping -c 1 -W 1 10.255.255.1
time ping -c 1 -W 5 10.255.255.1