#!/bin/bash

MAKE_PATH=""
EXEC_PATH=""
BENCHMARK_PATH=""
OUTPUT_PATH=""

cd "$MAKE_PATH" || exit
make compile > /dev/null || exit
cd "$BENCHMARK_PATH" || exit
rm -rf "$OUTPUT_PATH"
mkdir "$OUTPUT_PATH"


printf "Running benchmarks...\n"

rm -f "$OUTPUT_PATH/output.csv"
echo "unoptimized,a,b,c" >> "$OUTPUT_PATH/output.csv"

shopt -s nullglob
for i in *.iloc; do
  ~uli/cs415/ILOC_Simulator/sim < "$i" > "$OUTPUT_PATH/${i%%.*}.out";
  uo=$(grep -oP '[0-9]* (?=cycles)' "$OUTPUT_PATH/${i%%.*}.out")

  "$EXEC_PATH" "-a" < "$i" > "$OUTPUT_PATH/${i%%.*}.a.iloc" 2> /dev/null;
  ~uli/cs415/ILOC_Simulator/sim < "$OUTPUT_PATH/${i%%.*}.a.iloc" > "$OUTPUT_PATH/${i%%.*}.a.out";
  a=$(grep -oP '[0-9]* (?=cycles)' "$OUTPUT_PATH/${i%%.*}.a.out")

  "$EXEC_PATH" "-b" < "$i" > "$OUTPUT_PATH/${i%%.*}.b.iloc" 2> /dev/null;
  ~uli/cs415/ILOC_Simulator/sim < "$OUTPUT_PATH/${i%%.*}.b.iloc" > "$OUTPUT_PATH/${i%%.*}.b.out";
  b=$(grep -oP '[0-9]* (?=cycles)' "$OUTPUT_PATH/${i%%.*}.b.out")

  "$EXEC_PATH" "-c" < "$i" > "$OUTPUT_PATH/${i%%.*}.c.iloc" 2> /dev/null;
  ~uli/cs415/ILOC_Simulator/sim < "$OUTPUT_PATH/${i%%.*}.c.iloc" > "$OUTPUT_PATH/${i%%.*}.c.out";
  c=$(grep -oP '[0-9]* (?=cycles)' "$OUTPUT_PATH/${i%%.*}.c.out")

  echo "$uo,$a,$b,$c" >> "$OUTPUT_PATH/output.csv"
done


printf "Benchmarks completed\n"