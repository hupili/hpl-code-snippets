; Run: clisp multi-return-value.cl 
; This is to test the multi return pattern. 
; It's very convenient for program upgrade.

; Test the multi-return of built-in 'floor' function
(format t "~a~%" (multiple-value-bind (f r) 
   (floor 130 11)
	  (list f r)))
(format t "~a~%" (floor 130 11))

; ==== Following is a test to show the power of multi-return ===

; Your original function and how it is called by client
(defun myfunc (a)
 "a test function"
 a)
(format t "~a~%" (+ (myfunc 1) (myfunc 2)))

; Now we upgrade the function
(defun myfunc2 (a)
 "This is the upgrade of 'a test function'"
 (values-list (list a (* a 2))))
; The original call is not influenced
(format t "~a~%" (+ (myfunc2 1) (myfunc2 2)))
; You can utilize multiple return values
(format t "~a~%" 
 (multiple-value-bind (v1 v2) 
  (myfunc2 2)
  (* v1 v2)))

