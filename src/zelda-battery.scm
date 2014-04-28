#|---
  /- filename -/
  zelda-battery.scm

  /- copyright -/
  Copyright (c) 2014 Alexej Magura

  This file is part of Zelda Battery.

  Zelda Battery is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Zelda Battery is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Zelda Battery.  If not, see <http://www.gnu.org/licenses/>.
---|#
#!/usr/bin/env csi -qn

#|
(define battery-power-level
  (let ((acpi (
  (let ((acpi (cond ((equal=? (car (system-information

(let ((acpi (cond ((equal=? (car (system-information)) "Darwin") "pmset
(if (equal=? (car (system-information)) "Darwin")
  (
   |#
