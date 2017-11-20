all: clear swimLane pellet fish

clear:
	rm -rf *o swimLane pellet fish

swimLane:
	gcc -o swimlane swimlane.c

pellet:
	gcc -o pellet pellet.c

fish:
	gcc -o fish fish.c
