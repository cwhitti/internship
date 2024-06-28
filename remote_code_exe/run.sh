#!/bin/bash

VERSION=3.9
SOURCE_FILE=app/main.py

main ()
{
#echo "(!) Checking versions..." 
#newline
#echo "(!) $(pyenv versions)"
#echo "(!) $(source venv/bin/activate)"
echo "(!) Running with $(python$VERSION --version)"
#echo $(time python$VERSION -c 'int("2" + "0" * 100000000)')
echo $(python3 $SOURCE_FILE)
}

newline ()
{
    echo ""
}

main