#!/usr/bin/env bash

startdir="$(realpath "$(dirname $0)")"

cd "$startdir"/../src
csi <<<'
(load "cast.scm")
(load "io.scm")
(load "list.scm")
(load "power.scm")
(load "regex.scm")
(load "test.scm")
(load "unicode.scm")
' <&1
