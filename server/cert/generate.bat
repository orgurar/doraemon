# Or Gur Arie - self signed certificate

# generating the server's certificate and private key
"C:\Program Files\Git\usr\bin\openssl.exe" req -x509 -sha256 -nodes -days 365 -newkey rsa:2048 -keyout private.key -out certchain.pem