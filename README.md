# home-automation
This is an IoT project for home automation that include an Arduino and a Raspberry working together.  
The Arduino Uno collects data from sensors and can start many smart routine.  
The Raspberry shows the arduino data thanks to InfluxDB and Grafana. It also works as an alarm system,
when a movement is detected from the motion sensor, it takes a picture and sends it, with an advice, to your telegram bot.

## Get Started
* [Installation](#Installation)
* [Raspberry Setup](#Raspberry-Setup)
* [Arduino Setup](#Arduino-Setup)

## Installation

  ```
  chmod +x webcam.sh
  ```
  ```
  pip install psutil && chmod +x rpi-stats.py
  ```
  
  
## Raspberry Setup
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
  
## Arduino Setup
Connect the DTH11 to Digital PIN2 and the InfraRed sensor to the 3rd PIN.  
Load the sketch and connect the arduino directly to the raspberry.  
Obviously you can add any sensor you want,remembering to update the files to make them capable to read the data and show them on Grafana.
  
