# Get a fresh build to measure coverage
make clean

# Set coverage-flags to be used inside make.
export CFLAGS="-ftest-coverage -fprofile-arcs"
export LDLIBS="-lgcov"

# Make, using these extra flags.
make

# Remove any of the gcda files (should be done by make clean).
rm -f *.gcda

echo "Running test inputs given with the starter"
for i in 01 02 03 04 05 06 07 08 09 10 11
do
    args=()
    if [ "$i" == "09" ]; then
	args=(-term)
    elif [ "$i" == "11" ]; then
	args=(-bad -arguments)
    fi
    echo "./driver ${args[@]} < input-$i.txtt > output.txt 2> stderr.txt"
    ./driver ${args[@]} < input-$i.txt > output.txt 2> stderr.txt
done

# To report if we find any student inputs.
STUDENT_INPUT_COUNT=0

# Run the student-generated inputs that are redirected from a file.
list=$(echo my-file-input-*.txt)

if [ "$list" != 'my-file-input-*.txt' ]; then
    echo
    echo "Running with student-created file test inputs"
    for input in my-file-input-*.txt
    do
	echo "./driver < $input > output.txt 2> stderr.txt"
	./driver < $input > output.txt 2> stderr.txt
    done

    STUDENT_INPUT_COUNT=$((STUDENT_INPUT_COUNT + 1))
fi

# Run the student-generated inputs pretend to be typed in from a terminal
# (although they are really from a file).
list=$(echo my-term-input-*.txt)

if [ "$list" != 'my-term-input-*.txt' ]; then
    echo
    echo "Running with student-created terminal test inputs"
    for input in my-term-input-*.txt
    do
	echo "./driver -term < $input > output.txt 2> stderr.txt"
	./driver -term < $input > output.txt 2> stderr.txt
    done

    STUDENT_INPUT_COUNT=$((STUDENT_INPUT_COUNT + 1))
fi

if [ "$STUDENT_INPUT_COUNT" -eq 0 ]; then
    echo "**** No student-created test inputs"
fi

gcov driver map value input
