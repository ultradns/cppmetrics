#!/bin/bash
echo THE SCRIPT IS NOT WORKING YET. YOU NEED TO PERFORM THE ACTIONS MANUALLY
echo INSTALLING to system folders
# copy the include files to /local/include/cppmetrics/
pushd src
mkdir -p t
find cppmetrics -name *.h -exec 'cp --parent \{\} t/' \\;
# The lib goes to /local/lib/
cp ../libcppmetrics_hp.a t/
popd
echo done
