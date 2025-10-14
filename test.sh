#!/bin/bash

display() {	# takes title for arg
	echo ////////////////////////////////
	echo 
	echo testing : $1 ...
	echo
}

end_display() {	# takes test status for arg
	echo test $1
	echo && echo
	echo ////////////////////////////////
	echo
}

test_server() {	# takes file to exec
	display $1

	cd tests	
	executable="$1.sh"

	./$executable > logs/$1.log

	error=$(cat logs/$1.log | grep FAILED | wc -l)
	if (( $error == 0 )) ; then
		end_display SUCCESS
	else
		end_display FAILED
	fi
	cd ..
}

stress_test() {
	echo this test may takes some time ...

	./tests/stress1.sh &
	pid1=$!
	./tests/stress2.sh &
	pid2=$!
	
	
	sleep 5

	kill $pid1
	kill $pid2
}

if (( $BASH_ARGC == 0 )) ; then
        echo testing all && echo
        ls tests > files
        export files=$(cat files)
        for file in $files
        do
                echo file is $file
                test_server $file
        done
	exit
fi

echo "What do you want to test ? "
echo "1) autoindex"
echo "2) cgi"
echo "3) Client max body size"
echo "4) hosts"
echo "5) index"
echo "6) locations"
echo "7) methods"
echo "8) names"
echo "9) ports"
echo "10) roots"
echo "11) upload"
echo "12) stress test"

read -e response

case $response in \
	"1") autoindex ;;
	"2") cgi ;;
        "3") client_size ;;
        "4") host ;;
        "5") index ;;
        "6") location ;;
        "7") methods ;;
        "8") names ;;
        "9") ports ;;
        "10") root ;;
        "11") upload ;;
	"12") stress_test ;;
	*) echo Invalid input ...
esac

echo test finished
