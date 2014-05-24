; /- filename -/
; regex-helper.scm

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
(use regex)
(declare (unit zb-regex))
(declare (uses zb-list))

(define expand-pattern
  (lambda (expand pattern)
    (if expand
        (regex#regexp pattern)
        (if (list? pattern)
            (car pattern)
            pattern))))

(define split-grep
  (lambda (pattern text #!optional use-regexp split-on)
    (if split-on
      (string-split (car-seat (regex#grep (expand-pattern use-regexp pattern) text) split-on))
      (string-split (car-seat (regex#grep (expand-pattern use-regexp pattern) text))))))

(define double-split-grep
  (lambda (second-pattern first-pattern text #!optional use-regexp split-on)
    (regex#grep (expand-pattern use-regexp second-pattern)
                (split-grep first-pattern text use-regexp split-on))))

(define double-grep
  (lambda (second-pattern first-pattern text #!optional use-regexp)
    (regex#grep (expand-pattern use-regexp second-pattern)
                (regex#grep (expand-pattern use-regexp first-pattern) text))))

(define triple-grep
  (lambda (third-pattern second-pattern first-pattern text #!optional use-regexp)
    (regex#grep (expand-pattern use-regexp third-pattern)
                (double-grep second-pattern first-pattern text use-regexp))))
