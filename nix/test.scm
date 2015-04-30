(use utils posix test)
(define (truncate-newline str)
  (string-trim-right (string-translate str "\n" " ")))

(test "zbatt outputs 10 chars"
             10
             (string-length (truncate-newline (call-with-input-pipe "./zbatt -f + -e -" read-all))))
(exit)
