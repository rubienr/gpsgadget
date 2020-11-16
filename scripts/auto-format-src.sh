#!/bin/bash 

ack --cpp --cc -f src/ | xargs clang-format -i

