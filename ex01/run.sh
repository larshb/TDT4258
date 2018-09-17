#!/bin/bash

make > make.log #2> /dev/null
#make
if [ $? -eq 0 ]
then
	printf "\e[92mSuccessfully compiled! \e[95mUploading... "
	make upload > upload.log
	if [ $? -eq 0 ]
	then
		printf "\e[92mUpload successfull!\n\e[39m"
	else
		cat upload.log
	fi
	rm upload.log
else
	cat make.log
fi
rm make.log
