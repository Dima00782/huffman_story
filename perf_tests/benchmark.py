import subprocess
import time
import filecmp

def single_byte_letter_benchmark():
    input_file = 'perf_tests/war_and_peace.txt'
    output_file = 'war_and_peace.txt.huf'
    new_input_file = 'war_and_peace.txt'

    # compress the input file
    start_time = time.time()
    subprocess.run(['main/main', 'compress', input_file],
                    stdout=subprocess.PIPE,
                    universal_newlines=True)
    end_time = time.time()
    print("--- compression takes %s seconds ---" % (end_time - start_time))

    # decompress the output file
    start_time = time.time()
    subprocess.run(['main/main', 'decompress', output_file],
                    stdout=subprocess.PIPE, 
                    universal_newlines=True)
    end_time = time.time()
    print("--- decompression takes %s seconds ---" % (end_time - start_time))

    # sanity: files should be equal
    if filecmp.cmp(input_file, new_input_file):
        print("--- PASS ---")
    else:
        print("--- FAIL: resulted files aren't equal! ---")

def alphabet_letter_benchmark():
    input_file = 'perf_tests/example.cc'
    output_file = 'example.cc.huf'
    new_input_file = 'example.cc'
    alphabet_file = 'perf_tests/sample_alphabet.txt'

    # compress the input file
    start_time = time.time()
    subprocess.run(['main/main', 'compress', input_file, '--alphabet', alphabet_file],
                    stdout=subprocess.PIPE,
                    universal_newlines=True)
    end_time = time.time()
    print("--- compression takes %s seconds ---" % (end_time - start_time))

    # decompress the output file
    start_time = time.time()
    subprocess.run(['main/main', 'decompress', output_file, '--alphabet'],
                    stdout=subprocess.PIPE, 
                    universal_newlines=True)
    end_time = time.time()
    print("--- decompression takes %s seconds ---" % (end_time - start_time))

    # sanity: files should be equal
    if filecmp.cmp(input_file, new_input_file):
        print("--- PASS ---")
    else:
        print("--- FAIL: resulted files aren't equal! ---")

print("--- benchmark normal mode ---")
single_byte_letter_benchmark()
print()

print("--- benchmark alphabet mode ---")
alphabet_letter_benchmark()
print()
