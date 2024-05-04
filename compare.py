import os
from time import time
method1Time = []
method2Time = []
diff = []
for i in range(100):
    print("run number:%4d"%(i+1))
    st = time()
    os.system("cd method1/build/ && cmake .. >> /dev/null && make >> /dev/null && ./SHA1 >> /dev/null")
    et = time()
    method1Time.append(et - st)
    st = time()
    os.system("cd method2/build/ && cmake .. >> /dev/null && make >> /dev/null && ./SHA1 >> /dev/null")
    et = time()
    method2Time.append(et - st)
    diff.append(method1Time[i] - method2Time[i])

print("total time spent running 203 * 100 hashes using method1 | in seconds: ", sum(method1Time))
print("total time spent running 203 * 100 hashes using method1 | in seconds: ", sum(method2Time))
print("The average difference of the time taken in 100 runs is | in seconds: ", sum(diff))

