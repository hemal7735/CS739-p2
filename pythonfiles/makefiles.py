x=1
name='a'
todo=31
for i in range(1,31):
    x=x*2
    f = open(chr(ord(name)) + str(i-1),"w+")
    for j in range(x):
        f.write("a")
    f.close()
