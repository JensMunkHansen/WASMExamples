#!/bin/bash

cmake --preset default
cmake --build build --config Asan
