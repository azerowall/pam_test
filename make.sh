#!/bin/bash
g++ pam_app.cpp -lpam -g -ggdb -o app
gcc -shared -fPIC pam_mod.c -o pam_test_mod.so
