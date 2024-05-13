import os
import subprocess
import unittest
from glob import glob

# Make _artifacts directory to store tmp files
os.makedirs('_artifacts', exist_ok=True) 

class TestAllInputs(unittest.TestCase):
    def test_all_inputs(self):
        input_files = glob('./inputs/*.in')
        for input_file in input_files:
            # Run test if corresponding output file exists
            output_file = input_file.replace('inputs', 'outputs').replace('.in', '.out')
            if os.path.exists(output_file):
                with self.subTest(input_file=input_file):
                    output_file = input_file.replace('inputs', 'outputs').replace('.in', '.out')
                    tmp_file = f"_artifacts/tmp_{os.path.basename(input_file).replace('.in', '.out')}"
                    # First command to compile and run the spellcheck, outputting to tmp_file
                    command1 = f"make && ./spellcheck.out < {input_file} > {tmp_file}"
                    subprocess.run(command1, shell=True, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)

                    # Manually strip whitespaces from tmp_file
                    with open(tmp_file, 'r') as file:
                        lines = file.readlines()
                        stripped_lines = [line.rstrip() + '\n' for line in lines]
                    with open(tmp_file, 'w') as file:
                        file.writelines(stripped_lines)

                    # Second command to diff the stripped tmp_file with the expected output
                    command2 = f"diff {tmp_file} {output_file}"
                    result = subprocess.run(command2, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
                    # Check if the test passed (diff returns 0 if files are the same)
                    self.assertEqual(result.returncode, 0, msg=f"Failed at {input_file}, check output at {tmp_file}. Differences:\n{result.stdout.decode('utf-8').strip()}")

                    # Print checkmark if test passed
                    print(f"✅ {input_file} passed")

# Test for memory leaks
class TestMemoryLeaks(unittest.TestCase):
    def test_memory_leaks(self):
        # Run only if platform is ubuntu
        if os.uname().sysname != 'Linux':
            return
        
        input_files = glob('./inputs/*.in')
        for input_file in input_files:
            with self.subTest(input_file=input_file):
                command = f"valgrind --leak-check=full --error-exitcode=1 ./spellcheck.out < {input_file}"
                result = subprocess.run(command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
                self.assertEqual(result.returncode, 0, msg=f"Memory leak detected at {input_file}:\n{result.stderr.decode('utf-8').strip()}")

                # Print checkmark if test passed
                print(f"✅ {input_file} memory leak test passed")


if __name__ == '__main__':
    unittest.main()
