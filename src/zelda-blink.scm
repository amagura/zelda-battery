#!/usr/bin/env csi -qn
(use shell srfi-1 ansi-escape-sequences)

#>
extern const char * cppToScheme__host_acpi_util(void);
<#
(define x-host-acpi-util (foreign-lambda c-string "cppToScheme__host_acpi_util"))

(define percent->integer
  (lambda (perc)
    (inexact->exact (* (truncate (* (/ (string->number perc) 100) 10)) 10))))

(cond ((equal=? (x-host-acpi-util) "pmset")
       (define power-level
         (capture "pmset -g ps | sed -E '/%/!d; s_.*[[:space:]]([0-9]+)%.*_\\1_; s_[[:space:]]+$__' | tr -d '\\n'")))
      ((equal=? (x-host-acpi-util) "acpi")
       (define power-level
         (capture "acpi | awk '{print $4}' | sed -r 's/\\s+$//' | tr -d '\\n'"))))
       
(if (<= (percent->integer power-level) 30)
  (display (car (string-split (ansi-escape-sequences#set-text '(blink fg-red) " "))))
  (display (car (string-split (ansi-escape-sequences#set-text '(fg-red) " ")))))

#|
;; check to see if we're running on Mac OS X
(fmt-c#cpp-ifdef "ZELDA_BATTERY_DARWIN"
                 (define power-level 
                   (capture "pmset -g ps | sed -E '/%/!d; s_.*[[:space:]]([0-9]+)%.*_\\1_; s_[[:space:]]+$__' | tr -d '\\n'"))) ; assumes you're running on Mac OS X

;; check to see if we're running on LINUX
(fmt-c#cpp-ifdef "ZELDA_BATTERY_LINUX"
                 (define power-level
                   (capture "acpi | awk '{print $4}' | sed -r 's/\\s+$//' | tr -d '\\n'")))

|#
