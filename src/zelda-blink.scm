#!/usr/bin/env csi -qn
(use fmt-c shell srfi-1 ansi-escape-sequences)

(define percent->integer
  (lambda (perc)
    (inexact->exact (* (truncate (* (/ (string->number perc) 100) 10)) 10))))

;; check to see if PMSET is present
(fmt-c#cpp-ifdef "ZELDA_BATTERY_PMSET"
                 (define power-level 
                   (capture "pmset -g ps | sed -E '/%/!d; s_.*[[:space:]]([0-9]+)%.*_\1_; s_[[:space:]]+$__' | tr -d '\\n'"))) ; assumes you're running on Mac OS X

;; check to see if ACPI is present
(fmt-c#cpp-ifdef "ZELDA_BATTERY_ACPI"
                 (define power-level
                   (capture "acpi | awk '{print $4}' | sed -r 's/\s+$//' | tr -d '\\n'")))

(if (<= (percent->integer power-level) 30)
  (display (car (string-split (ansi-escape-sequences#set-text '(blink fg-red) " "))))
  (display (car (string-split (ansi-escape-sequences#set-text '(fg-red) " ")))))
