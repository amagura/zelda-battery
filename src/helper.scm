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
(use shell regex) ; use the `shell` egg
(declare (unit helper)) ; makes it so that other chicken scheme files can use the stuff defined in this file.

;; if the outcome of `procedure` does not === (absolutely and completely equal) #f (false), then return #t (true)
(define not-false?
  (lambda (procedure)
    (and #t (and procedure #t))))

;; if you have a percentage: 53%, this will convert it to 50.
;; if you have 45%, it will convert it to 40%.
;; essentially, it takes whatever number you give it and truncates it and then pads it with a zero.
;; 48 -> 40, 102 -> 100, 155 -> 150, 01 -> 0, etc
(define percent->integer
  (lambda (perc)
    (if (or (equal=? perc +inf.0)
            (equal=? perc -inf.0))
      perc
      (inexact->exact (* (truncate (* (/ (string->number perc) 100) 10)) 10)))))

;;; is the current machine running off AC Power (don't see why this wouldn't work on machines that do not have a battery, as in a desktop)
(define on-ac-power
  (lambda (util)
    ;; check 
    (cond ((string=? util "pmset")
           (not-false? (string-contains (car (regex#grep (regex#regexp "\\*")
                                               (regex#grep (regex#regexp "Power") (string-split (capture "pmset -g") (->string #\newline))))))))

          ((string=? util "acpi")
           (not-false? (regex#grep "on-line" (string-split (capture "acpi -a")))))

          ((string=? util "yacpi")
           (not-false? (regex#grep "charging" (string-split (capture "yacpi -pb")))))
          
          ((string=? util "acpiconf")
           '()))))

;; if the outcome of `get-power-level` is not an integer, (which might indicate an error,
;; which error may or may not be a problem: desktops don't have batteries so trying to get the 
;; battery level is kinda... impossible, and virtual machine emulations such as QEMU and 
;; VirtualBox don't always emulate power supply hardware...
;; so if the outcome of `get-power-level` is abnormal, assume that we're running on a piece
;; of hardware that either does not include a battery or does not provide any form of power supply hardware whatsoever
(define assume-power
  (lambda (util)
    (let ((power-level (get-power-level util)))
     (cond ((number? (string->number power-level)) power-level)
           ((eq? power-level (get-power-level "")) -inf.0) ; no utility present or an unsupported utility was somehow (should be impossible unless it got hard-coded into cppToScheme.c) used.
           (else +inf.0))))) ; infinity is used here to communicate that the zelda-blink needs to use a special color sequence to indicate that the current system is _special_, lol, and that Zelda Battery has no means of determining the current power level or even if there is a power level.
  
(define get-power-level
  (lambda (util)
    (cond ((string=? util "pmset")
           (regex#string-substitute (regex#regexp "%.*") ""
                                    (car (regex#grep "%" (string-split (capture "pmset -g ps"))))))

          ((string=? util "acpi")
           (regex#string-substitute (regex#regexp "%.*")
                                    (car (regex#grep "%" (string-split (capture "acpi"))))))

          ((string=? util "yacpi")
           (regex#string-substitute (regex#regexp "%.*") ""
                                   (car (regex#grep "%" (string-split (capture "yacpi -pb"))))))

          ((string=? util "acpiconf"))))) ; not fully supported (as in not supported at all) yet.  I need to install *BSD on my laptop before I can test this and add support.

(define heart "\u2665")
(define empty-heart "\u2661")
