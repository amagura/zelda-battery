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
(declare (unit regex-helper))

(define flatten-and-append
  (lambda (return-lst lst)
    (append return-lst
            (flatten lst))))

(define expand-pattern-lists-maybe ;`maybe' in the sense that non-boolean input is processed depending upon a conditionally, which is defined locally as an argument to be passed to this function.
  (lambda (expand #!rest ...)
    (if expand
        (list (map (lambda (x) (regex#regexp x))
              (if (list? ...) ... '(...))))
        (if (list? ...) ... '(...)))))

(define expand-patterns
  (lambda (expand #!rest ...)
    (flatten-and-append '() (expand-pattern-lists-maybe (if expand
                                (list (map (lambda (x) (regex#regexp x))
                                           (if (list? ...) ... '(...))))
                                (if (list? ...) ... '(...)))))))

(define twice-grep
  (lambda (second-pattern first-pattern text #!optional use-regexp)
    (regex#grep (expand-patterns use-regexp second-pattern)
                (regex#grep (expand-patterns use-regexp first-pattern) text))))
