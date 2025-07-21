#! /usr/bin/env bash
#
# SPDX-FileCopyrightText: 2025 George Florea Bănuș <georgefb899@gmail.com>
#
# SPDX-License-Identifier: CC0-1.0
#

# ensure it is defined
podir=${podir:?}

$EXTRACTRC `find . -name \*.kcfg` >> rc.cpp
$XGETTEXT $(find . -name \*.cpp -o -name \*.h) -o "$podir"/hana.pot

# Extract JavaScripty files as what they are, otherwise for example
# template literals won't work correctly (by default we extract as C++).
# https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Template_literals
if [ -f ./hana.pot ]; then
    $XGETTEXT --join-existing --language=JavaScript $(find . -name \*.qml -o -name \*.js) -o "$podir"/hana.pot
else
    $XGETTEXT --language=JavaScript $(find . -name \*.qml -o -name \*.js) -o "$podir"/hana.pot
fi

