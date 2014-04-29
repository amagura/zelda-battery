; /- filename -/
; zelda-blink.scm

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
#!/usr/bin/env csi -qn
(use shell srfi-1 ansi-escape-sequences)
(declare (uses helper))

#>
extern const char * cppToScheme__host_acpi_util(void);
<#
(define x-host-acpi-util (foreign-lambda c-string "cppToScheme__host_acpi_util"))

(if (<= (percent->integer (get-power-level (x-host-acpi-util))) 30)

(cond ((equal=? (x-host-acpi-util) "pmset")
       (define power-level
         (capture "pmset -g ps | sed -E '/%/!d; s_.*[[:space:]]([0-9]+)%.*_\\1_' | tr -d '\\n'")))
      ((equal=? (x-host-acpi-util) "acpi")
       (define power-level
         (capture "acpi | awk '{print $4}' | sed -r 's_([0-9]+)%.*_\\1_' | tr -d '\\n'"))))
       
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
