#!/usr/bin/env csi -qn

(define battery-power-level
  (let ((acpi (
  (let ((acpi (cond ((equal=? (car (system-information

(let ((acpi (cond ((equal=? (car (system-information)) "Darwin") "pmset
(if (equal=? (car (system-information)) "Darwin")
  (
