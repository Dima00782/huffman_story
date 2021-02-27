import subprocess
import time

start_time = time.time()
process = subprocess.run(['main/main', 'compress', 'perf_tests/war_and_peace.txt'], 
                         stdout=subprocess.PIPE, 
                         universal_newlines=True)
end_time = time.time()
# TODO: decrypt and compare files
print("--- %s seconds ---" % (end_time - start_time))
