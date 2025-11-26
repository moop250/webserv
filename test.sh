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
  	
	sub=.sh
	name=$1
	# echo for : $1
	name=$(echo ${name%$sub}.log)
	
	touch ../logs/$name

	./$executable $2 > ../logs/$name

	if (( $? == 0 )) ; then
		error=$(cat ../logs/$name | grep FAILED | wc -l)
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

	foo2=$(cat foo2.log | grep iteration)
	foo1=$(cat foo1.log | grep iteration)
	d=$(diff foo1.log foo2.log)
	rm foo1.log
	rm foo2.log
	echo 1 iterations : $foo1
	echo 2 iterations : $foo2
	echo diff : $d
}

stress_test
exit
if ! [[ -f "logs" ]] ; then
	mkdir -p logs
else
	rm -rf logs
	mkdir -p logs
fi

if (( $BASH_ARGC == 0 )) ; then
        echo testing all && echo
        ls tests > files
        export files=$(cat files)
        for file in $files
        do
                #echo file is $file
		do_i_test=$(echo $file | grep "stress" | wc -l)
		if (( $do_i_test == 0 )) ; then
			test_server $file $config
        	fi
		sleep 1
	done
	if [ -f "files" ] ; then
		rm files
	fi
else
	for arg in $@
	do
		do_i_test=$(echo $file | grep "stress" | wc -l)
		if (( $do_i_test == 0 )) ; then
	        	test_server "tests/$arg.sh" $config
		fi
	done
fi

if [ -f "files" ] ; then
	rm files
fi

sleep 1
echo Done
exit	# Mode interactif un peu chiant...

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
	"1") test_server autoindex.sh $config;;
	"2") test_server cgi.sh $config;;
        "3") test_server client_size.sh $config;;
        "4") test_server host.sh $config;;
        "5") test_server index.sh $config;;
        "6") test_server location.sh $config;;
        "7") test_server methods.sh $config;;
        "8") test_server names.sh $config;;
        "9") test_server ports.sh $config;;
        "10") test_server root.sh $config;;
        "11") test_server upload.sh $config;;
	"12") stress_test ;;
	*) echo Invalid input ...
esac

echo 
echo 
echo

if [ -f files ] ; then
	rm files 
fi
