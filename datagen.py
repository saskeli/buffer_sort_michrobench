import sys

def main(count, buf):
    print(count)
    for _ in range(count):
        for i in range(buf):
            print(i, i)

if __name__ == "__main__":
    if (len(sys.argv) < 3):
        print("Give n and b")
        exit()
    count = int(sys.argv[1])
    buf = int(sys.argv[2])
    main(count, buf)