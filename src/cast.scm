; /- filename -/
; cast.scm

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
(declare (unit zbcast))

;; if you have a percentage: 53%, this will convert it to 50.
;; if you have 45%, it will convert it to 40%.
;; essentially, it takes whatever number you give it and truncates it and then pads it with a zero.
;; 48 -> 40, 102 -> 100, 155 -> 150, 01 -> 0, etc

(define percent->integer
  (lambda (perc)
    (if (or (equal=? perc +inf.0) (equal=? perc -inf.0))
      perc
      (inexact->exact (* (truncate (* (/ (string->number perc) 100) 10)) 10)))))
