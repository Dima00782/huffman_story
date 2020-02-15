import subprocess
import time

start_time = time.time()
process = subprocess.run(['main/main', 'crypt', 'perf_tests/lorem_ipsum.txt'], 
                         stdout=subprocess.PIPE, 
                         universal_newlines=True)
end_time = time.time()
print("--- %s seconds ---" % (end_time - start_time))
