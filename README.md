# SMTP Test Prototype

Тестовий SMTP сервер і клієнт на C++17. Мінімальна реалізація для перевірки SMTP flow згідно RFC 821.

## Збірка

### CLion
Додай в `CMakeLists.txt`:
```cmake
add_executable(test_smtp_server server/server.cpp)
add_executable(test_smtp_client client/client.cpp)

if(WIN32)
    target_link_libraries(test_smtp_server PRIVATE ws2_32)
    target_link_libraries(test_smtp_client PRIVATE ws2_32)
endif()
```

### Вручну (MinGW / Windows)
```bash
g++ -std=c++17 -o server server.cpp -lws2_32
g++ -std=c++17 -o client client.cpp -lws2_32
```

### Linux
```bash
g++ -std=c++17 -o server server.cpp
g++ -std=c++17 -o client client.cpp
```

## Запуск

1. Запусти сервер — він слухає на `127.0.0.1:25000`
2. Запусти клієнт — він підключиться, відправить тестовий лист і завершиться

Або підключись вручну через telnet:
```
telnet 127.0.0.1 25000
```

## SMTP Flow

```
S: 220 TestSMTP Ready           <- сервер вітає клієнта
C: HELO localhost                <- клієнт представляється
S: 250 Hello localhost
C: MAIL FROM:<vlad@test.com>     <- від кого лист
S: 250 OK sender
C: RCPT TO:<friend@test.com>     <- кому лист
S: 250 OK recipient
C: DATA                          <- початок тіла листа
S: 354 Start input...
C: Subject: Test                 <- хедери + тіло
C: Hello!
C: .                             <- крапка = кінець тіла
S: 250 OK message accepted
C: QUIT                          <- завершення сесії
S: 221 Bye
```

## Підтримувані команди

| Команда | Опис |
|---------|------|
| HELO / EHLO | Привітання, початок сесії |
| MAIL FROM:\<addr\> | Адреса відправника |
| RCPT TO:\<addr\> | Адреса отримувача (можна кілька разів) |
| DATA | Початок введення тіла листа |
| QUIT | Завершення зʼєднання |
| RSET | Скидання стану сесії |
| NOOP | Нічого не робить (keepalive) |

## Коди відповідей

| Код | Значення |
|-----|----------|
| 220 | Сервер готовий |
| 221 | Закриття зʼєднання |
| 250 | OK, команда виконана |
| 354 | Вводи тіло листа |
| 500 | Невідома команда |
| 503 | Неправильна послідовність команд |

## Стейт-машина сервера

```
INIT ──HELO──> GREETED ──MAIL FROM──> MAIL ──RCPT TO──> RCPT ──DATA──> DATA
  │                                                                      │
  │                              <────────────── "." (кінець) ───────────┘
  │                              GREETED (готовий до наступного листа)
  │
  └──QUIT──> зʼєднання закрито
```

## Структура

```
server/server.cpp   — SMTP сервер (TCP listener + обробка команд)
client/client.cpp   — SMTP клієнт (підключення + відправка листа)
```

## Крос-платформеність

Код працює на Windows (Winsock2) і Linux (POSIX sockets). Різниця прихована за `#ifdef _WIN32`.