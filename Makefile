server: example-server.c networking.c networking.h
	gcc example-server.c networking.c -oserver -std=c89 -pedantic -Wall -Wextra

echo: echo-server.c networking.c networking.h
	gcc echo-server.c networking.c -oecho -std=c89 -pedantic -Wall -Wextra


client: example-client.c networking.c networking.h
	gcc example-client.c networking.c -oclient -std=c89 -pedantic -Wall -Wextra
