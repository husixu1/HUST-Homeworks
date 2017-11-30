#!/bin/bash

SPEED=50

main() {

	sleep 3
	info "start replacing"
	sleep 1
	xdotool key  --delay 200 'Ctrl+o'

	# confuse
	down 10
	nop  88

	info "replacing complete"
	# input
	F8 10
	enter 'test'
	key 'Return'
	# go to passcheck
	F8 88

	info "replacing passwd check"
	# skip passcheck
	nop 30
	F9 2

	info "start testing name"
	for i in `python generateKey.py`
	do
		key 1
		enter $i
		key 'Return'
		key 2
	done

	sleep 2

	info "name test finished"
	key 1
	enter 'ChenZh'
	key 'Return'
	key 2
	key 3
	key 4

}

down() {
	for i in `seq $1`
	do
		xdotool key --delay $SPEED 'Down'
	done
}

nop() {
	for i in `seq $1`
	do
		xdotool type --delay $SPEED 'nop'
		xdotool key --delay $SPEED 'Return'
	done
}

F8() {
	for i in `seq $1`
	do
		xdotool key --delay $SPEED 'F8'
	done
}

F9() {
	for i in `seq $1`
	do
		xdotool key --delay $SPEED 'F9'
	done
}

enter() {
	xdotool type --delay $SPEED $1
}

key() {
	xdotool key --delay $SPEED $1
}

info() {
	zenity --info --text="$1"
}
main "$@"
