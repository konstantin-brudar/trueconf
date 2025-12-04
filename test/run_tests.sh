#!/bin/bash

# Usage: ./run_tests.sh <program_path> <test_file>

PROGRAM="$1"
TESTS_FILE="$2"

PASSED=0
FAILED=0
TOTAL=0

if [[ ! -x "$PROGRAM" ]]; then
    echo "ERROR: Program '$PROGRAM' not found or not executable."
    exit 1
fi

if [[ ! -f "$TESTS_FILE" ]]; then
    echo "ERROR: Test file '$TESTS_FILE' not found."
    exit 1
fi

echo "=== Running tests ==="
echo "Program: $PROGRAM"
echo "Test file: $TESTS_FILE"
echo

while IFS='|' read -r test_file options expected; do
    # Skip empty lines and comments
    [[ -z "$test_file" || "$test_file" =~ ^# ]] && continue

    if [[ ! -f "$test_file" ]]; then
        echo "FAILED: $test_file - file not found"
        ((FAILED++))
        ((TOTAL++))
        continue
    fi

    options=$(echo "$options" | xargs)
    output=$("$PROGRAM" "$test_file" $options 2>/dev/null)
    actual=$(echo "$output" | tr -d '\r\n[:space:]')

    if [[ "$actual" == "$expected" ]]; then
        echo "PASSED: $test_file $options (expected: $expected, got: $actual)"
        ((PASSED++))
    else
        echo "FAILED: $test_file $options (expected: $expected, got: '$actual')"
        ((FAILED++))
    fi

    ((TOTAL++))
done < "$TESTS_FILE"

echo
echo "=== RESULT ==="
echo "Total tests: $TOTAL"
echo "Passed: $PASSED"
echo "Failed: $FAILED"
if [[ $FAILED -eq 0 ]]; then
    echo "All tests passed!"
else
    echo "Some tests failed!"
fi

exit $FAILED
