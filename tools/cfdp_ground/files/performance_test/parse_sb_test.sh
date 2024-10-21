#!/bin/bash

# Check if a file is provided as input
if [ $# -lt 1 ]; then
  echo "Usage: $0 input_file [output_file]"
  exit 1
fi

input_file="$1"
output_file="${2:-sb_test_output.txt}"  # Use second parameter if provided, otherwise default to "filtered_output.txt"

# Use grep to filter lines containing "Thread" or "[  MIR]" and save to the output file
grep -E 'Thread|\[  MIR\]' "$input_file" > "$output_file"

# Confirm completion and display output file path
echo "Software bus tests saved to $output_file"
