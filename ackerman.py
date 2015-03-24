import math
import subprocess
powers_of_two = []
base = 1
for i in range(10):
    powers_of_two[i] = math.pow(2, i)
    if(base % powers_of_two[i]) == 0:
        print "---------------------------"
        print "Testing" + base, base * powers_of_two
        subprocess.call(['./ackermantest', base, powers_of_two[i]])
    else:
        print "failure"

