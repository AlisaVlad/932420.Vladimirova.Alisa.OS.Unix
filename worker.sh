#!/bin/sh

SHARED_DIR="/shared"
LOCK_FILE="$SHARED_DIR/.lock"

CONTAINER_ID=$(od -An -N4 -tx4 /dev/urandom | tr -d ' ')
COUNTER=1

trap 'exit 0' TERM INT

while true; do
    (
        flock -x 9

        i=1
        while true; do
            FILENAME=$(printf "%03d" "$i")
            FILEPATH="$SHARED_DIR/$FILENAME"

            if [ ! -e "$FILEPATH" ]; then
                : > "$FILEPATH"
                echo "$FILEPATH"
                break
            fi
            i=$((i + 1))
        done

    ) 9>"$LOCK_FILE" > /tmp/current_file

    CREATED_FILE=$(cat /tmp/current_file)

    printf "Container ID: %s. File seq: %d\n" "$CONTAINER_ID" "$COUNTER" > "$CREATED_FILE"
    COUNTER=$((COUNTER + 1))

    sleep 1

    rm -f "$CREATED_FILE"

    sleep 1
done
