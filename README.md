# home-automation
IoT project for home automation

# Installation

  ```
  chmod +x main.sh
  chmod +x webcam.sh
  ```
# Raspberry Setup
To install InfluxDB add Influx repositories to apt:
  ```
 wget -qO- https://repos.influxdata.com/influxdb.key | sudo apt-key add -
source /etc/os-release
echo "deb https://repos.influxdata.com/debian $(lsb_release -cs) stable" | sudo tee /etc/apt/sources.list.d/influxdb.list
  ```
Upadate apt and install:
  ```
  sudo apt update && sudo apt install -y influxdb
  ```
Start the influxdb service and set it to run at boot:
  ```
  sudo systemctl unmask influxdb.service
  sudo systemctl start influxdb
  sudo systemctl enable influxdb.service
  ```
Run the inlux client with `influx` and create a user:
  ```
  create database '<DBnamehere>'
  use '<DBnamehere>'

  create user '<usernamehere>' with password '<passwordhere>' with all privileges
  grant all privileges on '<DBnamehere>' to '<usernamehere>'

  show users

  user admin
  ---- -----
  '<usernamehere>' true
  ```
To install Grafana add repositories to apt:
  ```
  wget -q -O - https://packages.grafana.com/gpg.key | sudo apt-key add -
  echo "deb https://packages.grafana.com/oss/deb stable main" | sudo tee /etc/apt/sources.list.d/grafana.list
  ```
Update and install:
  ```
  sudo apt update && sudo apt install -y grafana
  ```
Enable the service and set to run at boot:
  ```
  sudo systemctl unmask grafana-server.service
  sudo systemctl start grafana-server
  sudo systemctl enable grafana-server.service
  ```
Grafana is now running on the machine and is accessible from any device on the local network.

Open a browser and go to http://'ipaddress':3000, where the IP address is the address that you used to connect to the Raspberry Pi.

Access the Grafana login page with the default username admin, and the default password admin.

Create a dashboard adding the graphs you need, remembering to select Influx as Grafana's data source. 
  
  
  
  
  
  
