import can
from time import sleep 

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
    
def send_can_fd_msg(channel='can0', message_id=0x640, data=b'RCWS CAN TE CAN-FD!'):


    # Configure the bus with the channel and fd mode
    bus = can.interface.Bus(channel=channel, bustype='socketcan', fd=True)

    # Create a CAN message with extended ID, data and bitrate switch for FD
    msg = can.Message(arbitration_id=message_id, is_extended_id=True,data=data, is_fd=True,bitrate_switch=True)

    # Send the message
    try:
        bus.send(msg)
        print("Message sent on {}".format(bus.channel_info))
    except can.CanError:
        print("Message failed to send")

if __name__ == "__main__":
    configure_can_fd()	
    for i in range (100):
   	    send_can_fd_msg(channel='can0', message_id=0x500+i,data=b'RCWS CAN T%d CAN-FD!' %i)
   	    sleep(0.033)
     

