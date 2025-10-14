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

	move=$(echo $1 | grep tests | wc -l)
	if (( $move == 0)) ; then
		cd tests
	fi
	executable="$1"

	if [[ ! -f "$executable" ]]; then
		echo "Test file $executable not found"
		cd ..
		return
	fi


	echo Here in $PWD

	local name=$(echo $1 | tr -d tests)

	./$executable > logs/$name.log

	if (( $? == 0 )) ; then
		error=$(cat logs/$name.log | grep FAILED | wc -l)
	        if (( $error == 0 )) ; then
        	        end_display SUCCESS
	        else
        	        end_display FAILED
        	fi
	else
		echo test file not found
	fi
	if (( $move == 0)) ; then
                cd ..
        fi

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

if ! [[ -f "logs" ]] ; then
	mkdir -p logs
else
	rm -rf logs
	mkdir -p logs
fi

#	si 0 arg alors exec tout
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
	if [ -f "files" ] ; then
		rm files
	fi
	exit
fi

#	execute chaque argument donne au prog
for arg in $@
do
	do_i_test=$(echo $file | grep "stress" | wc -l)
        if (( $do_i_test == 0 )) ; then
        	test_server "tests/$arg.sh"
	fi
done

sleep 1
echo Done
exit
echo "Do you want to test something more ? "
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
echo "Tap enter alone to test it all"
echo "'q' to escape"

read -e response

case $response in \
	"1") test_server autoindex.sh ;;
	"2") test_server cgi.sh ;;
        "3") test_server client_size.sh ;;
        "4") test_server host.sh ;;
        "5") test_server index.sh ;;
        "6") test_server location.sh ;;
        "7") test_server methods.sh ;;
        "8") test_server names.sh ;;
        "9") test_server ports.sh ;;
        "10") test_server root.sh ;;
        "11") test_server upload.sh ;;
	"12") stress_test ;;
	*) echo Invalid input ...
esac

echo 
echo 
echo

if [ -f files ] ; then
	rm files 
fi
