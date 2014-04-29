; /- filename -/
; helper.scm

; /- copyright -/
; Copyright (c) 2014 Alexej Magura

; This file is part of Zelda Battery.

; Zelda Battery is free software: you can redistribute it and/or modify
; it under the terms of the GNU General Public License as published by
; the Free Software Foundation, either version 3 of the License, or
; (at your option) any later version.

; Zelda Battery is distributed in the hope that it will be useful,
; but WITHOUT ANY WARRANTY; without even the implied warranty of
; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
; GNU General Public License for more details.

; You should have received a copy of the GNU General Public License
; along with Zelda Battery.  If not, see <http://www.gnu.org/licenses/>.
(declare (unit helper))

#>
extern const char * cppToScheme__sed(void);
extern const char * cppToScheme__awk(void);
<#
(define xsed (foreign-lambda c-string "cppToScheme__sed"))
(define xawk (foreign-lambda c-string "cppToScheme__awk"))

(define percent->integer
  (lambda (perc)
    (inexact->exact (* (truncate (* (/ (string->number perc) 100) 10)) 10))))

(define on-ac-power
  (lambda (util)
    (cond ((equal=? util "pmset")
           (system "pmset -g | sed '/Power/!d' | grep '*' | grep 'AC'"))

          ((equal=? util "acpi")
           (system "acpi -a | grep 'on-line'"))

          ((equal=? util "yacpi")
           (system "yacpi -pb | grep charging")))))
  
(define get-power-level
  (lambda (util)
    (cond ((equal=? util "pmset")
           (capture "pmset -g ps | sed -E '/%/!d; s_.*[[:space:]]([0-9]+)%.*_\\1_' | tr -d '\\n'"))

          ((equal=? util "acpi")
           (capture "acpi | awk '{print $4}' | sed -r 's_([0-9]+)%.*_\\1_' | tr -d '\\n'"))

          ((equal=? util "yacpi")
           (capture "yacpi | sed -r 's_.*=[[:space:]]([0-9]+)%.*_\\1_' | tr -d '\\n'")))))

(define heart "♥")
(define empty-heart "♡")
