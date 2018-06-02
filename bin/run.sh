
if [ $# -lt 2 ]; then
    echo "Wrong number of parameters"
    echo "Expected 2 (inpu_file_name output_file_name)"
    echo "Got ${#}( ${@} )"
    exit
fi 


SCRIPT_PATH=`dirname $0`
CURR_PATH=`pwd`

INPUT="${CURR_PATH}/${1}"
OUTPUT="$2"

if [ ! -f "$INPUT" ]; then
    echo "Input file doesn't exist or is not a regular file!"
    exit
fi

mkdir "${CURR_PATH}/output" >/dev/null 2>&1
mkdir "${CURR_PATH}/output/logs" >/dev/null 2>&1

${SCRIPT_PATH}/conc_to_go "$INPUT" "$OUTPUT" >"${CURR_PATH}/output/logs/translator"

if [ $? -ne 0 ]; then
    echo "Could not translate the file. See output/logs for more information."
    exit
fi

FILE_PATH="${CURR_PATH}/output/${OUTPUT}.go"

go fmt $FILE_PATH
go build $FILE_PATH >"${CURR_PATH}/output/logs/go_build" 2>"${CURR_PATH}/output/logs/go_build_errors"
BUILD_OUTPUT=$?

cp "$FILE_PATH" "${CURR_PATH}/output/logs"
rm "$FILE_PATH"

if [ $BUILD_OUTPUT -ne 0 ]; then
    echo "Go build problems!"
    exit
fi

mv "${CURR_PATH}/$OUTPUT" "${CURR_PATH}/output/${OUTPUT}"

if [ "$3" = "run" ]; then
    echo 'Run option present. Running output'
    ${CURR_PATH}/output/${OUTPUT}
fi

echo "Success :)"