
if [ $# -lt 2 ]; then
    echo "Wrong number of parameters"
    echo "Expected 2 (inpu_file_name output_file_name)"
    echo "Got ${#}( ${@} )"
    exit
fi 

INPUT="$1"
OUTPUT="$2"

if [ ! -f "$INPUT" ]; then
    echo "Input file doesn't exist or is not a regular file!"
    exit
fi

mkdir output >/dev/null 2>&1
mkdir "output/logs" >/dev/null 2>&1

./conc_to_go "$INPUT" "$OUTPUT" >"output/logs/translator"

if [ $? -ne 0 ]; then
    echo "Could not translate the file. See output/logs for more information."
    exit
fi

FILE_PATH="output/${OUTPUT}.go"

go fmt $FILE_PATH
go build $FILE_PATH >"output/logs/go_build"

if [ $? -ne 0 ]; then
    echo "Go build problems!"
    cp "$FILE_PATH" "output/logs"
    rm "$FILE_PATH"
    exit
fi

cp "$FILE_PATH" "output/logs"
rm "$FILE_PATH"
mv "$OUTPUT" "output/${OUTPUT}"

if [ "$3" = "run" ]; then
    echo 'Run option present. Running output'
    ./output/${OUTPUT}
fi

echo "Success :)"