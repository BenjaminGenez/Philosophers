#!/bin/bash

HERE=$(cd "$(dirname "$0")" && pwd)
cd "$HERE"

echo "========================================="
echo "MEMORY LEAK TESTS WITH VALGRIND"
echo "========================================="
echo

# Check if valgrind is installed
if ! command -v valgrind >/dev/null 2>&1; then
    echo "ERROR: valgrind is not installed"
    echo "Install with: sudo apt-get install valgrind"
    exit 1
fi

VALGRIND_OPTS="--leak-check=full --show-leak-kinds=all --track-origins=yes --error-exitcode=1"
FAILED=0

# Function to check valgrind results
check_leaks() {
    local log_file=$1
    local test_name=$2
    
    # Check if valgrind completed successfully
    if ! grep -q "HEAP SUMMARY" "$log_file"; then
        echo "FAIL: Valgrind did not complete properly"
        FAILED=1
        return
    fi
    
    # Check for memory leaks
    if grep -q "All heap blocks were freed -- no leaks are possible" "$log_file"; then
        echo "PASS: No memory leaks detected"
        return
    fi
    
    # Check the leak summary for any leaks
    local definitely_lost=$(grep "definitely lost:" "$log_file" | awk '{print $4}' | tr -d ',')
    local indirectly_lost=$(grep "indirectly lost:" "$log_file" | awk '{print $4}' | tr -d ',')
    local possibly_lost=$(grep "possibly lost:" "$log_file" | awk '{print $4}' | tr -d ',')
    local still_reachable=$(grep "still reachable:" "$log_file" | awk '{print $4}' | tr -d ',')
    
    if [ "$definitely_lost" = "0" ] && [ "$indirectly_lost" = "0" ] && [ "$possibly_lost" = "0" ] && [ "$still_reachable" = "0" ]; then
        echo "PASS: No memory leaks detected"
    else
        echo "FAIL: Memory leaks detected"
        echo "Details:"
        grep -A 5 "LEAK SUMMARY" "$log_file"
        FAILED=1
    fi
}

echo "Test 1: Memory leak check with 1 philosopher (dies)"
echo "Command: ./philo 1 800 200 200"
timeout 5 valgrind $VALGRIND_OPTS ./philo 1 800 200 200 > /tmp/leak1.log 2>&1
check_leaks /tmp/leak1.log "Test 1"
echo
echo "---"
echo

echo "Test 2: Memory leak check with 5 philosophers (limited meals)"
echo "Command: ./philo 5 800 200 200 3"
timeout 10 valgrind $VALGRIND_OPTS ./philo 5 800 200 200 3 > /tmp/leak2.log 2>&1
check_leaks /tmp/leak2.log "Test 2"
echo
echo "---"
echo

echo "Test 3: Memory leak check with must_eat parameter"
echo "Command: ./philo 5 800 200 200 7"
timeout 15 valgrind $VALGRIND_OPTS ./philo 5 800 200 200 7 > /tmp/leak3.log 2>&1
check_leaks /tmp/leak3.log "Test 3"
echo
echo "---"
echo

echo "Test 4: Memory leak check with death scenario"
echo "Command: ./philo 4 310 200 100"
timeout 5 valgrind $VALGRIND_OPTS ./philo 4 310 200 100 > /tmp/leak4.log 2>&1
check_leaks /tmp/leak4.log "Test 4"
echo
echo "---"
echo

echo "Test 5: Memory leak check with 2 philosophers (death)"
echo "Command: ./philo 2 400 200 200"
timeout 5 valgrind $VALGRIND_OPTS ./philo 2 400 200 200 > /tmp/leak5.log 2>&1
check_leaks /tmp/leak5.log "Test 5"
echo
echo "---"
echo

echo "Test 6: Memory leak check with quick meals"
echo "Command: ./philo 3 600 100 100 5"
timeout 10 valgrind $VALGRIND_OPTS ./philo 3 600 100 100 5 > /tmp/leak6.log 2>&1
check_leaks /tmp/leak6.log "Test 6"
echo
echo "---"
echo

echo "========================================="
if [ $FAILED -eq 0 ]; then
    echo "ALL MEMORY LEAK TESTS PASSED"
    echo "========================================="
    exit 0
else
    echo "SOME MEMORY LEAK TESTS FAILED"
    echo "========================================="
    exit 1
fi
