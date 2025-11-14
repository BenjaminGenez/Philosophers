#!/bin/bash

echo "========================================="
echo "PHILOSOPHERS EVALUATION TESTS"
echo "========================================="
echo

echo "Test 1: ./philo 1 800 200 200"
echo "Expected: Philosopher should not eat and should die"
./philo 1 800 200 200
echo
echo "---"
echo

echo "Test 2: ./philo 5 800 200 200"
echo "Expected: No one should die (running for 10 seconds)"
timeout 10 ./philo 5 800 200 200 > /tmp/test2.log 2>&1
if grep -q "died" /tmp/test2.log; then
    echo "FAIL: Someone died"
    grep "died" /tmp/test2.log
else
    echo "PASS: No deaths in 10 seconds"
fi
echo
echo "---"
echo

echo "Test 3: ./philo 5 800 200 200 7"
echo "Expected: No one should die, simulation stops when all eat 7 times"
timeout 15 ./philo 5 800 200 200 7 > /tmp/test3.log 2>&1
EXIT_CODE=$?
if [ $EXIT_CODE -eq 0 ]; then
    if grep -q "died" /tmp/test3.log; then
        echo "FAIL: Someone died"
        tail -20 /tmp/test3.log
    else
        echo "PASS: Simulation completed, all ate 7 times"
        tail -5 /tmp/test3.log
    fi
else
    echo "FAIL: Timeout or error"
fi
echo
echo "---"
echo

echo "Test 4: ./philo 4 410 200 200"
echo "Expected: No one should die (running for 10 seconds)"
timeout 10 ./philo 4 410 200 200 > /tmp/test4.log 2>&1
if grep -q "died" /tmp/test4.log; then
    echo "FAIL: Someone died"
    grep "died" /tmp/test4.log
else
    echo "PASS: No deaths in 10 seconds"
fi
echo
echo "---"
echo

echo "Test 5: ./philo 4 310 200 100"
echo "Expected: A philosopher should die"
./philo 4 310 200 100 | tail -10
echo
echo "---"
echo

echo "========================================="
echo "All tests completed"
echo "========================================="
