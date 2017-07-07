# This is to demo that function with in function is redefined each time when the out layer is called
# === result ===
# %python3 def_in_def_redef.py
# func2: 0
# func1: 0
# func2: 0
# func1: 1
# func2: 0
# func1: 2
# func2: 0
# func1: 3

def func1():
    if hasattr(func1, 'counter'):
        func1.counter += 1
    else:
        func1.counter = 0

    def func2():
        if hasattr(func2, 'counter'):
            func2.counter += 1
        else:
            func2.counter = 0
        print('func2:', func2.counter)
        return func2.counter
    func2()

    print('func1:', func1.counter)
    return func1.counter
   
func1()
func1()
func1()
func1()
