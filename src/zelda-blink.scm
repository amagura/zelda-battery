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
(declare (uses zbhelper))

#>
extern const char * cppToScheme__host_acpi_util(void);
extern int cppToScheme__blink_on_ac_pwr(void);
extern int cToScheme__printf(const char * restrict, const char *);
<#
(define x-host-acpi-util (foreign-lambda c-string "cppToScheme__host_acpi_util"))
(define x-blink-on-ac-pwr (foreign-lambda int "cppToScheme__blink_on_ac_pwr"))
(define xprintf (foreign-lambda int "printf" c-string c-string))

(let ((ret (xprintf "%s" "hello")))
 (display " ") (display ret))
(newline)
#;(if (<= (percent->integer (assume-power (x-host-acpi-util))) 30)
  (display (car (string-split (ansi-escape-sequences#set-text
                              (if (on-ac-power? (x-host-acpi-util))
                               (if (x-blink-on-ac-pwr)
                                '(blink fg-red)
                                '(fg-red))
                               '(blink fg-red)) " "))))
 (display (car (string-split (ansi-escape-sequences#set-text '(fg-red) " ")))))
