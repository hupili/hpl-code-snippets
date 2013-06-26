;Run: clisp multi-return-value.cl 
;This is to test the multi return pattern. 
;It's very convenient for program upgrade.
(format t "~a~%" (multiple-value-bind (f r) 
   (floor 130 11)
	  (list f r)))
(format t "~a~%" (floor 130 11))
