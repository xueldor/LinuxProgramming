sin_port = htons(9999)  
开始用的htonl,表面运行正常，实际上没有监听9999端口。因为sin_port是16位的整形，故应该用htons。