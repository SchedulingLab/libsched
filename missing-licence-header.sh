#!/bin/sh

LICENCE="SPDX-License-Identifier: GPL-3.0-or-later"

for F in $(find include library)
do
  if ! [ -f "$F" ]
  then
    continue
  fi

  if ! grep "$LICENCE" "$F" > /dev/null
  then
    echo $F
  fi
done
