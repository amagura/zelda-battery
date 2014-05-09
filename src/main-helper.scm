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
(use shell posix list-utils) ; use the `shell` egg
(declare (unit main-helper)) ; makes it so that other chicken scheme files can use the stuff defined in this file.
(declare (uses regex-helper))

;; if the outcome of `procedure` does not === (absolutely and completely equal) #f (false), then return #t (true)
(define not-false?
  (lambda (proc #!optional bool)
   (or (and bool proc #t) (and proc))))

;; if you have a percentage: 53%, this will convert it to 50.
;; if you have 45%, it will convert it to 40%.
;; essentially, it takes whatever number you give it and truncates it and then pads it with a zero.
;; 48 -> 40, 102 -> 100, 155 -> 150, 01 -> 0, etc
(define percent->integer
  (lambda (perc)
    (if (or (equal=? perc +inf.0) (equal=? perc -inf.0))
      perc
      (inexact->exact (* (truncate (* (/ (string->number perc) 100) 10)) 10)))))

(define call-with-input-split
  (lambda (cmdline #!optional split-on mode)
    (if split-on
      (string-split (call-with-input-pipe cmdline (or mode read-all)) split-on)
      (string-split (call-with-input-pipe cmdline (or mode read-all))))))

(define acpiconf-which-device-is-battery?
  (lambda (index)
    (or (when (not-null? (call-with-input-split "acpiconf -i " (string-append index " 2> /dev/null"))) index)
        (acpiconf-which-device-is-battery? (if (>= index 11) (- idex 1) (if (>= 0 index) (+ index 1) index))))))


;;; is the current machine running off AC Power (don't see why this wouldn't work on machines that do not have a battery, as in a desktop)
(define on-ac-power?
  (lambda (util)
    (cond ((string=? util "pmset") ;; Mac OS X
           (not-false? (string-contains "AC" (car (twice-grep "\\*" "Power" (call-with-input-split (string-append util " -g") (->string #\newline))))) #t))
          ((string=? util "acpi") ;; Linux
           (not-false? (regex#grep "on-line" (call-with-input-split (string-append util " -a"))) #t))
          ((string=? util "yacpi") ;; Linux
           (not-false? (regex#grep "charging" (call-with-input-split (string-append util " -pb"))) #t))
          ((string=? util "apm") ;; *BSD
           (not-false? (not-null? (twice-grep "on-line" "AC" (call-with-input-split util (->string #\newline)))) #t))
          ;((string=? util "acpiconf") ;; *BSD
           ;(not-null? (twice-grep "charging" "State:" (car (filter (map
            ;(regex#grep "charging"
             ;(regex#grep "State:"
              ;(car (filter (lambda (x) (not-null? x))
               ;(loop for idex from 10 downto 0 collect
                                   ;(string-split (capture ,(string-append util " -i " (number->string idex) " 2> /dev/null")) (->string #\newline)))))))))

          (else #f)))) ;; Unsupported

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

(define car-seat ; a `car' that is safe enough to use where `car' would otherwise be unsafe to use by itself.
  (lambda (lst #!optional return-on-fail)
    (if (not-null? lst)
      (car lst)
      (if return-on-fail return-on-fail '(())))))
      
(define get-power-level
  (lambda (util)
    (cond ((string=? util "pmset")
           (regex#string-substitute (regex#regexp "%.*") ""
                                    (car-seat (regex#grep "%" (call-with-input-split (string-append util " -g ps"))) '("%"))))
          ((string=? util "acpi")
           (regex#string-substitute (regex#regexp "%.*") ""
                                    (car-seat (regex#grep "%" (call-with-input-split util)) '("%"))))
          ((string=? util "yacpi")
           (regex#string-substitute (regex#regexp "%.*") ""
                                    (car-seat (regex#grep "%" (call-with-input-split (string-append util " -pb"))) '("%"))))
          ;((string=? util "acpiconf")
           ;(regex#string-substitute (regex#regexp "%.*") ""
            ;(car (or
                  ;(not-null? (regex#grep "%" (loop for idex from 10 downto 0 collect
                                              ;(car (or
                                                    ;(not-null? (regex#grep "%" (string-split (capture ,(string-append "acpiconf -i " (number->string idex) " 2> /dev/null")))))
                                                    ;'(""))))))
                  ;'("%")))))
          (else ""))))

(define heart "\u2665")
(define empty-heart "\u2661")
