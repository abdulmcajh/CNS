gnome-terminal -e "python server.py 23455 23457 23458"
sleep 0.25
gnome-terminal -e "python leader.py 23455 A"
sleep 0.25
gnome-terminal -e "python leader.py 23455 B"
sleep 0.25
gnome-terminal -e "python normal.py 23455 A"
sleep 0.25
gnome-terminal -e "python normal.py 23455 B"

