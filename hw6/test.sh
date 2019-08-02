#!/bin/bash
if [ "$1" == "ref" ];then
	echo "run ref code..."
	if [ "$3" == "D" -o "$3" == "d" ];then
		echo "-------------------------------------------------------"
		./ref/cirTest-linux18 -f ./tests.err/$2
	elif [ "$3" == "S" -o "$3" == "s" ];then
		./ref/cirTest-linux18 -f ./tests.err/$2 >& ./mytestoutput/ref_$2.txt
	else
		echo "show detail or save?(D/S):"
		read detail
		if [ "$detail" == "D" -o "$detail" == "d" ];then
			echo "-------------------------------------------------------"
			./ref/cirTest-linux18 -f ./tests.err/$2
		elif [ "$detail" == "S" -o "$detail" == "s" ];then
			./ref/cirTest-linux18 -f ./tests.err/$2 >& ./mytestoutput/ref_$2.txt
		else
			echo Format\ error!
			return 1
		fi
	fi

elif [ "$1" == "my" ];then
	echo "make my code..."
	make
	# if [ $? -ne 0 ];then
	# 	return 1
	# fi
	echo "run my code..."
	if [ "$3" == "D" -o "$3" == "d" ];then
		echo "-------------------------------------------------------"
		./bin/cirTest -f ./tests.err/$2
	elif [ "$3" == "S" -o "$3" == "s" ];then
		./bin/cirTest -f ./tests.err/$2 >& ./mytestoutput/my_$2.txt
	else
		echo "show detail or save?(D/S):"
		read detail
		if [ "$detail" == "D" -o "$detail" == "d" ];then
			echo "-------------------------------------------------------"
			./bin/cirTest -f ./tests.err/$2
		elif [ "$detail" == "S" -o "$detail" == "s" ];then
			./bin/cirTest -f ./tests.err/$2 >& ./mytestoutput/my_$2.txt
		else
			echo Format\ error!
			return 1
		fi
	fi

elif [[ "$1" == "diff" ]]; then
	echo "make my code..."
	make
	echo "run my code..."
	./bin/cirTest -f ./tests.err/$2 >& ./mytestoutput/my_$2.txt
	echo "run ref code..."
	./ref/cirTest-linux18 -f ./tests.err/$2 >& ./mytestoutput/ref_$2.txt
	diff ./mytestoutput/my_$2.txt ./mytestoutput/ref_$2.txt
fi
