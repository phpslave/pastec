#!/bin/bash
SQL_FILENAME_LINK="http://pastec.io/files/visualWordsORB.tar.gz"
SQL_FILENAME_BASE="visualWordsORB.dat"

echo "Import"
if [ ! -f "/home/vagrant/share/pastec/build/${SQL_FILENAME_BASE}" ]; then
   cd /home/vagrant/share/pastec/build/
   /usr/bin/wget ${SQL_FILENAME_LINK}
   /bin/tar -zxvf visualWordsORB.tar.gz
else
    echo "Already Loaded"
    exit 0
fi