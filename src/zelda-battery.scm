; /- filename -/
; zelda-battery.scm

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
(use srfi-1)
(declare (uses main-helper))

#>
extern const char * cppToScheme__host_acpi_util(void);
<#
(define x-host-acpi-util (foreign-lambda c-string "cppToScheme__host_acpi_util"))

(let ((battery (percent->integer (assume-power (x-host-acpi-util)))))
 (cond ((= battery +inf.0)
        (map (lambda (char) (display heart) (display empty-heart)) ; 0101010101 (means that your machine is probably a desktop or a virtual machine)
             (make-list 5)))
       ((= battery -inf.0)
        (map (lambda (char) (display empty-heart)) ; 000000001 (means no supported utilities are present)
             (make-list 9))
        (display heart))
       (else        
         ;; print how much power is left out of 100%
         (map (lambda (char) (display heart))
              (make-list (/ battery 10)))
         ;; print how much power has been used out of 100%
         (map (lambda (char) (display empty-heart))
              (make-list (- 10 (/ battery 10)))))))
