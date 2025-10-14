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
	executable="$1"

	if [[ ! -f "$executable" ]]; then
		echo "Test file $executable not found"
		cd ..
		return
	fi

	./$executable  > logs/$1.log

	if (( $? == 0 )) ; then
		error=$(cat logs/$1.log | grep FAILED | wc -l)
	        if (( $error == 0 )) ; then
        	        end_display SUCCESS
	        else
        	        end_display FAILED
        	fi
	else
		echo test file not found
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

mkdir logs
if (( $BASH_ARGC == 0 )) ; then
        echo testing all && echo
        ls tests > files
        export files=$(cat files)
        for file in $files
        do
                #echo file is $file
		do_i_test=$(echo $file | grep "stress" | wc -l)
		if (( $do_i_test == 0 )) ; then
			test_server $file
        	fi
	done
	rm files
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
	"1") test_server autoindex ;;
	"2") test_server cgi ;;
        "3") test_server client_size ;;
        "4") test_server host ;;
        "5") test_server index ;;
        "6") test_server location ;;
        "7") test_server methods ;;
        "8") test_server names ;;
        "9") test_server ports ;;
        "10") test_server root ;;
        "11") test_server upload ;;
	"12") stress_test ;;
	*) echo Invalid input ...
esac

echo 
echo 
echo

rm files 
