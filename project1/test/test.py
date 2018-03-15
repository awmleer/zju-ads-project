from subprocess import Popen, PIPE, STDOUT
import time

for order in range(1,4):
    print("==========order %d=========="%order)
    for tree_type in range(1,4):
        print("tree type: "+str(tree_type))
        for size in range(1000,10001,1000):
            x=0
            for i in range(1,11):
                p = Popen(['../cmake-build-debug/project1'], stdout=PIPE, stdin=PIPE, stderr=PIPE)
                a = p.communicate(input=b'%d %d %d'%(size, tree_type, order))
                stdout_data = a[0].decode()
                time.sleep(0.01)
                try:
                    x += float(stdout_data)
                except ValueError:
                    print("Error: %s"%stdout_data)
                    print(a)
                    quit()
            print(round(x*100,3))
        
            
