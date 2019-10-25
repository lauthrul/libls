for test:
==========================================================================

cryptotool md5 123                  // -> 202cb962ac59075b964b07152d234b70
cryptotool sha1 123                 // -> 40bd001563085fc35165329ea1ff5c5ecbdbbeef
cryptotool sha224 123               // -> 78d8045d684abd2eece923758f3cd781489df3a48e1278982466017f
cryptotool sha256 123               // -> a665a45920422f9d417e4867efdc4fb8a04a1f3fff1fa07e998e86f7f7a27ae3
cryptotool sha384 123               // -> 9a0a82f0c0cf31470d7affede3406cc9aa8410671520b727044eda15b4c25532a9b5cd8aaf9cec4919d76255b6bfb00f
cryptotool sha512 123               // -> 3c9909afec25354d551dae21590bb26e38d53f2173b8d3dc3eee4c047e7ab1c1eb8b85103e3be7ba613b31bb5c9c36214dc9f14a42fd7a2fdb84856bca5c44c2

cryptotool base64_encode 123
cryptotool base64_decode MTIz

cryptotool url_encode "http://www.baidu.com/?key=ÄãºÃ£¬world!"
cryptotool url_decode "http%3A%2F%2Fwww.baidu.com%2F%3Fkey%3D%C4%E3%BA%C3%A3%ACworld%21"

cryptotool encoding_convert "http://www.baidu.com/?key=ÄãºÃ£¬world!" -F gb2312  -T utf-8
cryptotool encoding_convert "http://www.baidu.com/?key=ä½ å¥½ï¼Œworld!" -T gb2312  -F utf-8

cryptotool des_encrypt -m ecb -k l53d70p3 -p pkcs7 "http://www.baidu.com/?key=hello,world!" -of hex
cryptotool des_decrypt -m ecb -k l53d70p3 -p pkcs7 595915461467a6281cdc851002aa2641304cd040a564867afc3a611060cb8e4d58d9455be3ada970 -if hex -of raw
cryptotool des_encrypt -m cbc -k l53d70p3 -iv 12345678 -p pkcs7 "http://www.baidu.com/?key=hello,world!" -of hex
cryptotool des_decrypt -m cbc -k l53d70p3 -iv 12345678 -p pkcs7 957533a15438d28180af0b0d16a771c7907e46f6a1c20841ee1becbb7a32d9dbc21c78608d8d896a -if hex -of raw

cryptotool aes_encrypt -m ecb -k l53d70p3 -kl 128 -p pkcs7 "http://www.baidu.com/?key=hello,world!" -of hex
cryptotool aes_decrypt -m ecb -k l53d70p3 -kl 128 -p pkcs7 "4e4aad83670ced4d6ba4d4d0832c3dac90540d5c3bdf16afbcf6e68770de3a32a9bb9f5bf7bea50e4ae7f80a1868abd7" -if hex -of raw
cryptotool aes_encrypt -m cbc -k 12345678l53d70p3 -kl 128 -iv 12345678l53d70p3 -p pkcs7 "http://www.baidu.com/?key=hello,world!" -of hex
cryptotool aes_decrypt -m cbc -k 12345678l53d70p3 -kl 128 -iv 12345678l53d70p3 -p pkcs7 "181922f701165a28f4dd0c695ead9723454003b0d997e1e439eae9b080305e86340f38ca0a9d2d5371de6e01ef7bb141" -if hex -of raw

cryptotool 3des_encrypt -m ecb -k l53d70p312345678l53d70p3 -p pkcs7 "http://www.baidu.com/?key=hello,world!" -of hex
cryptotool 3des_decrypt -m ecb -k l53d70p312345678l53d70p3 -p pkcs7 d5069779e1b15228b804a9358c76c395374e8e999cf93552089e457736e563d3f9ff6441768e7947 -if hex -of raw
cryptotool 3des_encrypt -m cbc -k l53d70p312345678l53d70p3 -iv 12345678 -p pkcs7 "http://www.baidu.com/?key=hello,world!" -of hex
cryptotool 3des_decrypt -m cbc -k l53d70p312345678l53d70p3 -iv 12345678 -p pkcs7 0f3413df682773e1315fa85b050a5ca577579a8f032b78b1b4d73ad64140cc3464d29153c844ad61 -if hex -of raw

cryptotool md5 123 -o crypto/md5.out.txt
cryptotool sha1 123 -o crypto/sha1.out.txt
cryptotool sha224 123 -o crypto/sha224.out.txt
cryptotool sha256 123 -o crypto/sha256.out.txt
cryptotool sha384 123 -o crypto/sha384.out.txt
cryptotool sha512 123 -o crypto/sha512.out.txt

cryptotool aes_encrypt -m cbc -k 12345678l53d70p3 -kl 128 -iv 12345678l53d70p3 -p pkcs7 -f "c:\windows\win.ini" -o "crypto/win.ini.out.txt" -of hex
cryptotool aes_decrypt -m cbc -k 12345678l53d70p3 -kl 128 -iv 12345678l53d70p3 -p pkcs7 -f "crypto/win.ini.out.txt" -if hex -o "crypto/win.ini"

cryptotool 3des_encrypt -m cbc -k l53d70p312345678l53d70p3 -iv 12345678 -p pkcs7 -f "C:\Windows\System32\notepad.exe" -o "crypto/notepad.out.txt"
cryptotool 3des_decrypt -m cbc -k l53d70p312345678l53d70p3 -iv 12345678 -p pkcs7 -f "crypto/notepad.out.txt" -o "crypto/notepad.exe"

