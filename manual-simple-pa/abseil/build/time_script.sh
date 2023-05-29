#!/bin/bash

# Clear the contents of output.txt
> output.txt

# Declare variables for sum and count
sum=0
count=0

# Run the command 11 times, save the output and calculate the sum
for ((i = 1; i <= 11; i++)); do
  output=$( { time -p ./abseil_pa_manual.out; } 2>&1 )
  elapsed_time=$(echo "$output" | grep -oP 'real\s+\K\S+')
  echo "Run $i: $output"
  echo "Run $i: $output" >> output.txt
  
  # Exclude the first run when calculating sum and count
  if ((i > 1)); then
    sum=$(echo "$sum + $elapsed_time" | bc -l)
    count=$((count + 1))
  fi
done

# Calculate the average
average=$(echo "scale=2; $sum / $count" | bc -l)

# Print the average
echo "Average (over last 10 runs): $average"
