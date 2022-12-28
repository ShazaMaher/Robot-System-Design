
# Static IP Address
The IP address of the raspberry pi is:
192.168.0.10

# Raspberry pi user_name and password
The user name of the raspberry pi is :"pi"
The passward of the raspberry pi is : "rsd2019"

## MySQL Setup
__Install Flask:__ 
```
pip install flask-mysql
```
```
pip install Flask
```

__Install MySQL + server + client:__ 

Follow guide:

https://virtualzero.net/blog/install-mysql-for-a-flask-app-on-ubuntu-18.04-lts?fbclid=IwAR0VXTb00Y2FlC2AnvqPOvaTX47iCIeiwdYawAA0CEpgagtkeN1LlWBM_Fo

__Start server:__ 
```
/etc/init.d/mysql start 
```

## Database setup
__Start MySQL shell:__ 
```
sudo mysql -u root -p
```
__Create user and grant privileges:__ 

```
mysql> CREATE USER 'rsd'@'localhost' IDENTIFIED BY 'rsd2018'; 
```
```
mysql> GRANT ALL PRIVILEGES ON * . * TO 'rsd'@'localhost';
```
```
mysql> FLUSH PRIVILEGES;
```

__Create rsd2018 database:__ 

Follow guide:

https://www.a2hosting.com/kb/developer-corner/mysql/managing-mysql-databases-and-users-from-the-command-line?fbclid=IwAR1KRui_KnS9M97hUWaNybHGq7NDSIjtMLreeTkBdkyYzyalDJE5QjDHFxo

__Import predefined table:__ 
```
mysql> USE rsd2018;
```
```
mysql> source *filepath*/db_export.sql;
```
## Additional guides
https://www.digitalocean.com/community/tutorials/how-to-create-a-new-user-and-grant-permissions-in-mysql?fbclid=IwAR2KL8Rf09W_BXk_Nlk9u5rJJtRhXN5oVJlzG1JRuKVvVZP3BfizztdkQrU




##PACKML
Skeleton set up in packml_ros_node.cpp. The rest is taken from https://github.com/ros-industrial-consortium/packml.
