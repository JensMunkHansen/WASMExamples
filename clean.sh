#!/bin/bash

find . -name build -type d | xargs -i -t rm -Rf {}
find . -name node_modules -type d | xargs -i -t rm -Rf {}
