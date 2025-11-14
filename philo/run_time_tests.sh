#!/bin/bash

# Run a few focused timing tests for 2 philosophers.
# This script expects the `philo` executable to be in the current directory
# and that its output lines follow the format: "<timestamp> <id> <status>"
# It checks that any reported death happens within 10 ms of the configured
# time_to_die (a larger delay is considered a failure).

set -euo pipefail

TMPDIR=/tmp/philo_time_tests
mkdir -p "$TMPDIR"

TOLERANCE_MS=10

run_and_capture() {
    local name="$1"; shift
    local args=("$@")
    local logfile="$TMPDIR/${name}.log"

    # print progress to stderr so callers capturing stdout receive only the logfile path
    echo "Running: ./philo ${args[*]}" >&2
    # Use a short timeout to avoid hangs
    timeout 8 ./philo "${args[@]}" > "$logfile" 2>&1 || true
    echo "$logfile"
}

check_death_time() {
    local logfile="$1"
    local expected_die_ms="$2"

    # Find the first line that contains 'died' and take its timestamp
    if ! grep -q "died" "$logfile"; then
        echo "NO_DEATH"
        return 1
    fi
    local line
    line=$(grep "died" "$logfile" | head -n1)
    local ts
    ts=$(printf "%s" "$line" | awk '{print $1}')
    # compute difference
    local diff
    diff=$((ts - expected_die_ms))
    if [ $diff -lt 0 ]; then
        diff=$(( -diff ))
    fi
    if [ $diff -le $TOLERANCE_MS ]; then
        echo "OK: death at ${ts} ms (expected ${expected_die_ms} ms, diff ${diff} ms)"
        return 0
    else
        echo "BAD: death at ${ts} ms (expected ${expected_die_ms} ms, diff ${diff} ms > ${TOLERANCE_MS} ms)"
        return 2
    fi
}

fail_count=0

echo "========================================="
echo "PHILOSOPHERS TIMING TESTS (2 philosophers)"
echo "Tolerance: ${TOLERANCE_MS} ms"
echo "========================================="

# A list of tests. Each entry: name | args... | expect (die|nodie) | expected_die_ms
# expected_die_ms is the numeric time_to_die value from the args when expect=die
tests=(
    "fast_no_death 2 800 200 200 nodie 0"
    "should_die_310 2 310 200 100 die 310"
    "should_die_200 2 200 150 150 die 200"
    "long_eat_no_death 2 1000 500 500 nodie 0"
)

for entry in "${tests[@]}"; do
    # split
    name=$(printf "%s" "$entry" | awk '{print $1}')
    n=$(printf "%s" "$entry" | awk '{print $2}')
    time_to_die=$(printf "%s" "$entry" | awk '{print $3}')
    time_to_eat=$(printf "%s" "$entry" | awk '{print $4}')
    time_to_sleep=$(printf "%s" "$entry" | awk '{print $5}')
    expect=$(printf "%s" "$entry" | awk '{print $6}')
    expected_ms=$(printf "%s" "$entry" | awk '{print $7}')

    logfile=$(run_and_capture "$name" "$n" "$time_to_die" "$time_to_eat" "$time_to_sleep")

    if [ "$expect" = "die" ]; then
        out=$(check_death_time "$logfile" "$expected_ms" ) || status=$?
        status=${status:-0}
        if [ $status -eq 0 ]; then
            echo "[PASS] $name: $out"
        elif [ $status -eq 1 ]; then
            echo "[FAIL] $name: no death detected but expected a death at ${expected_ms} ms"
            echo "--- log ---"
            cat "$logfile" | tail -n 20
            echo "--- end log ---"
            fail_count=$((fail_count+1))
        else
            echo "[FAIL] $name: $out"
            echo "--- log ---"
            cat "$logfile" | tail -n 20
            echo "--- end log ---"
            fail_count=$((fail_count+1))
        fi
    else
        if grep -q "died" "$logfile"; then
            echo "[FAIL] $name: found death but expected none"
            grep "died" "$logfile"
            fail_count=$((fail_count+1))
        else
            echo "[PASS] $name: no death as expected"
        fi
    fi
    echo
done

if [ $fail_count -eq 0 ]; then
    echo "ALL TIMING TESTS PASSED"
    exit 0
else
    echo "$fail_count test(s) failed"
    exit 2
fi
