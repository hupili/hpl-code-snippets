def func1():
    print('func1')
    try:
        raise Exception
    except Exception as e:
        print('except')
        raise e
    finally:
        print('finally')
        return


def func2():
    print('func2')
    try:
        raise Exception
    except Exception as e:
        print('except')
        raise e
    finally:
        print('finally')
        #NOTE:
        #    There is no return here


func1()
func2()
