; /- filename -/
; power.scm

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
(use list-utils)
(declare (unit zb-power))
(declare (uses zb-io))

;;; is the current machine running off AC Power (don't see why this wouldn't work on machines that do not have a battery, as in a desktop)
(define on-ac-power?
  (lambda (util)
    (cond ((string=? util "pmset") ;; Mac OS X
           (not-false? (not-null? (triple-grep "AC" "\\*" "Power" (call-with-input-split (string-append util " -g") (->string #\newline)))) #t))
          ((string=? util "acpi") ;; Linux
           (not-false? (regex#grep "on-line" (call-with-input-split (string-append util " -a"))) #t))
          ((string=? util "yacpi") ;; Linux
           (not-false? (regex#grep "charging" (call-with-input-split (string-append util " -pb"))) #t))
          ((string=? util "apm") ;; *BSD
           (not-false? (not-null? (double-grep "on-line" "AC" (call-with-input-split util (->string #\newline)))) #t))
          (else #f)))) ;; Unsupported
