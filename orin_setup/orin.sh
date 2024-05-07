#!/bin/bash

# Makesure only root can run out script 
if (($EUID != 0));then
	echo "[ERROR] This script must be run as root"
	echo "[ERROR] please Run with 'sudo orin.sh'"
	exit
fi

# apt update upgrade 
echo 'apt update & upgrade'
apt update
apt upgrade -y

# qt install
echo 'qt install'
apt install -y qtbase5-dev qtchooser qt5-qmake qtbase5-dev-tools
apt install -y qt5-qmake
apt install -y qtcreator

# python pip install 
echo 'python pip install'
apt install -y python3-pip

# jtop install 
echo 'jtop install'
pip install -U jetson-stats

# chromium install 
echo "chromium install"
apt install -y chromium-browser

#vscode install 
cd /home/rcws
git clone https://github.com/JetsonHacksNano/installVSCode.git
cd installVSCode
./installVSCodeWithPython.sh
cd /home/rcws 
rm -rf installVSCode

# NOPASSWD setup
echo "nopasswd setup"
sudo echo "rcws ALL=NOPASSWD:ALL" >> /etc/sudoers

# can auto setup
echo "can auto setup"
su - rcws -c "pip install python-can"
mkdir -p /home/rcws/.config/autostart/
cp can_autosetup.desktop /home/rcws/.config/autostart/can_autosetup.desktop
cp can_setup.py /home/rcws/.can_setup.py

# VNC Server install & setup
echo "VNC Server install & setup"
echo "=================================="
echo "VNC Server password : rcws_h"
echo "=================================="
cd /usr/lib/systemd/user/graphical-session.target.wants
sudo ln -s ../vino-server.service ./.
cd /home/rcws
apt install -y vino
su - rcws -c "dbus-run-session gsettings set org.gnome.Vino prompt-enabled false"
su - rcws -c "dbus-run-session gsettings set org.gnome.Vino require-encryption false"
su - rcws -c "dbus-run-session gsettings set org.gnome.Vino authentication-methods \"['vnc']\""
su - rcws -c "dbus-run-session gsettings set org.gnome.Vino vnc-password $(echo -n rcws_h|base64)"

 #Serial setup
echo "serial setup"
sudo usermod -a -G dialout rcws
sudo chmod a+rw /dev/ttyTHS*
sudo touch /etc/udev/rules.d/99-ttyths.rules
sudo echo 'KERNEL="ttyTHS*",MODE="0666"' >> /etc/udev/rules.d/99-ttyths.rules
sudo touch /etc/udev/rules.d/99-ttyusb.rules
sudo echo 'KERNEL="ttyUSB*",MODE="0666"' >> /etc/udev/rules.d/99-ttyusb.rules


# reboot system
printf 'reboot system(y/n)? '
read answer
if [ "$answer" != "${answer#[Yy]}" ] ;then
    echo Yes
    sudo reboot
else
    echo No
fi
