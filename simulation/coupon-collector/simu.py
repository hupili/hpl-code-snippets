import random

N = 100000
n = 384
if __name__ == "__main__":
    ex = []
    for i in range(N):
        if i % 1000 == 0 :
            print float(sum(ex)) / (i+1)
        col = {}
        count = 0 
        while len(col) < n:
            col[random.randint(1,n)] = 1
            count += 1
        ex.append(count)
    print float(sum(ex)) / N

