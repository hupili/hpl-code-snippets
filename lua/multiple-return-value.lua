-- All roots from the question on SO:
--    * http://stackoverflow.com/questions/17648591/multiple-value-return-pattern-in-python-not-tuple-list-dict-or-object-soluti
-- Someone in PyCN Google Group mentioned the multiple-return-value of Lua.
-- I try to construct one example to check whether this is the pattern we want.

-- Short conclusion:
--    * The lua style of multi-value is what we are pursuing
--    * When the last parameter of a function call is a function,
--    all the returned values will be put in that position.
--    * As a rule of thumb, one had better always write
--    `h( (g( (f()) )) )`. The extra level of brackets will only
--    keep the first return value.
--    * This is different tradeoff from common LISP:
--    single value is default; multi value should be explicitly binded before use.

-- The first version of floor
function floor1(n)
    return math.floor(n)
end

-- Client 1: careful with potential future multiple value return
print ((floor1(10/3)) * (floor1(10/3)))
--> 9

-- Client 2: not so careful
print (floor1(10/3) * floor1(10/3))
--> 9

function myexp2(i, extra)
    print ("In myexp2, i=", i)
    print ("In myexp2, extra=", extra)
    return math.pow(2,i)
end

-- Client 4: cascade function call. Careful
print (myexp2((floor1(10/3))))
--> In myexp2, i=   3
--> In myexp2, extra=   nil
--> 8

-- Client 5: cascade function call. Not careful
print (myexp2(floor1(10/3)))
--> In myexp2, i=   3
--> In myexp2, extra=   nil
--> 8

-- Upgrade: return the residual to give the client more info
function floor2(n)
    _f = math.floor(n)
    _r = n - _f
    return _f, _r
end

print ("Direct print:", floor2(10/3))
print ("Single value print:", (floor2(10/3)))
--> Direct print:   3   0.33333333333333
--> Single value print:   3

-- Client 1: careful with potential future multiple value return
print ((floor2(10/3)) * (floor2(10/3)))
--> 9

-- Client 2: not so careful
print (floor2(10/3) * floor2(10/3))
--> 9

-- Client 3: use more information returned by our new version
f, r = floor2(10/3)
print("Floored:", f, "Residual:", r)
--> Floored:    3   Residual:   0.33333333333333

-- Client 4: cascade function call. Careful
print (myexp2((floor2(10/3))))
--> In myexp2, i=   3
--> In myexp2, extra=   nil
--> 8

-- Client 5: cascade function call. Not careful
print (myexp2(floor2(10/3)))
--> In myexp2, i=   3
--In myexp2, extra= 0.33333333333333
--> 8
