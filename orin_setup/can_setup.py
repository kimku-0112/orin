import can
import subprocess

def configure_can_fd(interface='can0', bitrate='500000', dbitrate='500000'):
    # Load necessary CAN modules (if not already loaded)
    subprocess.run(['sudo', 'modprobe', 'can'])
    subprocess.run(['sudo', 'modprobe', 'can_raw'])
    subprocess.run(['sudo', 'modprobe', 'can_fd'])

    # Bring down the interface
    subprocess.run(['sudo', 'ip', 'link', 'set', interface, 'down'])

    # Set the interface up with CAN-FD settings
    subprocess.run(['sudo', 'ip', 'link', 'set', interface, 'type', 'can', 
                    'bitrate', bitrate, 'dbitrate', dbitrate, 'fd', 'on'])

    # Bring up the interface
    subprocess.run(['sudo', 'ip', 'link', 'set', interface, 'up'])

if __name__ == "__main__":
   configure_can_fd()	
