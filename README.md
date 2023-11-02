# robotic-arm-esp-8266

Implementação de um braço robótico inteligente que recebe comandos de uma página web para a movimentação, o projeto é composto por uma esp8266, um servidor web e um servidor websocket, o servidor web conecta no servidor websocket e envia os comandos, assim o websocket envia os comandos para a esp-8266, o projeto foi feito usando Arduino IDE para a programação da esp por pedido do professor

O servidor web é feito em React (https://github.com/mariarezende07/robotic_interface) e foi feito a partir de uma fork do projeto https://github.com/glumb/robot-gui 
