#!/bin/bash

find . -name build -type d | xargs -i -t rm -Rf {}
