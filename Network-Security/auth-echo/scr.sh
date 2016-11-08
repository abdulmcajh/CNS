gnome-terminal -e "python server.py 23444"
sleep 1
gnome-terminal -e "python client.py 23444"

