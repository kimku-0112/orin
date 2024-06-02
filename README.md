# orin setup installation
```
git clone https://github.com/kimku-0112/orin.git
cd orin/orin_setup
sudo ./orin.sh
sudo ./cp_file.sh
```

# orin opencv Installation
```
wget https://github.com/Qengineering/Install-OpenCV-Jetson-Nano/raw/main/OpenCV-4-9-0.sh
sudo chmod 755 ./OpenCV-4-9-0.sh
./OpenCV-4-9-0.sh
```

# boost library installation
```
wget https://archives.boost.io/release/1.85.0/source/boost_1_85_0.tar.gz
tar -xvf boost_1_85_0.tar.gz
cd boost_1_85_0
./bootstrap.sh
sudo ./b2 install 
```

# nlohmann json library installation
```
sudo apt-get update
sudo apt-get install nlohmann-json3-dev
```
