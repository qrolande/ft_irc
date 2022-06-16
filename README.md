ft_irc

IRC (англ. Internet Relay Chat) — протокол прикладного уровня для обмена сообщениями в режиме реального времени.
Разработан в основном для группового общения, также позволяет общаться через личные сообщения и обмениваться данными, в том числе файлами.
IRC использует транспортный протокол TCP и криптографический TLS (опционально).
Ввиду технической простоты реализации протокол IRC в свое время стал использоваться при организации ботнетов в качестве средства передачи управляющих команд компьютерам - участникам ботнета от владельца.
Командный проект


## How to run server
Use **make** rule to compile the program. Run it with port and password
``` 
make && ./ircserv [port] [password]
``` 
to connect -> nc localhost [port] -> then you need to enter: 
```
PASS [password]

NICK [any nickname]

USER 1 2 3 4 (this 4 arguments is needed to finish registration)
```
send message:
```
PRIVMSG [nick] : any text
```

## How to run bot
```
cd bot && make
```
```
./bot [port] [password]
```
