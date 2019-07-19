#依赖库：
1. [log4cxx](https://github.com/apache/logging-log4cxx.git)

	```
	$ ./configure --with-charset=utf-8 --with-logchar=utf-8 # 设置utf-8编码
	$ make
	$ make install
	```

2. [libiconv](https://www.gnu.org/software/libiconv/#TOCdownloading)

	目前使用[1.16](https://ftp.gnu.org/pub/gnu/libiconv/libiconv-1.16.tar.gz)版本

	```
	$ ./configure --prefix=/usr/local
	$ make
	$ make install
	```

3. libssl

	```
	$ yum install openssl-devel # centos安装命令
	```

4. libcurl

	```
	$ yum install libcurl-devel # centos安装命令
	```

5. expat

	```
	$ yum install expat-devel # centos安装命令
	```

6. [MySQL Connector C 6.1](https://downloads.mysql.com/archives/c-c/)

	下载[mysql-connector-c-6.1.11-linux-glibc2.12-x86_64.tar.gz](https://downloads.mysql.com/archives/get/file/mysql-connector-c-6.1.11-linux-glibc2.12-x86_64.tar.gz)后解压到`/usr/local/`目录下：

	```
	$ tar xvf mysql-connector-c-6.1.11-linux-glibc2.12-x86_64.tar.gz /usr/local/mysql
	```