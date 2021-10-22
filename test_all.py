import os
import subprocess
import sys

def main():
    binary = sys.argv[1]
    count = 0
    for s in os.listdir():
        if s.startswith("test"):
            assert s.endswith(".tsv"), "Expected format: test01.tsv, found {}".format(s)
            num = s[4:-4]
            expected_filename = "expected" + num + ".tsv"
            assert os.path.isfile(expected_filename), "{} doesnt exist".format(expected_filename)
            output_filename = "out" + num + ".tsv"
            with open(output_filename, "w") as f:
                cat_cmd = ["cat", s]
                cat = subprocess.Popen(cat_cmd, stdout=subprocess.PIPE)
                binary_cmd = [binary]
                subprocess.check_call(binary_cmd, stdin=cat.stdout, stdout=f)
            subprocess.check_call(["diff", output_filename, expected_filename])
            count += 1
    print("{} success!".format(count))
    assert count > 0


if __name__ == "__main__":
    main()