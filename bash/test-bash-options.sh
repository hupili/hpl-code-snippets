#!/bin/bash
# Demo of bash setups

# Show every command executed
set -x
# Stop on error
set -e

# Trigger return value 1
false
echo $?

# Test stop
true
echo "You should not be able to see this line"
