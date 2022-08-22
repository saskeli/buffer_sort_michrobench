import sys
from random import Random

def main(count, buf, output_type):
    print(count)
    r = Random()
    for _ in range(count):
        for i in range(buf):
            if (output_type == "random"):
                print(f"{r.randint(0, 2**14 - 1 - i)} {i}")
            if (output_type == "sorted"):
                print(f"{i} {i}")
            if (output_type == "inverted"):
                print(f"{0} {i}")

if __name__ == "__main__":
    if (len(sys.argv) < 3):
        print("Give n and b")
        exit()
    output_type = "random"
    if (len(sys.argv) > 3):
        output_type = sys.argv[3]
    count = int(sys.argv[1])
    buf = int(sys.argv[2])
    main(count, buf, output_type)