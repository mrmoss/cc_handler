#!/bin/bash
openssl req -x509 -nodes -days 365 -newkey rsa:4096 -keyout my.key -out my.crt
chmod 644 my.crt
chmod 600 my.key
