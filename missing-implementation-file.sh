#!/bin/sh

HEADERS_FILE=$(mktemp)

find include | sed 's/^include\///' | sed 's/.h$/.cc/' | sort > "$HEADERS_FILE"

SOURCES_FILE=$(mktemp)

find library | sed 's/^library\///' | sort > "$SOURCES_FILE"

diff --color -u "$HEADERS_FILE" "$SOURCES_FILE"
